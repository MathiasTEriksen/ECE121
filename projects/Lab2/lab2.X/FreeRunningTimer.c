/* 
 * File:   FreeRunningTimer.c
 * Author: Mathias Eriksen
 * Brief: 
 * Created on February 8 2023 12:08 pm
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

static unsigned int MilliSec = 0;
/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/

void FreeRunningTimer_Init(void){
    
    MilliSec = 0;
    
    TMR5 = 0;
    T5CON = 0x0;
    
    T5CONbits.TCKPS = 0b10;     // pre scalar of 4
    PR5 = ((PBCLK/1000)/4) - 1 ;    // calculate PR register
        
    IPC5bits.T5IP = 3;      // interrupt priority
    IPC5bits.T5IS = 0;
    IFS0bits.T5IF = 0;
    IEC0bits.T5IE = 1;      // enable interrupt
    T5CONbits.ON = 1;    
    
}

unsigned int FreeRunningTimer_GetMilliSeconds(void){
    
    return MilliSec;    // return current millisec count
    
}

unsigned int FreeRunningTimer_GetMicroSeconds(void){
    
    return (MilliSec*1000) + ((TMR5+1)/10); // use millisec count and micro from TMR
    
}


#ifdef TestHarness 


int main(){
    
    BOARD_Init();
    LEDS_INIT();
    Protocol_Init(BR);
    FreeRunningTimer_Init();
    
    unsigned char str[MAXPAYLOADLENGTH];
    sprintf(str, "Welcome to Mterikse's Lab2 FRT Test Harness. Compiled at %s %s\n",__DATE__, __TIME__);
    
    unsigned int lastval = MilliSec;
    
    Send_Packet(ID_DEBUG, strlen(str)-1 , str);
    
    while (1){
        
        ProcessPackets(str);
        
        if ((MilliSec - lastval) >= 1000){
            lastval = MilliSec;
            LEDS_SET(1 ^ LEDS_GET());
            sprintf(str, "The Running Timer is: %d milliseconds \n", lastval);
            Send_Packet(ID_DEBUG, strlen(str)-1 , str);
            sprintf(str, "The Running Timer is: %d microseconds \n",  FreeRunningTimer_GetMicroSeconds());
            Send_Packet(ID_DEBUG, strlen(str)-1 , str);
        }
    }    
}

#endif

void __ISR(_TIMER_5_VECTOR) Timer5IntHandler(void){
    
    IFS0bits.T5IF = 0;
    MilliSec++;         // iterate millisec counter
        
}