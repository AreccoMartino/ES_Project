#include <xc.h>
#include <stdlib.h>             // For abs()
#include "pwm_lib.h"


void oc_init(void) {
    OC1CON1bits.OCTSEL = 7;     // Use internal clock (FCY))
    OC2CON1bits.OCTSEL = 7;
    OC3CON1bits.OCTSEL = 7;
    OC4CON1bits.OCTSEL = 7;
    OC1CON2bits.SYNCSEL = 0x1F;
    OC2CON2bits.SYNCSEL = 0x1F;   
    OC3CON2bits.SYNCSEL = 0x1F;  
    OC4CON2bits.SYNCSEL = 0x1F;  
    OC1RS = PWM_PERIOD_TICKS;         // Set 10 kHz pwm period
    OC2RS = PWM_PERIOD_TICKS; 
    OC3RS = PWM_PERIOD_TICKS; 
    OC4RS = PWM_PERIOD_TICKS; 
    // The prescaler is left at the default 1:1 setting
}

void oc_start(void) {
    OC1CON1bits.OCM = 0b110;        // Edge-aligned mode
    OC2CON1bits.OCM = 0b110;
    OC3CON1bits.OCM = 0b110;
    OC4CON1bits.OCM = 0b110;
}

void oc_stop(void) {
    OC1CON1bits.OCM = 0;
    OC2CON1bits.OCM = 0;
    OC3CON1bits.OCM = 0;
    OC4CON1bits.OCM = 0;
}

void oc1_set_duty(unsigned int duty_percent) { 
    OC1R  = (PWM_PERIOD_TICKS * duty_percent) / 100;  
}

void oc2_set_duty(unsigned int duty_percent) {  
    OC2R  = (PWM_PERIOD_TICKS * duty_percent) / 100;
}

void oc3_set_duty(unsigned int duty_percent) {  
    OC3R  = (PWM_PERIOD_TICKS * duty_percent) / 100;
}

void oc4_set_duty(unsigned int duty_percent) {  
    OC4R  = (PWM_PERIOD_TICKS * duty_percent) / 100;
}

void oc_pins_init(void) {
    // RD1
    RPOR0bits.RP65R = 0b010000;     // Remap
    TRISDbits.TRISD1 = 0;           // Set as output

    // RD2
    RPOR1bits.RP66R = 0b010001;
    TRISDbits.TRISD2 = 0;

    // RD3
    RPOR1bits.RP67R = 0b010010;
    TRISDbits.TRISD3 = 0;

    // RD4
    RPOR2bits.RP68R = 0b010011;
    TRISDbits.TRISD4 = 0;
}

void set_motor_speeds(int speed, int yawrate) {

    int l_power, r_power;
    unsigned int l_duty, r_duty;

    if (speed == 0 && yawrate == 0) {
        // If both speed and yawrate are 0, we directly set the duty cycles without wasting time in useless computations
        oc1_set_duty(0);
        oc2_set_duty(0);
        oc3_set_duty(0);
        oc4_set_duty(0);
        return;
    }

    // Check that user input is valid [-100, 100]
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;
    if (yawrate > 100) yawrate = 100;
    if (yawrate < -100) yawrate = -100;

    // Mix the speed and yawrate to compute the power for each side
    l_power = speed - yawrate;
    r_power = speed + yawrate;

    // l and r absolute powers are at most 200 so we can safely cast to unsigned:
    unsigned int abs_l_power = (unsigned int) abs(l_power);     
    unsigned int abs_r_power = (unsigned int) abs(r_power);
    unsigned int max_abs_power = abs_l_power > abs_r_power ? abs_l_power : abs_r_power;

    if (max_abs_power > 100) {      // Normalize the power values to be within [-100, 100]
        l_duty = (abs_l_power * 100) / max_abs_power;
        r_duty = (abs_r_power * 100) / max_abs_power;
    } else {
        l_duty = abs_l_power;
        r_duty = abs_r_power;
    }

    // left_dc = MIN_DUTY_CYCLE + (unsigned int)(abs_power * (100 - MIN_DUTY_CYCLE)) / 100;
    // right_dc = MIN_DUTY_CYCLE + (unsigned int)(abs_power * (100 - MIN_DUTY_CYCLE)) / 100;

    // Apply duty cycles for the left side
    if (l_power > 0) {
        oc2_set_duty(l_duty);      // Left Forward (+)
        oc1_set_duty(0);
    } else {
        oc1_set_duty(l_duty);      // Left Backward (-)   
        oc2_set_duty(0);  
    }
    // Apply duty cycles for the right side
    if (r_power > 0) {
        oc4_set_duty(r_duty);      // Right Forward (+)
        oc3_set_duty(0);
    } else {
        oc3_set_duty(r_duty);      // Right Backward (-)   
        oc4_set_duty(0);   
    }
}