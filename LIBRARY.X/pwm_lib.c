#include <xc.h>
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



// //???????????????????????????????????????????????????????
// // DIFFERENTIAL-DRIVE MIXER
// // Reuses your oc?_set_duty() functions by passing them as arguments

// #define MIN_DUTY    40    // minimum percent that moves the motor
// #define MAX_DUTY   100

// // clamp x to [lo..hi]
// static inline int clamp(int x, int lo, int hi) {
//     if (x < lo) return lo;
//     if (x > hi) return hi;
//     return x;
// }

// // convert a speed command in -100..+100 to a duty cycle (0 or 40..100)
// static inline unsigned int cmd_to_duty(int cmd) {
//     if (cmd == 0) return 0;  // stop
//     return MIN_DUTY + (unsigned int)(abs(cmd) * (MAX_DUTY - MIN_DUTY)) / 100u;
// }

// // set one side of the buggy: cmd>0 -> forward, cmd<0 -> backward
// // oc_backward and oc_forward are your existing functions
// static void set_side(int cmd,
//                      void (*oc_backward)(unsigned int),
//                      void (*oc_forward )(unsigned int))
// {
//     unsigned int d = cmd_to_duty(cmd);
//     if (cmd > 0) {
//         oc_forward(d);      // calls e.g. oc2_set_duty(d)
//         oc_backward(0);     // calls e.g. oc1_set_duty(0)
//     } else if (cmd < 0) {
//         oc_forward(0);
//         oc_backward(d);
//     } else {
//         oc_forward(0);
//         oc_backward(0);
//     }
// }

// // MASTER MIXER FUNCTION
// // speed  ? [-100..+100]: positive -> forward
// // yawrate ? [-100..+100]: positive -> left turn (CCW)
// void drive_set(int speed, int yawrate)
// {
//     // compute differential commands
//     int left_cmd  = clamp(speed - yawrate, -100, 100);
//     int right_cmd = clamp(speed + yawrate, -100, 100);

//     // apply:
//     // left side uses OC1=backward, OC2=forward
//     // here we *reuse* your functions by passing them in:
//     set_side(left_cmd,
//              oc1_set_duty,  // backward
//              oc2_set_duty); // forward

//     // right side uses OC3=backward, OC4=forward
//     set_side(right_cmd,
//              oc3_set_duty,
//              oc4_set_duty);
// }






// OVUHEOFHOEIHCVNEV

void set_motor_speeds(int speed, int yawrate) {

    //const int MIN_DUTY_CYCLE = 40; // Minimum duty cycle for the motors

    int left_motor_power;
    int right_motor_power;

    // --- 1. Input Validation ---
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;
    if (yawrate > 100) yawrate = 100;
    if (yawrate < -100) yawrate = -100;

    // --- 2. Mixing Logic ---
    left_motor_power = speed - yawrate/2;
    right_motor_power = speed + yawrate/2;


    if (speed > 0) {
        if (left_motor_power > 100) left_motor_power = 100;
        if (left_motor_power < 0) left_motor_power = 1;   
        if (right_motor_power > 100) right_motor_power = 100;
        if (right_motor_power < 0) right_motor_power = 1;
    }
    else if (speed < 0) {
        if (left_motor_power < -100) left_motor_power = -100;
        if (left_motor_power > 0) left_motor_power = -1;   
        if (right_motor_power < -100) right_motor_power = -100;
        if (right_motor_power > 0) right_motor_power = -1;
    } else {
        left_motor_power = -yawrate;
        right_motor_power = yawrate;
    }
       

    // --- 3. Map Power to Duty Cycle & Determine Direction ---
    
    unsigned int oc1_duty = 0, oc2_duty = 0, oc3_duty = 0, oc4_duty = 0;
    unsigned int left_dc = 0, right_dc = 0;

    // --- Process Left Side ---
    if (left_motor_power != 0) {
        // Get the command magnitude (1 to 100)
        int abs_power = abs(left_motor_power);
        
        // Map power [1-100] to duty cycle [MIN_DUTY_CYCLE - 100]
        // This integer math formula does the linear mapping:
        // DC = start_of_range + (portion_of_input_range * output_range_size)
        left_dc = MIN_DUTY_CYCLE + (unsigned int)(abs_power * (100 - MIN_DUTY_CYCLE)) / 100;

        // Apply to correct pin based on direction
        if (left_motor_power > 0) {
            oc2_duty = left_dc; // Left Forward (+)
        } else {
            oc1_duty = left_dc; // Left Backward (-)
        }
    }

    // --- Process Right Side ---
    if (right_motor_power != 0) {
        // Get the command magnitude (1 to 100)
        int abs_power = abs(right_motor_power);
        
        // Map power [1-100] to duty cycle [MIN_DUTY_CYCLE - 100]
        right_dc = MIN_DUTY_CYCLE + (unsigned int)(abs_power * (100 - MIN_DUTY_CYCLE)) / 100;
        
        // Apply to correct pin based on direction
        if (right_motor_power > 0) {
            oc4_duty = right_dc; // Right Forward (+)
        } else {
            oc3_duty = right_dc; // Right Backward (-)
        }
    }

    // --- 5. Apply Final Duty Cycles to Peripherals ---
    oc1_set_duty(oc1_duty);
    oc2_set_duty(oc2_duty);
    oc3_set_duty(oc3_duty);
    oc4_set_duty(oc4_duty);

    return right_dc;
}


