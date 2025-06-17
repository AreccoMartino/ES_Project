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


volatile int STATE = WAIT_FOR_START;
// unsigned int missed_rx_bytes = 0;
// unsigned int missed_tx_bytes = 0;
volatile char rx_buffer_array[RX_BUFFER_SIZE];
volatile CircularBuffer rxBuffer;
volatile char tx_buffer_array[TX_BUFFER_SIZE];
volatile CircularBuffer txBuffer;


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
            // missed_rx_bytes ++;
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
    IFS1bits.INT1IF = 0;        
    IEC0bits.T2IE = 1;
    tmr_setup_period(TIMER2, 10);
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt() {
    IFS0bits.T2IF = 0;      
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



int main(void) {
    
    int linear_rate = 0;
    int yaw_rate = 0;
    char rx_byte;
    int ir_cm = 0;
    float ir_volt = 0.0f;
    float latest_batt_v  = 0.0f;

    unsigned int count_emergency = 0;
    unsigned int count = 0;

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
    leds_init();
    lights_init();
    buttons_init();
    uart_init();
    spi_init();
    oc_init();
    oc_pins_init();
    oc_start();
    ir_init();              // Enables sensor and sets pin B14 to analog mode
    battery_init();         // Set pin B11 to analog mode for battery reading
    adc1_init(); 
    adc2_init(); 
    acc_init(); 
    
    // Setup interrupts
    // global_interrupt_enable();       // BOH 
    uart_rx_interrupt_enable();
    // NB: We don't enable the tx interrupt yet as it will be done by uart_send_string once needed
    RPINR0bits.INT1R = 0x58;    // Associate INT1 functionality to RE8 (RP88=0x58)
    global_interrupt_enable();  // FOR SOME REASON THIS NEEDS TO BE HERE
    IFS1bits.INT1IF = 0;        // Clear flag
    IEC1bits.INT1IE = 1;        // Enable specific interrupt
    
    // Setup TIMER1 to time main loop at 500 Hz
    tmr_setup_period(TIMER1, 2);           

    while (1) {

        // Here we start manual sampling. Conversion (about 1.5 μs) will follow immediately after.
        // So by the time we reach the busy-wait, it's very likely already finished.
        // This lets us execute other code while the ADC is sampling in the background.
        adc1_start_sampling();          // Sample at each loop (500 Hz)
        if (count == 0) {               // Sample every 500 loops (1 Hz) 
            adc2_start_sampling();
        }

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
                } else if (strcmp(ps.msg_type, "PCSTP") == 0) {
                    if (STATE == EMERGENCY) {
                        uart_send_string(&txBuffer, "$MACK,0*\n");
                    } else {
                        STATE = WAIT_FOR_START;
                        uart_send_string(&txBuffer, "$MACK,1*\n");
                    }    
                } else if (strcmp(ps.msg_type, "PCSTT") == 0) {
                    if (STATE == EMERGENCY) {
                        uart_send_string(&txBuffer, "$MACK,0*\n");
                    } else {
                        STATE = MOVING;
                        uart_send_string(&txBuffer, "$MACK,1*\n");
                    } 
                }
            }    
        }



        // Here we busy-wait for the ADC1 conversion to complete to make sure we have the new reading.
        // Nevertheless, we expect to find the conversion finished by the time we get here, 
        // because sampling (about 2 μs) and conversion (about 1.5 μs) should be already completed in the background.
        // Knowing that 1TAD = 9TCY and we have 12TAD per conversion:
        // ==> total ≅ 12 * 9 * (1 / 72 MHz) ≅ 1.5 μs
        // So by the time we reach this point, the 1.5 μs have already elapsed.
        // Same reasoning applies to the ADC2.
        while (!AD1CON1bits.DONE);      
        int ir_reading = ADC1BUF0;
        AD1CON1bits.DONE = 0;          // Clear the DONE bit 
        ir_volt = (float)ir_reading / 4095.0f * 3.3f;

        ir_cm = ir_compute_distance(ir_volt);

        if (ir_cm < DISTANCE_THRESHOLD) {
            // If we go under the threshold, change the state to EMERGENCY and send feedback on the UART (only once)
            if (STATE != EMERGENCY) {
                STATE = EMERGENCY;
                uart_send_string(&txBuffer, "$MEMRG,1*\n");
            }
            // We reset the counter EVERY time we go under the threshold
            count_emergency = 0;
        } else if (STATE == EMERGENCY && ++count_emergency >= 2500) { 
            // Resume from emergency when 5 seconds (2500 cycles) without going under the threshold have passed
            // Only increment if we’re already in EMERGENCY. (&& operator is short-circuiting).
            // This prevents count_emergency from growing indefinitely when the sensor stays far away.
            STATE = WAIT_FOR_START;
            LEDL = 0;                       // Reset LEDs
            LEDR = 0;
            uart_send_string(&txBuffer, "$MEMRG,0*\n");
        }

        // Handle IR distance feedback on the UART at 10 Hz 
        if ((count + 2) % 50 == 0) {
            char msg[16];
            format_msg_mdist(msg, ir_cm);
            uart_send_string(&txBuffer, msg);
        }

        // Handle acc feedback on the UART at 10 Hz
        if ((count + 1) % 50 == 0) {
            int raw_axes[3];
            acc_read_axes(raw_axes);
            char msg[32];
            format_msg_macc(msg, raw_axes[0], raw_axes[1], raw_axes[2]);
            uart_send_string(&txBuffer, msg);
        }

        // Handle the feedback of the battery reading at 1 Hz (once every 500 cycles)
        if (count == 0) {  
            while (!AD2CON1bits.DONE);          // busy-wait for safety even though at this point the conversion should already be done
            int battery_reading = ADC2BUF0;
            AD2CON1bits.DONE = 0;          // Clear the DONE bit 
            latest_batt_v = (float)battery_reading / 1023.0f * 3.3f * 3.0f;
            char msg[16];
            sprintf(msg, "$MBATT,%.2f*\n", (double)latest_batt_v);
            uart_send_string(&txBuffer, msg);
        }
        
        // Handle LED1 blinking at 1 Hz (toggled at 2 Hz)
        if (count % 250 == 0) {
            LED1 = !LED1;
            if (STATE == EMERGENCY) {
                LEDL = !LEDL;
                LEDR = !LEDR;
            }
        }
        
        switch (STATE) {
            case WAIT_FOR_START:
                set_motor_speeds(0, 0);
                LED2 = 0;               ////// DEBUG //////
                break;
            case MOVING:
                set_motor_speeds(linear_rate, yaw_rate);
                LED2 = 1;               ////// DEBUG //////
                break;
            case EMERGENCY:
                set_motor_speeds(0, 0);
                LED2 = 0;               ////// DEBUG //////
                break;
        }

        count = (count + 1) % 500;      // Reset count every 500 iterations to keep it in range [0, 499]

        // Wait until next period
        if (tmr_wait_period(TIMER1) == 1) LEDFRONT = 1;              
        else LEDFRONT = 0;                       
    }
    return 0;
}
