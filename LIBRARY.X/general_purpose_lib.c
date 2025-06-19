#include <xc.h>
#include <string.h>
#include "config.h"
#include "general_purpose_lib.h"
#include "spi_lib.h"


int Buffer_Init(volatile CircularBuffer* cb, volatile char* buf_ptr, unsigned int size) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    cb->size = size;
    cb->buffer = buf_ptr;  
    return 0;
}

int Buffer_Write(volatile CircularBuffer* cb, char data) {
    // Writes one character into the circular buffer.
    // Returns 0 on success, or -1 if the buffer is full.
    if (cb->count == cb->size) {
        return -1;                  // Buffer is full.
    }
    
    cb->buffer[cb->tail] = data;
    cb->tail = (cb->tail + 1) % cb->size;
    cb->count++;
    return 0;
}

int Buffer_Read(volatile CircularBuffer* cb, char* data_ptr) {
    // Reads one character from the circular buffer into the location pointed to by data.
    // Returns 0 on success, or -1 if the buffer is empty.
    if (cb->count == 0) {
        return -1;                  // Buffer is empty.
    }
    
    *data_ptr = cb->buffer[cb->head];
    cb->head = (cb->head + 1) % cb->size;
    cb->count--;
    return 0;
}

// void DataBuffer_Init(DataBuffer* mb) {
//     mb->index = 0;
//     mb->count = 0;
//     for (int i = 0; i < BUF_AVG_SAMPLES; i++) {
//         mb->x[i] = mb->y[i] = mb->z[i] = 0;
//     }
// }

// void DataBuffer_Write(DataBuffer* mb, int x, int y, int z) {
//     mb->x[mb->index] = x;
//     mb->y[mb->index] = y;
//     mb->z[mb->index] = z;
//     mb->index = (mb->index + 1) % BUF_AVG_SAMPLES;
//     if(mb->count < BUF_AVG_SAMPLES)
//         mb->count++;
// }

// void DataBuffer_Average(DataBuffer* mb, int* avg_x, int* avg_y, int* avg_z) {
//     long sum_x = 0, sum_y = 0, sum_z = 0;
//     for (int i = 0; i < mb->count; i++) {
//         sum_x += mb->x[i];
//         sum_y += mb->y[i];
//         sum_z += mb->z[i];
//     }
//     if(mb->count > 0) {
//         *avg_x = (int)(sum_x / mb->count);
//         *avg_y = (int)(sum_y / mb->count);
//         *avg_z = (int)(sum_z / mb->count);
//     } else {
//         *avg_x = *avg_y = *avg_z = 0;
//     }
// }

void set_digital_mode(void) {
    // Sets all analog pins to digital mode
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0;
}

void set_interrupt_priorities(void) {
    IPC2bits.U1RXIP = 6;        // Set UART1 RX interrupt priority to 6
    IPC3bits.U1TXIP = 4;        // Set UART1 TX interrupt priority to 4 (default)
    IPC5bits.INT1IP = 5;        // Set INT1 interrupt priority to 5
    IPC1bits.T2IP = 5;          // Set TIMER2 interrupt priority to 5
}

void leds_init(void) {
    // Set output mode for the LED1/2 pins
    TRISAbits.TRISA0 = 0;
    TRISGbits.TRISG9 = 0;
    // Make sure leds are turned off
    LED1 = 0;
    LED2 = 0;
}

void lights_init(void) {
    // Set output mode
    TRISBbits.TRISB8 = 0;
    TRISFbits.TRISF1 = 0;
    TRISGbits.TRISG1 = 0;
    TRISAbits.TRISA7 = 0;
    // Make sure leds are turned off
    LEDL = 0;
    LEDR = 0;
    LEDBRAKE = 0;
    LEDFRONT = 0;
}

void button1_init(void) {
    // Set input mode for the buttons
    TRISEbits.TRISE8 = 1;   
    // Associate INT1 functionality to RE8 (RP88=0x58)
    RPINR0bits.INT1R = 0x58; 
}

void global_interrupt_enable(void) {
    INTCON2bits.GIE = 1;       
}

void ir_init() {
    // NOTE: we used Mikrobus 2 as Mikrobus 1 was broken on our car (number 8)
    TRISBbits.TRISB9 = 0;       // Set as output
    LATBbits.LATB9 = 1;         // Enable IR sensor
    ANSELBbits.ANSB14 = 1;      // set pin AN1 == RB14 in analog mode;
    TRISBbits.TRISB14 = 1;      // input
}

void battery_init() {
    ANSELBbits.ANSB11 = 1;      // set pin AN1 == RB14 in analog mode;
    TRISBbits.TRISB11 = 1;      // input
}

// void algorithm (void) {
//     // Simulates an algorithm which takes 7 ms to execute
//         tmr_wait_ms(TIMER2, 7);
// }

// void mag_sus2act(void) {
//     // This function switches the magnetometer from suspended to active mode
//     CS_MAG = 0;
//     spi_write_address(0x4B, 0x01);      // Switch from suspended to sleep mode
//     CS_MAG = 1;
    
//     tmr_wait_ms(TIMER3, 3);             // Wait 3 ms for the switch to take place
    
