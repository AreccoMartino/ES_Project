#include <xc.h>
#include "adc_lib.h"


void adc1_init(void) {
    AD1CON1bits.AD12B = 1;      // 12-bit
    AD1CON1bits.FORM  = 0;      // Integer format
    AD1CON1bits.SSRC  = 7;      // Conversion starts right after the end of sampling
    AD1CON1bits.ASAM  = 0;      // Manual sampling 
    AD1CON2bits.CSCNA = 0;      // No scan
    AD1CON2bits.CHPS  = 0;      // Only CH0
    AD1CON3bits.ADCS  = 8;      // 1 TAD = (ADCS + 1) * Tcy = 9 Tcy
    AD1CON3bits.SAMC  = 16;     // Sample time = 16 Tad (conversion time is fixed to 12 TAD)
    AD1CHS0bits.CH0SA = 14;     // Select AN14 (RB14) as input
    AD1CON1bits.ADON = 1;       // Turn on ADC1
}

void adc2_init(void) {
    // We don't set the 12 bit mode because it's not available for the ADC2, anyway for our purpose 10 bits should be enough.
    AD2CON1bits.FORM  = 0;      // Integer format
    AD2CON1bits.SSRC  = 7;      // Conversion starts right after the end of sampling
    AD2CON1bits.ASAM  = 0;      // Manual sampling 
    AD2CON2bits.CSCNA = 0;      // No scan
    AD2CON2bits.CHPS  = 0;      // Only CH0
    AD2CON3bits.ADCS  = 8;      // 1 TAD = (ADCS + 1) * Tcy = 9 Tcy
    AD2CON3bits.SAMC  = 16;     // Sample time = 16 Tad (conversion time is fixed to 12 TAD)
    AD2CHS0bits.CH0SA = 11;     // Select AN11 (RB11) as input
    AD2CON1bits.ADON = 1;       // Turn on ADC2
}

int adc1_read(void) {
    // This function reads the ADC1 value from channel 0
    while (!AD1CON1bits.DONE);   // Wait for conversion to complete
    int result = ADC1BUF0;       // Read the result
    AD1CON1bits.DONE = 0;        // Clear the DONE bit manually for safety
    return result;
}

int adc2_read(void) {
    // This function reads the ADC2 value from channel 0
    while (!AD2CON1bits.DONE);   // Wait for conversion to complete
    int result = ADC2BUF0;       // Read the result
    AD2CON1bits.DONE = 0;        // Clear the DONE bit manually for safety
    return result;
}

float adc1_process(int reading) {
    // This function processes the ADC1 reading (12 bit) and returns the voltage
    // The ADC1 is configured to read from AN14 (RB14), which is the IR sensor
    return (float)reading / 4095.0f * 3.3f;     // Convert to voltage (0-4095 maps to 0-3.3V)
}

float adc2_process(int reading) {
    // This function processes the ADC2 reading (10 bit) and returns the voltage
    // The ADC2 is configured to read from AN11 (RB11), which is the battery voltage
    return (float)reading / 1023.0f * 3.3f;     // Convert to voltage (0-1023 maps to 0-3.3V)
}