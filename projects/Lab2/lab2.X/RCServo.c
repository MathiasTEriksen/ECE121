/* 
 * File:   RCServo.c
 * Author: Mathias Eriksen
 * Brief: 
 * Created on February 23 2023 9:98 pm
 * Modified on <month> <day>, <year>, <hour> <pm/am>
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/

#include "Protocol2.h"
#include "FreeRunningTimer.h"
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "BOARD.h"
#include <sys/attribs.h>
#include "uart.h"
#include "MessageIDs.h"
#include "RCServo.h"
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
//#define TestHarness

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/

void Timer3_Init(void){
    
    TMR3 = 0;
    T3CON = 0x0;
    
    T3CONbits.TCKPS = 0b101;        // 32 pre scalar
    PR3 = ((PBCLK/50)/32) - 1 ;     // calculate for 50 Hz
        
    IPC3bits.T3IP = 3;
    IPC3bits.T3IS = 0;
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1;
    T3CONbits.ON = 1;  
    
}

int RCServo_Init(void){
    
    Timer3_Init();
    
    OC3CONbits.OCTSEL = 1;      // Timer3 is base         
    OC3CONbits.OCM = 0b110;     // PWM mode on OCx, Fault pin disabled
    OC3RS = PR3*(1.5/20);       // initialize pulse width for center of servo
    OC3R =  PR3*(1.5/20);       // initialize R register
    OC3CONbits.ON = 1;          // turn on output compare
    
}

int RCServo_SetPulse(unsigned int inPulse){
    
    OC3RS = (PR3*inPulse)/20000;    // set pulse using OC3 period and requested pulse width
    
}
/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

#ifdef TestHarness
int main(){
       
    BOARD_Init();
    Protocol_Init(BR);
    RCServo_Init();
    
    OC3RS = PR3*(1.5/20);
    
    while(1){
        
        ProcessPackets();
        
    }
    
}
#endif

void __ISR(_TIMER_3_VECTOR) Timer3IntHandler(void){
    
    IFS0bits.T3IF = 0;
        
}