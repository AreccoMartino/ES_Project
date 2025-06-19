/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CONFIG_H
#define	CONFIG_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// GENERAL
#define FCY 72000000UL      // Instruction cycle frequency
#define MAX_DELAY 233       // Max int value to not overflow 16-bit timer register
#define DISTANCE_THRESHOLD 20

// STATES
#define WAIT_FOR_START 0
#define MOVING 1
#define EMERGENCY 2

// TIMERS
#define TIMER1 1
#define TIMER2 2
#define TIMER3 3
#define TIMER4 4
#define TIMER5 5
#define TIMER6 6
#define TIMER7 7
#define TIMER8 8
#define TIMER9 9

// UART
#define UART_BAUD_RATE 9600
// Periodic messages:
// - $MACC,x,y,z*\n = 28 bytes every 100 ms             // x,y,z are signed ints (6 characters at most)
// - $MDIST,distance*\n = 12 bytes every 100 ms         // the biggest possible value of distance is 234, so in the worst case 3 characters are sufficient
// - $MBATT,v_batt*\n = 13 bytes every 1000 ms          // v_batt is always 4 bytes (X.YZ)  
// Sporadic messages:
// - $MEMRG,1*\n = 10 bytes with a minimum inter-arrival time of 5 s   
// - $MEMRG,0*\n = 10 bytes with a minimum inter-arrival time of 5 s      
// - $MACK,0*\n / $MACK,1*\n = 9 bytes triggered as a response to $PCSTT* or $PCSTP* commands. If the user where ...
// ... to send those commands as fast as possible we would get a new message every 7 ms, but this is unrealistic and ...
// ... would raise a lot the needed baud rate. Therefore, we have assumed that the PC sends those commands every 100 ms.
// Average production rate: 28*10 + 12*10 + 13*1 + 10/5 + 10/5 + 9*10 = 507 bytes/s 
// Consumption rate at 9600 bps: about 960 bytes/s, so the baud rate is sufficient.
#define RX_BUFFER_SIZE 4
// Because at 9600 bps we need about 1 ms to receive 1 byte, and considering that ...
// the main loop empties the buffer every 2 ms, in the worst case up to 2 bytes ...
// ... could accumulate. A 4 byte rxBuffer should be sufficient and not overflow
#define TX_BUFFER_SIZE 128
// The periodic messages are scheduled with offsets, spreading the load between cycles. However, to ...
// ... size the buffer properly, we analyze a conservative worst-case scenario where all the ... 
// ... message types (both periodic and sporadic) are added to the buffer within a single cycle.
// We assume one instance of each periodic message, one MACK message, and both MEMRG messages, as an ... 
// ... emergency may end and be immediately re-triggered.
// In this scenario we would have: 28 + 12 + 13 + 9 + 10 + 10 = 82 bytes.
// No new messages will be queued before the next 100 ms, and at 960 byte/s we are guaranteed to have an ... 
// ... empty buffer by then. Therefore, we size the buffer for the maximum burst of 82 bytes, ... 
// ... choosing 128 bytes to be conservative and prevent overflow, assuming we don't exceed the 1 MACK message per 100 ms hypothesis.

//SPI
#define CS_ACC LATBbits.LATB3            
#define CS_GYR LATBbits.LATB4
#define CS_MAG LATDbits.LATD6

// LEDS
#define LED1 LATAbits.LATA0
#define LED2 LATGbits.LATG9

// BUTTONS
#define BUTTON1 PORTEbits.RE8
#define BUTTON2 PORTEbits.RE9

// LIGHTS
#define LEDL LATBbits.LATB8 
#define LEDR LATFbits.LATF1
#define LEDBRAKE LATGbits.LATG1
#define LEDFRONT LATAbits.LATA7

// // ACCELEROMETER
// #define BUF_AVG_SAMPLES 5

// PWM OC
#define PWM_FREQ 10000UL
#define PWM_PERIOD_TICKS (FCY/PWM_FREQ)
#define MIN_DUTY_CYCLE 40  
#define PWM_SPAN (100 - MIN_DUTY_CYCLE)


// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* CONFIG_H */