// void set_motor_speeds(int speed, int yawrate) {
//     int left_motor_power;
//     int right_motor_power;

//     // --- 1. Input Validation ---
//     if (speed > 100) speed = 100;
//     if (speed < -100) speed = -100;
//     if (yawrate > 100) yawrate = 100;
//     if (yawrate < -100) yawrate = -100;

//     // --- 2. Two-Mode Mixing Logic ---
//     if (speed == 0) {
//         // MODE 1: Rotate in place (Tank Turn)
//         // yawrate > 0 is CCW (Left), so right wheel fwd, left wheel bwd.
//         // yawrate < 0 is CW (Right), so left wheel fwd, right wheel bwd.
//         left_motor_power = -yawrate;
//         right_motor_power = yawrate;
//     } else {
//         // MODE 2: Arcing Turn (Consistent across forward/backward)
//         if (yawrate > 0) { // (CCW)
//             // Left wheel is inside, so it's slower.
//             left_motor_power = (speed * (100 - yawrate)) / 100;
//             // Right wheel is outside, so it gets full speed.
//             right_motor_power = speed;
//         } else if (yawrate < 0) { // (CW)
//             // Left wheel is outside, so it gets full speed.
//             left_motor_power = speed;
//             // Right wheel is inside, so it's slower. Use abs() for clarity.
//             right_motor_power = (speed * (100 - abs(yawrate))) / 100;
//         } else { // Moving straight (yawrate is 0)
//             left_motor_power = speed;
//             right_motor_power = speed;
//         }
//     }

//     // --- 3. Saturation / Clamping ---
//     // This section is now less critical for MODE 2 but still vital for MODE 1.
//     if (left_motor_power > 100) left_motor_power = 100;
//     if (left_motor_power < -100) left_motor_power = -100;
//     if (right_motor_power > 100) right_motor_power = 100;
//     if (right_motor_power < -100) right_motor_power = -100;

//     // --- 4. Map Power to Duty Cycle & Determine Direction ---
//     // This section remains the same as it correctly handles positive/negative power
//     unsigned int oc1_duty = 0, oc2_duty = 0, oc3_duty = 0, oc4_duty = 0;

//     // Process Left Side
//     if (left_motor_power != 0) {
//         int abs_power = abs(left_motor_power);
//         unsigned int left_dc = MIN_DUTY_CYCLE + (unsigned int)(abs_power * (100 - MIN_DUTY_CYCLE)) / 100;
//         if (left_motor_power > 0) oc2_duty = left_dc; // Left Forward (+)
//         else oc1_duty = left_dc; // Left Backward (-)
//     }

//     // Process Right Side
//     if (right_motor_power != 0) {
//         int abs_power = abs(right_motor_power);
//         unsigned int right_dc = MIN_DUTY_CYCLE + (unsigned int)(abs_power * (100 - MIN_DUTY_CYCLE)) / 100;
//         if (right_motor_power > 0) oc4_duty = right_dc; // Right Forward (+)
//         else oc3_duty = right_dc; // Right Backward (-)
//     }

//     // --- 5. Apply Final Duty Cycles to Peripherals ---
//     oc1_set_duty(oc1_duty);
//     oc2_set_duty(oc2_duty);
//     oc3_set_duty(oc3_duty);
//     oc4_set_duty(oc4_duty);
// }