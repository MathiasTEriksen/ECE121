/* 
 * File:   RotaryEncoder.c
 * Author: Mathias Eriksen
 * Brief: 
 * Created on February 10 2023 9:98 pm
 * Modified on <month> <day>, <year>, <hour> <pm/am>
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include "RotaryEncoder.h"
#include "Protocol2.h"
#include "uart.h"
#include "MessageIDs.h"
#include "BOARD.h"
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <sys/attribs.h>
#include <stdint.h>
#include "FreeRunningTimer.h"
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

//#define TestHarness

#define HEAD 204
#define TAIL 185
#define BACKSLASHR 13
#define BACKSLASHN 10
/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/

int RotaryEncoder_Init(char interfaceMode){
    
    // SPI Setup
    
    // Read from SPI1RXB
    
    // Initialize TRISx on I/0 shield
    
    // clock rates up to 10 Mhz only, cant use PBCLK
    
    if (interfaceMode){
        
        SPI1CONbits.ON = 0;
        SPI1CONbits.MODE16 = 1; // 16 bit register from encoder
                                // 15 - parity, 14 - 0, 13:0 -> data
        SPI1CONbits.MODE32 = 0;
        SPI1CONbits.MSTEN = 1;  // master mode
        SPI1CONbits.CKP = 1;
        SPI1CONbits.CKE = 0;

        SPI1BRG = 15; // 1.25 MHz
        
        TRISEbits.TRISE5 = 0;   // SCn
        TRISFbits.TRISF2 = 1;   // SDI
        TRISFbits.TRISF3 = 0;   // SDO
        TRISFbits.TRISF6 = 0;   // SCK
        
        SPI1CONbits.ON = 1;     // turn on SPI
        return SUCCESS;
        
    } else {
        SPI1CONbits.ON = 0;     // turn off SPI when not in use
        return ERROR;
    } 
}


unsigned short RotaryEncoder_ReadRawAngle(void){
    
    unsigned short readval;
    
    LATEbits.LATE5 = 0;     // active low, so start data transaction
    SPI1BUF = 0xFFFF;       // read command to raw data address
    while(!SPI1STATbits.SPIRBF);    // wait till data has been received in SPIRBF, has to shift out add, shift in data
    readval = SPI1BUF;      // read the data that has been shifted in 
    LATEbits.LATE5 = 1;     // stop data transaction

    return readval;         // return the angle reading
        
}

void Send_RawAngle(unsigned short angle){
    unsigned char val[2];
    unsigned char len = 3;
    unsigned char ID = ID_ROTARY_ANGLE;
        
    if (angle & 0x4000){        // if we have an error bit
        return;                 // return
    } else {
        
        val[1] = angle<<1 & 0xff;       // convert to byte
        val[0] = (angle>>7) & 0x3f;     // masking error/parity and shifting by one to display angle correctly

        Send_Packet(ID, len, val);      // send out angle reading
    }
}

#ifdef TestHarness

int main(){
    
    unsigned short data;
    unsigned short val;
    int lastval;
    
    BOARD_Init();
    Protocol_Init(BR);
    FreeRunningTimer_Init();
    RotaryEncoder_Init(1);
    
    unsigned char str[MAXPAYLOADLENGTH];
    sprintf(str, "Welcome to Mterikse's Lab2 Rotary Encoder Test Harness. Compiled at %s %s\n",__DATE__, __TIME__);
    
    Send_Packet(ID_DEBUG, strlen(str)-1 , str);
    
    while (1){
        
        data = RotaryEncoder_ReadRawAngle();
        int x =0;
        if ((FreeRunningTimer_GetMilliSeconds() - lastval) >= 10){
            lastval = FreeRunningTimer_GetMilliSeconds();
            Send_RawAngle(data);
        }   
    } 
}

#endif