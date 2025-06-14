#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../LIBRARY.X/general_purpose_lib.h"
#include "../LIBRARY.X/timer_lib.h"
#include "../LIBRARY.X/uart_lib.h"
#include "../LIBRARY.X/spi_lib.h"
#include "../LIBRARY.X/parser_lib.h"
#include "../LIBRARY.X/pwm_lib.h"
#include "../LIBRARY.X/adc_lib.h"

int STATE = WAIT_FOR_START; // Initial state of the system

volatile char rx_buffer_array[RX_BUFFER_SIZE];
volatile CircularBuffer rxBuffer;
volatile char tx_buffer_array[TX_BUFFER_SIZE];
volatile CircularBuffer txBuffer;

unsigned int missed_rx_bytes = 0;
unsigned int missed_tx_bytes = 0;

// Global variables for latest sensor readings
volatile float latest_ir_cm   = 0.0f;
volatile float latest_batt_v  = 0.0f;


void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void) {
    
    IFS0bits.U1RXIF = 0;                // Clear interrupt flag
    // Read bytes from FIFO until they're available. If by any chance we had more than ...
    // ... one byte in the FIFO this allows to read them all and avoid the risk ...
    // ... of accumulating them until the FIFO overruns
    while (U1STAbits.URXDA) {
        char received_byte = U1RXREG;       // Read received character
        if (Buffer_Write(&rxBuffer, received_byte) == -1) {
            // If the rxBuffer is full, the received_byte is lost, so we could ...
            // ... increase a counter to keep track of how many bytes we lost
            missed_rx_bytes ++;
        }
        // As soon as there is no more data available in the FIFO we exit the loop and the ISR
    }
}


void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void) {
    
    IFS0bits.U1TXIF = 0;
    char byte;
    while (!U1STAbits.UTXBF) {
        if (Buffer_Read(&txBuffer, &byte) == 0) {
            U1TXREG = byte;
        } else {
            uart_tx_interrupt_disable();
            // If txBuffer is empty, there are no more characters to be sent for now so ...
            // ... we disable the tx interrupt. It will be reenabled by the uart_send_string ...
            // ... function once new characters are added to the txBuffer
            break;  // Exit the loop to avoid getting stuck in a infinite loop
        }
    }
}


void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt() {
    IFS1bits.INT1IF = 0;        // Reset INT1 interrupt flag
    IEC0bits.T2IE = 1;
    tmr_setup_period(TIMER2, 10);
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt() {
    IFS0bits.T2IF = 0;        // Reset INT1 interrupt flag
    IEC0bits.T2IE = 0;
    T2CONbits.TON = 0;
    IEC1bits.INT1IE = 1;
    
    if (BUTTON1 == 1) {
        if (STATE == WAIT_FOR_START) {
            STATE = MOVING;
        } else if (STATE == MOVING) {
            STATE = WAIT_FOR_START;
        } 
    }
    
}

void __attribute__((__interrupt__, no_auto_psv)) _AD1Interrupt(void) {
    IFS0bits.AD1IF = 0;
    int raw = ADC1BUF0;
    float v_ir = (float)raw / 4095.0f * 3.3f;
    latest_ir_cm = ir_compute_cm(v_ir);
}

void __attribute__((__interrupt__, no_auto_psv)) _AD2Interrupt(void) {
    IFS1bits.AD2IF = 0;
    int raw = ADC2BUF0;
    latest_batt_v = (float)raw / 4095.0f * 3.3f * 3.0f;
}




int main(void) {
    
    int linear_rate = 0;
    int yaw_rate = 0;
    unsigned int count_led = 0;
    char rx_byte; 
    int count_battery = 0; 
    int count_ir = 0;

    // Initialize circular buffers
    Buffer_Init(&rxBuffer, rx_buffer_array, RX_BUFFER_SIZE);        // Initialize RX buffer
    Buffer_Init(&txBuffer, tx_buffer_array, TX_BUFFER_SIZE);        // Initialize TX buffer

    // Initialize parser
    parser_state ps;
	ps.state = STATE_DOLLAR;
	ps.index_type = 0; 
	ps.index_payload = 0;


    

    // Initialize peripherals
    set_digital_mode();

    // Set up ADC
    //ANSELBbits.ANSB14 = 1;
    TRISBbits.TRISB9 = 0; //output PIN A3-->RST1 to activate EN on sensor
    LATBbits.LATB9 = 1;
    ANSELBbits.ANSB14 = 1; //set pin AN1 == RB15 in anolog mode;
    TRISBbits.TRISB14 = 1; //input

    leds_init();
    lights_init();
    buttons_init();
    uart_init();
    spi_init();
    oc_init();
    oc_pins_init();
    oc_start();
    adc1_init(); 
    adc2_init();  
    
    // Setup interrupts
    // global_interrupt_enable();       // BOH 
    uart_rx_interrupt_enable();
    // NB: We don't enable the tx interrupt yet as it will be done by uart_send_string once needed
    RPINR0bits.INT1R = 0x58;    // Associate INT1 functionality to RE8 (RP88=0x58)
    global_interrupt_enable();  // FOR SOME REASON THIS NEEDS TO BE HERE
    IFS1bits.INT1IF = 0;        // Clear flag
    IEC1bits.INT1IE = 1;        // Enable specific interrupt
    
    

    // Setup timers
    tmr_setup_period(TIMER3, 2);           // Setup TIMER3 to time main loop at 500 Hz
    // Note: we used TIMER3 so that we can also use it to trigger ADC1 sampling at 500 Hz

    while (1) {

        // This loop parses one by one all the new, unread bytes in the rxBuffer
        while (1) {
            // Critical section to protect the read buffer operation
            uart_rx_interrupt_disable();
            int read_result = Buffer_Read(&rxBuffer, &rx_byte);
            uart_rx_interrupt_enable();

            if (read_result == -1) 
                break;       // Break the loop if the rxBuffer is empty

            if (parse_byte(&ps, rx_byte) == NEW_MESSAGE) {
                if (strcmp(ps.msg_type, "PCREF") == 0) {
                    linear_rate = extract_integer(ps.msg_payload); 
                    int idx = next_value(ps.msg_payload, 0);
                    yaw_rate = extract_integer(ps.msg_payload + idx);  
                }
            }
            
        }

        switch (STATE) {
            case WAIT_FOR_START:
                set_motor_speeds(0, 0);
                LED2 = 0; 
                break;
            case MOVING:
                set_motor_speeds(linear_rate, yaw_rate);
                LED2 = 1;
                break;
            case EMERGENCY:
                break;
        }

        // Handle LED1 blinking at 1 Hz
        if (++count_led >= 250) {
            count_led = 0;
            LED1 = !LED1;
        }

        
        
        if (count_battery == 250) {
            char msg[50];
            sprintf(msg, "Battery: %.2f V\n", latest_batt_v);
            uart_send_string(&txBuffer, msg);
        }

        if (++count_battery >= 500) {  
            count_battery = 0;
            adc2_start_sampling();
        }

        if (++count_ir >= 50) {  
            count_ir = 0;
            char msg[50];
            sprintf(msg, "IR: %.2f cm\n", latest_ir_cm);
            uart_send_string(&txBuffer, msg);
        }




        // Wait until next period
        if (tmr_wait_period(TIMER3) == 1) {   
            // missed deadline
            LEDFRONT = 1;                    
        }
        else {                             
            LEDFRONT = 0;                       
        }

    }

    return 0;
}
