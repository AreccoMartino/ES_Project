#include <xc.h>
#include <stdlib.h>
#include <string.h>
#include "general_purpose_lib.h"
#include "timer_lib.h"
#include "spi_lib.h"
#include "pwm_lib.h"


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

void DataBuffer_Init(DataBuffer* mb) {
    mb->index = 0;
    mb->count = 0;
    for (int i = 0; i < BUF_AVG_SAMPLES; i++) {
        mb->x[i] = mb->y[i] = mb->z[i] = 0;
    }
}

void DataBuffer_Write(DataBuffer* mb, int x, int y, int z) {
    mb->x[mb->index] = x;
    mb->y[mb->index] = y;
    mb->z[mb->index] = z;
    mb->index = (mb->index + 1) % BUF_AVG_SAMPLES;
    if(mb->count < BUF_AVG_SAMPLES)
        mb->count++;
}

void DataBuffer_Average(DataBuffer* mb, int* avg_x, int* avg_y, int* avg_z) {
    long sum_x = 0, sum_y = 0, sum_z = 0;
    for (int i = 0; i < mb->count; i++) {
        sum_x += mb->x[i];
        sum_y += mb->y[i];
        sum_z += mb->z[i];
    }
    if(mb->count > 0) {
        *avg_x = (int)(sum_x / mb->count);
        *avg_y = (int)(sum_y / mb->count);
        *avg_z = (int)(sum_z / mb->count);
    } else {
        *avg_x = *avg_y = *avg_z = 0;
    }
}

void set_digital_mode(void) {
    // Sets all analog pins to digital mode
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0;
}

void leds_init(void) {
    // Set output mode for the LED1/2 pins
    TRISAbits.TRISA0 = 0;
    TRISGbits.TRISG9 = 0;
    // Make sure leds are turned off
    LED1 = 0;
    LED2 = 0;
}

void buttons_init(void) {
    // Set input mode for the buttons
    TRISEbits.TRISE8 = 1;
    TRISEbits.TRISE9 = 1;    
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

void global_interrupt_enable(void) {
    INTCON2bits.GIE = 1;       
}

void algorithm (void) {
    // Simulates an algorithm which takes 7 ms to execute
        tmr_wait_ms(TIMER2, 7);
}

void mag_sus2act(void) {
    // This function switches the magnetometer from suspended to active mode
    CS_MAG = 0;
    spi_write_address(0x4B, 0x01);      // Switch from suspended to sleep mode
    CS_MAG = 1;
    
    tmr_wait_ms(TIMER3, 3);             // Wait 3 ms for the switch to take place
    
    CS_MAG = 0;
    spi_write_address(0x4C, 0b00110000);// Switch from sleep to active mode and set 25 Hz data rate
    CS_MAG = 1;
}

void mag_read_axes(int* axes_ptr) { 
    unsigned char raw_data[6]; // Buffer for raw data 
    // Read raw data
    CS_MAG = 0; 
    spi_read_address(0x42, raw_data, 6);
    CS_MAG = 1; 

    axes_ptr[0] = (int)(((int)raw_data[1] << 8) | (raw_data[0] & 0b11111000)) / 8;
    axes_ptr[1] = (int)(((int)raw_data[3] << 8) | (raw_data[2] & 0b11111000)) / 8;
    axes_ptr[2] = (int)(((int)raw_data[5] << 8) | (raw_data[4] & 0b11111110)) / 2;
}

void mag_update_readings(DataBuffer* mb) {
    // This function calls mag_read_axes(), and then adds the new readings into the averaging buffer.
    int raw_axes[3];
    // Read raw axes
    mag_read_axes(raw_axes);  
    // Update MagDataBuffer pointed to by mb with the new measurements
    DataBuffer_Write(mb, raw_axes[0], raw_axes[1], raw_axes[2]);
}

void acc_init(){
    // This function set the acc. sensor in filtered mode with 31.25Hz bandwidth (a sample every 16ms)
    CS_ACC = 0;
    spi_write_address(0x10, 0b00001000);    // Set t_ut = 64 ms (15.625 Hz)
    // This makes sure that the acc registers are updated faster than we read and send them on the uart
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

void acc_update_readings(DataBuffer* mb) {
    // This function calls acc_read_axes(), and then adds the new readings into the averaging buffer.
    int raw_axes[3];
    // Read raw axes
    acc_read_axes(raw_axes);  
    // Update MagDataBuffer pointed to by mb with the new measurements
    DataBuffer_Write(mb, raw_axes[0], raw_axes[1], raw_axes[2]);
}

void ir_init() {
    TRISBbits.TRISB9 = 0;       // Set as output
    LATBbits.LATB9 = 1;         // Enable IR sensor
    ANSELBbits.ANSB14 = 1;      // set pin AN1 == RB14 in analog mode;
    TRISBbits.TRISB14 = 1;      // input
}

void battery_init() {
    ANSELBbits.ANSB11 = 1;      // set pin AN1 == RB14 in analog mode;
    TRISBbits.TRISB11 = 1;      // input
}

int itoa(int value, char* buffer) {
    int i = 0;
    int is_negative = 0;

    // Handle the special case of 0 explicitly
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return 1;
    }

    // Handle negative numbers by setting a flag and making the value positive
    if (value < 0) {
        is_negative = 1;
        // Be careful with INT_MIN, as -INT_MIN can overflow.
        // For most microcontrollers where int is 16-bit, this is -32768.
        // -(-32768) is 32768, which overflows a signed 16-bit int.
        // On a 32-bit system, this is less likely to be an issue.
        // A robust way is to work with unsigned, but for this context, simple negation is fine.
        value = -value;
    }

    // Process individual digits. The digits are generated in reverse order.
    // For example, 123 becomes '3', '2', '1'.
    while (value != 0) {
        int rem = value % 10;
        buffer[i++] = rem + '0';
        value = value / 10;
    }

    // If the number was negative, append the '-' sign to the end of the reversed string.
    if (is_negative) {
        buffer[i++] = '-';
    }

    // --- Inlined String Reversal Logic ---
    // At this point, the buffer contains the digits (and sign) in reverse order.
    // e.g., for -123, the buffer is ['3', '2', '1', '-'] and i = 4.
    // We now reverse the string in place.
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
    // --- End of Inlined Reversal ---

    // Null-terminate the now-correctly-ordered string
    buffer[i] = '\0'; 
    
    // Return the length of the string
    return i; 
}

void format_msg_macc(char* buffer, int x, int y, int z) {
    char* ptr = buffer;
    int len;

    // 1. Copy the prefix
    strcpy(ptr, "$MACC,");
    ptr += 6;

    // 2. Convert and append X
    len = itoa(x, ptr);
    ptr += len;
    *ptr++ = ',';

    // 3. Convert and append Y
    len = itoa(y, ptr);
    ptr += len;
    *ptr++ = ',';

    // 4. Convert and append Z
    len = itoa(z, ptr);
    ptr += len;

    // 5. Append the suffix and null terminator
    *ptr++ = '*';
    *ptr++ = '\n';
    *ptr = '\0';
}

void format_msg_mdist(char* buffer, unsigned int distance) {
    char* ptr = buffer;

    // 1. Copy the prefix
    strcpy(ptr, "$MDIST,");
    ptr += 7; // Move pointer past the prefix

    // 2. Convert the number and append it
    int len = itoa(distance, ptr);
    ptr += len; // Move pointer past the number

    // 3. Append the suffix
    strcpy(ptr, "*\n");
}