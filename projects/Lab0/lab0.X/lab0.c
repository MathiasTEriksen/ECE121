/*====================\
    Mathias Eriksen    
    mterikse@ucsc.edu
    ECE121 
    Lab0                 
\=====================*/

#include "BOARD.h"
#include <stdlib.h>
#include <stdio.h>
#include <xc.h>

#define NOPS_FOR_5MS 3125 // ~6000 for one minute
#define Timer   // define for conditional compilation

void NOP_delay_5MS(){
    int i;
    for (i=0; i < NOPS_FOR_5MS ; i++) { 
        asm("nop"); 
    } 
}

int main(void) {
    
    TRISE = 0x00; // initialize LEDs
    
    TRISF = 0x02; // initialize buttons
    TRISD = 0xE0;
    
    LATE = 0x00; // set LEDs off
    
    #ifdef Buttons       
    
        while(1){

            if (PORTF & 0x02){  // bitmask for one button
                LATE = 0x03;    // set LED
            }

            if (PORTD & 0x20){
                LATE = 0x0C;
            }

            if (PORTD & 0x40){
                LATE = 0x30;
            }
            if (PORTD & 0x80){
                LATE = 0xC0;
            }
        }
    #endif

    #ifdef Timer

        uint16_t LEDbits = 0;
        int j;

        while(1){

            j++;
            if (j >= 50){   // count 50 times 5 ms delay for 250 ms toggle
               LEDbits++;
               j=0;
            }

            LATE = LEDbits;

            if (LEDbits > 0xFF){    // reset LEDs when they are all on
                LEDbits = 0x00;
            }

            if (PORTF & 0x02){  // check for button press for reset
                LEDbits = 0x00;
            }

            NOP_delay_5MS();    // run the no op for 5 ms
        } 
        
    #endif

}
