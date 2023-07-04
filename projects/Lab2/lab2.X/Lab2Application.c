/* 
 * File:   Lab2Application.c
 * Author: Mathias Eriksen
 * Brief: 
 * Created on March 1 2023 9:98 pm
 * Modified on <month> <day>, <year>, <hour> <pm/am>
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <sys/attribs.h>

#include "BOARD.h"
#include "Protocol2.h"
#include "uart.h"
#include "MessageIDs.h"

#include "FreeRunningTimer.h"
#include "RCServo.h"
#include "RotaryEncoder.h"
#include "PingSensor.h"
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define AppEnable

//#define Encoder_Max 32768
#define Encoder_Max 16384

#define Distance_Min 25
#define Distance_Max 125

#define Servo_Min 600
#define Servo_Max 2400
#define Servo_Diff 1800


/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/
void ReportAngle(unsigned int ServoPulse){
    
    int ServoAngle;
    int ServoPulseInt = ServoPulse;
    
    ServoAngle = ((120*(ServoPulseInt - Servo_Min)/Servo_Diff)) - 60;   // calc servo angle using pulse
    ServoAngle = 1000*ServoAngle;   // send angle in mdeg, so *1000
    int x = 0;
    unsigned char val[5];
        
    val[4]= 0b10;                            // send in range status bit
    val[0] = (ServoAngle >> 24) & 0xff;      
    val[1] = (ServoAngle >> 16) & 0xff;      // byte conversion
    val[2] = (ServoAngle >> 8) & 0xff; 
    val[3] = (ServoAngle >> 0 ) & 0xff;  
    
    Send_Packet(ID_LAB2_ANGLE_REPORT, 6, val);
    
}

void SetPulseEncoder(unsigned short angle){
        
    unsigned short DegAngle;
    unsigned int ServoPulse;
  
    DegAngle = (360*(angle))/(Encoder_Max);     // calc encoder angle from raw angle         
    
    ServoPulse = (DegAngle*5) + Servo_Min;      // convert to pulse width
    
    RCServo_SetPulse(ServoPulse);               // set servo pulse    
    ReportAngle(ServoPulse);                    // report angle
    
}

void SetPulseSensor(unsigned short distance){
        
    unsigned int ScaledPulse;
    
    if (distance > Distance_Max){               // set servo at max value if reading is too high
        RCServo_SetPulse(Servo_Max);
        ReportAngle(Servo_Max);
    } else if (distance < Distance_Min){        // set servo at min value if reading too low
        RCServo_SetPulse(Servo_Min);
        ReportAngle(Servo_Min);
    } else {                                    // if in range
        ScaledPulse = (18*(distance - Distance_Min)) + Servo_Min;   // calc servo value   
        RCServo_SetPulse(ScaledPulse);                              // set servo pulse
        ReportAngle(ScaledPulse);
    }   

}

#ifdef AppEnable
int main(void){
        
    BOARD_Init();
    Protocol_Init(BR);
    
    FreeRunningTimer_Init();
    RotaryEncoder_Init(1);
    PingSensor_Init(1);
    RCServo_Init();
    
    int lastval;
    unsigned short angle;
    unsigned short distance;
    unsigned short servopulse;
    
    while (1){
        
        ProcessPackets();        
        
        if ((FreeRunningTimer_GetMilliSeconds() - lastval) >= 20){
            lastval = FreeRunningTimer_GetMilliSeconds();
            
            if (IC3CONbits.ON != TRUE){    // Encoder Mode 
                angle = RotaryEncoder_ReadRawAngle();
                angle = (angle << 1) & 0x3fff ;
                SetPulseEncoder(angle);                 
            }
            
            else if (IC3CONbits.ON == TRUE){    // Sensor Mode
                distance = PingSensor_GetDistance();                
                SetPulseSensor(distance);
            }        
        }   
    }
}
#endif


