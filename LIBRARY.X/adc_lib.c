#include <xc.h>
#include <stdint.h>
#include <math.h>
#include "adc_lib.h"

//------------------------------------------------------------------------------
// ADC1: IR sensor su AN15, trigger = Timer3 @ 500 Hz
void adc1_init(void) {
    // 1) Abilita AN15
    
    // TRISBx = input automatico

    // 2) Config ADC1
    AD1CON1 = 0;
    AD1CON1bits.AD12B = 1;      // 12-bit
    AD1CON1bits.FORM  = 0;      // intero
    AD1CON1bits.SSRC  = 0b010;  // trigger da Timer3
    AD1CON1bits.ASAM  = 1;      // auto-sample

    AD1CON2 = 0;
    AD1CON2bits.CSCNA = 0;      // no scan
    AD1CON2bits.SMPI  = 0;      // interrupt ogni conversione
    AD1CON2bits.CHPS  = 0;      // solo CH0

    AD1CON3 = 0;
    AD1CON3bits.ADCS  = 8;      // Tad = 9 Tcy
    AD1CON3bits.SAMC  = 16;     // sample per 16 Tad

    // 3) Seleziona AN14
    AD1CHS0bits.CH0SA = 14;

    // 4) Interrupt
    //IPC3bits.AD1IP = 4;
    IFS0bits.AD1IF = 0;
    IEC0bits.AD1IE = 1;

    // 5) Accendi ADC1
    AD1CON1bits.ADON = 1;
}



//------------------------------------------------------------------------------
// ADC2: batteria su AN11, avvio manuale
void adc2_init(void) {
    // 1) Abilita AN11
    ANSELBbits.ANSB11 = 1;

    // 2) Config ADC2 per manual sample/auto-convert
    AD2CON1 = 0;
    // AD2CON1bits.AD12B = 1;      // 12-bit    --> doesn't work with ADC2 only 10-bit mode
    AD2CON1bits.FORM  = 0;      // intero
    AD2CON1bits.SSRC  = 7;  // auto-convert (dopo fine campionamento)
    AD2CON1bits.ASAM  = 0;      // campionamento manuale

    AD2CON2 = 0;
    AD2CON2bits.CSCNA = 0;      // no scan
    AD2CON2bits.SMPI  = 0;      // interrupt ogni conversione
    AD2CON2bits.CHPS  = 0;      // solo CH0

    AD2CON3 = 0;
    AD2CON3bits.ADCS  = 8;      // Tad = 9 Tcy
    AD2CON3bits.SAMC  = 16;     // sample per 16 Tad

    // 3) Seleziona AN11
    AD2CHS0bits.CH0SA = 11;

    // 4) Interrupt
    //IPC7bits.AD2IP = 3;
    IFS1bits.AD2IF = 0;
    IEC1bits.AD2IE = 1;

    // 5) Accendi ADC2 (ma non parte finché non setti SAMP=1)
    AD2CON1bits.ADON = 1;
}



