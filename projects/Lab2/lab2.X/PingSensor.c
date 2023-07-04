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
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
//#define TestHarness

#define REG_MAX 65536

#define Trigger LATDbits.LATD0

static unsigned short t1;
static unsigned short cm;

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

void Timer4_Init(void){
    
    TRISDbits.TRISD0 = 0;   // Trigger
    
    TMR4 = 0;
    T4CON = 0x0;
    
    T4CONbits.TCKPS = 0b100;        // 16 pre scalar
    PR4 = ((PBCLK/60)/16) - 1 ;     // calculate for 60 Hz
        
    IPC4bits.T4IP = 3;
    IPC4bits.T4IS = 0;
    IFS0bits.T4IF = 0;
    IEC0bits.T4IE = 1;
    T4CONbits.ON = 1;  
    
}

void Timer2_Init(void){
    
    TMR2 = 0;
    T2CON = 0x0;
    
    T2CONbits.TCKPS = 0b011;        // 8 pre scalar
    PR2 = ((PBCLK/80)/8) - 1 ;      // calculate for 80 Hz
        
    IPC2bits.T2IP = 3;
    IPC2bits.T2IS = 0;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    T2CONbits.ON = 1;  
    
}

void Input_Capture3_Init(void){
    
    TRISDbits.TRISD10 = 1; // Input pin
    
    IFS0bits.IC3IF = 0;
    
    IPC3bits.IC3IP = 4;
    IPC3bits.IC3IS = 0;
    IEC0bits.IC3IE = 1;
    
    IC3CONbits.ON = 0;
    IC3CON = 0;
    IC3CONbits.ICTMR = 1;       // use Timer2 as base
    IC3CONbits.ICM = 0b001;     // set mode to trigger on rising and falling edges
    IC3CONbits.ON = 1;  
    
}

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/

int PingSensor_Init(char interfaceMode){
    
    if (!interfaceMode){
        Timer4_Init();
        Timer2_Init();
        Input_Capture3_Init();  
        return SUCCESS;
    } else {
        IC3CONbits.ON = 0;  // disable interrupts when not in use
        T2CONbits.ON = 0; 
        T4CONbits.ON = 0; 
        return ERROR;
    }    
}

unsigned short PingSensor_GetDistance(void){
    
    return cm;
    
}

#ifdef TestHarness
int main(){
    
    BOARD_Init();
    PingSensor_Init(0);
    FreeRunningTimer_Init();
    Protocol_Init(BR);
    
    unsigned short distance;
    unsigned int lastval;
    unsigned char val[2];
  
    while (1){
        
        if ((FreeRunningTimer_GetMilliSeconds() - lastval) >= 100){
            lastval = FreeRunningTimer_GetMilliSeconds();
            distance = PingSensor_GetDistance();

            val[1] = distance & 0xff;
            val[0] = (distance>>8) & 0xff;

            Send_Packet(ID_PING_DISTANCE, 3, val);        
        }
    }
}
#endif

void __ISR(_TIMER_4_VECTOR) Timer4IntHandler(void){
    
    IFS0bits.T4IF = 0;
        
    while (TMR4 < 45){  
        Trigger = 1;        // keep trigger pin high for >10 µs
    }    
    
    Trigger = 0;            // return trigger low
}

void __ISR(_INPUT_CAPTURE_3_VECTOR) __IC3Interrupt(void){
    
    unsigned short t2;
    
    IFS0bits.IC3IF = 0;
    
    if (PORTDbits.RD10){                        // if rising edge
        t1 = (IC3BUF & 0x0000FFFF) % (REG_MAX); // mask top 16 bits, save time 
    } else if (!PORTDbits.RD10){                // if falling edge
        t2 = (IC3BUF & 0x0000FFFF) % (REG_MAX) ;              
        cm = ((abs(t2-t1))*0.2*10)/58;          // calculate distance in mm using pulse width
    }
}

void __ISR(_TIMER_2_VECTOR) Timer2IntHandler(void){
    
    IFS0bits.T2IF = 0;
        
}