//     CS_MAG = 0;
//     spi_write_address(0x4C, 0b00110000);// Switch from sleep to active mode and set 25 Hz data rate
//     CS_MAG = 1;
// }

// void mag_read_axes(int* axes_ptr) { 
//     unsigned char raw_data[6]; // Buffer for raw data 
//     // Read raw data
//     CS_MAG = 0; 
//     spi_read_address(0x42, raw_data, 6);
//     CS_MAG = 1; 

//     axes_ptr[0] = (int)(((int)raw_data[1] << 8) | (raw_data[0] & 0b11111000)) / 8;
//     axes_ptr[1] = (int)(((int)raw_data[3] << 8) | (raw_data[2] & 0b11111000)) / 8;
//     axes_ptr[2] = (int)(((int)raw_data[5] << 8) | (raw_data[4] & 0b11111110)) / 2;
// }

// void mag_update_readings(DataBuffer* mb) {
//     // This function calls mag_read_axes(), and then adds the new readings into the averaging buffer.
//     int raw_axes[3];
//     // Read raw axes
//     mag_read_axes(raw_axes);  
//     // Update MagDataBuffer pointed to by mb with the new measurements
//     DataBuffer_Write(mb, raw_axes[0], raw_axes[1], raw_axes[2]);
// }

void acc_init(){
    // This function set the acc. sensor in filtered mode with 7.81 bandwidth (15.625 Hz update frequency)
    // This makes sure that the acc registers are updated faster than we read and send them on the uart (10 Hz)
    CS_ACC = 0;
    spi_write_address(0x10, 0b00001000);    // Set t_ut = 64 ms (15.625 Hz)
    CS_ACC = 1;
}

void acc_read_axes(int* axes_ptr) {
    unsigned char raw_data[6]; // Buffer for raw data 
    // Read raw data
    CS_ACC = 0; 
    spi_read_address(0x02, raw_data, 6);
    CS_ACC = 1; 
    // NB: The acc data is 12-bit, so we changed the mask for the LSB and we need to shift by 4 bits instead of 3 (division by 16 instead of 8)
    axes_ptr[0] = (int)(((int)raw_data[1] << 8) | (raw_data[0] & 0b11110000)) / 16;
    axes_ptr[1] = (int)(((int)raw_data[3] << 8) | (raw_data[2] & 0b11110000)) / 16;
    axes_ptr[2] = (int)(((int)raw_data[5] << 8) | (raw_data[4] & 0b11110000)) / 16;     
}

// void acc_update_readings(DataBuffer* mb) {
//     // This function calls acc_read_axes(), and then adds the new readings into the averaging buffer.
//     int raw_axes[3];
//     // Read raw axes
//     acc_read_axes(raw_axes);  
//     // Update MagDataBuffer pointed to by mb with the new measurements
//     DataBuffer_Write(mb, raw_axes[0], raw_axes[1], raw_axes[2]);
// }

int itoa(int value, char* buffer) {
    // This function, while very common, is not available as a standard library in C, so we implement it here.
    int i = 0;
    int is_negative = 0;
    unsigned int u_value;

    // Handle the case of 0
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return 1;
    }

    // Handle negative numbers by setting a flag and making the value positive
    if (value < 0) {
        is_negative = 1;
        // We use the unsigned u_value to avoid signed integer overflow when value is -32768
        u_value = -value;
    } else {
        u_value = value;
    }

    // Process individual digits. The digits are generated in reverse order.
    // For example, 123 becomes '3', '2', '1'.
    // Process digits
    while (u_value != 0) {
        int rem = u_value % 10;     // Modulo operation by 10 extracts the last digit
        buffer[i++] = rem + '0';    // Convert digit to ASCII character ('0' is 48 in ASCII) and store it in buffer
        u_value = u_value / 10;     // Integer division by zero removes the last digit
    }

    // If the number was negative, append the '-' sign to the end of the reversed string.
    if (is_negative) {
        buffer[i++] = '-';
    }

    // At this point the buffer contains the digits and sign in reverse order.
    int start_idx = 0;
    int end_idx = i - 1;
    char temp;
    while (start_idx < end_idx) {
        // Swap characters
        temp = buffer[start_idx];
        buffer[start_idx] = buffer[end_idx];
        buffer[end_idx] = temp;
        // Move pointers towards the middle
        start_idx++;
        end_idx--;
    }

    buffer[i] = '\0';               // Null-terminate the string    

    return i;                       // Return the length
}

void format_msg_macc(char* buffer, int x, int y, int z) {
    char* ptr = buffer;
    int len;

    // Append first part of the message
    strcpy(ptr, "$MACC,");
    ptr += 6;

    // Convert and append the axes values separated by commas
    len = itoa(x, ptr);
    ptr += len;
    *ptr++ = ',';

    len = itoa(y, ptr);
    ptr += len;
    *ptr++ = ',';

    len = itoa(z, ptr);
    ptr += len;

    // Append the end of the message
    strcpy(ptr, "*\n");
}

void format_msg_mdist(char* buffer, unsigned int distance) {
    char* ptr = buffer;

    // Append first part of the message
    strcpy(ptr, "$MDIST,");
    ptr += 7; 

    // Convert and append the distance value
    int len = itoa(distance, ptr);
    ptr += len;

    // Append the end of the message
    strcpy(ptr, "*\n");
}