#include <xc.h>
#include <string.h>
#include <stdio.h>
#include "../LIBRARY.X/config.h"
#include "../LIBRARY.X/general_purpose_lib.h"
#include "../LIBRARY.X/timer_lib.h"
#include "../LIBRARY.X/uart_lib.h"
#include "../LIBRARY.X/spi_lib.h"
#include "../LIBRARY.X/parser_lib.h"
#include "../LIBRARY.X/pwm_lib.h"
#include "../LIBRARY.X/adc_lib.h"



volatile int button1_pressed = 0; 
volatile unsigned int missed_rx_bytes = 0;
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
        button1_pressed = 1; 
    }   
}



int main(void) {
    
    // Initialize variables
    int STATE = WAIT_FOR_START;
    unsigned int count = 0;
    unsigned int count_emergency = 0;
    unsigned int missed_tx_bytes = 0;       // This is not touched by ISRs so we can keep it local to the main
    int linear_rate = 0;
    int yaw_rate = 0;
    int ir_cm = 0;
    float ir_volt = 0.0f;
    float latest_batt_v  = 0.0f;
    char rx_byte;

    // Initialize circular buffers
    Buffer_Init(&rxBuffer, rx_buffer_array, RX_BUFFER_SIZE);        // Initialize RX buffer
    Buffer_Init(&txBuffer, tx_buffer_array, TX_BUFFER_SIZE);        // Initialize TX buffer

    // Initialize parser
    parser_state ps;
	ps.state = STATE_DOLLAR;
	ps.index_type = 0; 
	ps.index_payload = 0;

    // Initialize peripherals
    set_digital_mode();             // Set all pins to digital mode
    set_interrupt_priorities();     // Set the priority for each of the ISRs
    leds_init();                    // Set all LED pins to output mode
    lights_init();                  // Set all light pins to output mode
    button1_init();                 // Initialize button1 and map to INT1
    ir_init();                      // Enable IR sensor and sets pin B14 to analog mode
    battery_init();                 // Set pin B11 to analog mode for battery reading
    adc1_init();                    // Initialize ADC1 for IR sensor, manual start, automatic end of sampling and conversion
    adc2_init();                    // Initialize ADC2 for battery, manual start, automatic end of sampling and conversion
    spi_init();                     // Initialize SPI at 6 MHz 
    acc_init();                     // Set accelerometer sampling frequency to 15.625 Hz (64 ms)
    oc_init();                      // Initialize OC modules for PWM at 10 kHz
    oc_pins_init();                 // Map OC to the correct pins
    oc_start();                     // Start PWM generation
    uart_init();                    // Initialize UART with 9600 bps baud rate, 8 data bits, no parity, 1 stop bit
    
    // NB: We don't enable the tx interrupt yet as it will be done by uart_send_string once needed
    uart_rx_interrupt_enable();
    button1_interrupt_enable();   
    global_interrupt_enable();     
    
    // Setup TIMER1 to time main loop at 500 Hz
    tmr_setup_period(TIMER1, 2);           

    while (1) {
        // This flag is only ever set in the ISR and cleared in the main, so we shouldn't need to protect it with a critical section
        if (button1_pressed) {              // If button1 is pressed, toggle the state
            button1_pressed = 0;            // Reset the button press flag
            if (STATE == WAIT_FOR_START) {
                STATE = MOVING;   
            } else if (STATE == MOVING) {
                STATE = WAIT_FOR_START;
            }
        }
        // Here we start manual sampling. Sampling+conversion in this configuration should take 3.5 us.
        // So by the time we reach the busy-wait, it's very likely already finished.
        // This lets us execute other code while the ADC is sampling and converting in the background.
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
                        missed_tx_bytes += uart_send_string(&txBuffer, "$MACK,0*\n");
                    } else {
                        STATE = WAIT_FOR_START;
                        missed_tx_bytes += uart_send_string(&txBuffer, "$MACK,1*\n");
                    }    
                } else if (strcmp(ps.msg_type, "PCSTT") == 0) {
                    if (STATE == EMERGENCY) {
                        missed_tx_bytes += uart_send_string(&txBuffer, "$MACK,0*\n");
                    } else {
                        STATE = MOVING;
                        missed_tx_bytes += uart_send_string(&txBuffer, "$MACK,1*\n");
                    } 
                } else if (strcmp(ps.msg_type, "MISS") == 0) {
                    char msg[32];
                    format_msg_miss(msg, missed_rx_bytes, missed_tx_bytes);
                    missed_tx_bytes += uart_send_string(&txBuffer, msg);
                }
            }    
        }

        // Here we busy-wait for the ADC1 conversion to complete to make sure we have the new reading (adc1_read() is "blocking").
        // Nevertheless, we expect to find the conversion finished by the time we get here, because ...
        // ... sampling (2 us) and conversion (1.5 us) should be already completed in the background.
        // In fact, knowing that in our configuration 1TAD = 9TCY and we have 16TAD per sampling and 12TAD per conversion:
        // ==> sampling time = 16 * 9 * (1 / 72 MHz) = 2 us
        // ==> conversion time = 12 * 9 * (1 / 72 MHz) = 1.5 us
        // So by the time we reach this point, the new result should already be available without waiting.
        int ir_reading = adc1_read();
        ir_volt = adc1_process(ir_reading); 
        ir_cm = ir_compute_distance(ir_volt);

        if (ir_cm < DISTANCE_THRESHOLD) {
            // If we go under the threshold, change the state to EMERGENCY and send feedback on the UART (only once)
            if (STATE != EMERGENCY) {
                STATE = EMERGENCY;
                missed_tx_bytes += uart_send_string(&txBuffer, "$MEMRG,1*\n");
            }
            // We reset the counter EVERY time we go under the threshold
            count_emergency = 0;
        } else if (STATE == EMERGENCY && ++count_emergency >= 2500) { 
            // Resume from emergency when 5 seconds (2500 cycles) without going under the threshold have passed
            // count_emergency is only incremented if we’re already in EMERGENCY. (&& operator is short-circuiting).
            // This prevents count_emergency from growing indefinitely when the obstacle stays far away.
            STATE = WAIT_FOR_START;
            missed_tx_bytes += uart_send_string(&txBuffer, "$MEMRG,0*\n");
            LEDL = 0;                       // Reset LEDs
            LEDR = 0;
        }

        // Handle IR distance feedback on the UART at 10 Hz 
        if ((count + 2) % 50 == 0) {
            char msg[16];
            // We use our own function in place of sprintf because we were missing deadlines because of it
            format_msg_mdist(msg, ir_cm);
            missed_tx_bytes += uart_send_string(&txBuffer, msg);
        }

        // Handle acc feedback on the UART at 10 Hz
        if ((count + 1) % 50 == 0) {
            int raw_axes[3];
            acc_read_axes(raw_axes);
            char msg[32];
            // We use our own function in place of sprintf because we were missing deadlines because of it
            format_msg_macc(msg, raw_axes[0], raw_axes[1], raw_axes[2]);
            missed_tx_bytes += uart_send_string(&txBuffer, msg);
        }

        // Handle the feedback of the battery reading at 1 Hz (once every 500 cycles)
        if (count == 0) {  
            // Like for ADC1, adc2_read() does busy-waiting even though at this point the sampling and conversion should already be finished
            int battery_reading = adc2_read();                  // Read the battery voltage from ADC2
            latest_batt_v = 3 * adc2_process(battery_reading);  // Process the reading to get the voltage and multiply by 3 to account for the voltage divider
            char msg[16];
            // In this case we kept sprintf because it was too hard to replace it in the case of float/double ...
            // ... variables and we weren't missing deadlines anyway
            // Casting to double is necessary to avoid warning
            sprintf(msg, "$MBATT,%.2f*\n", (double)latest_batt_v);
            missed_tx_bytes += uart_send_string(&txBuffer, msg);
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
                // NOTE: The set_motor_speeds_no_deadzone() can be optionally used instead of set_motor_speeds() to avoid the deadzone under 40% duty cycle
                // set_motor_speeds_no_deadzone(0, 0);  
                LED2 = 0;               ////// DEBUG //////
                break;
            case MOVING:
                set_motor_speeds(linear_rate, yaw_rate);
                // set_motor_speeds_no_deadzone(linear_rate, yaw_rate);
                LED2 = 1;               ////// DEBUG //////
                break;
            case EMERGENCY:
                set_motor_speeds(0, 0);
                // set_motor_speeds_no_deadzone(0, 0);
                LED2 = 0;               ////// DEBUG //////
                break;
        }
        
        count = (count + 1) % 500;      // Reset count every 500 iterations to keep it in range [0, 499]

        // Wait until next period (LEDFRONT toggling is just for debugging purposes)
        if (tmr_wait_period(TIMER1) == 1) LEDFRONT = 1;              
        else LEDFRONT = 0;                       
    }
    return 0;
}
