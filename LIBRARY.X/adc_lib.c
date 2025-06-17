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
    // AD2CON1bits.AD12B = 1;      // 12-bit    --> doesn't work with ADC2, we have only 10-bit mode
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