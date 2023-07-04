/* 
 * File:   Protocol.h
 * Author: Petersen
 * rewritten for Uart.c ECE121 W2023
 */


#ifndef PROTOCOL_H
#define	PROTOCOL_H

/*******************************************************************************
 * PUBLIC #DEFINES                                                            *
 ******************************************************************************/

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "BOARD.h"
#include <sys/attribs.h>

#define PACKETBUFFERSIZE 8
#define MAXPAYLOADLENGTH 128 
#define DEBUG 0 

/*******************************************************************************
 * PUBLIC DATATYPES
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/
unsigned char Protocol_CalcIterativeChecksum(unsigned char charIn, unsigned char curChecksum);

void Protocol_Init(unsigned long baudrate);

void ProcessPackets();

void Send_Debug(unsigned char *str);

int Send_Packet(unsigned char ID, unsigned char length, unsigned char *payload);

void Convert_to_char(unsigned int val, unsigned char *c);
/**
 * This macro initializes all LEDs for use. It enables the proper pins as outputs and also turns all
 * LEDs off.
 */
#define LEDS_INIT() do {LATECLR = 0xFF; TRISECLR = 0xFF;} while (0)

/**
 * Provides a way to quickly get the status of all 8 LEDs into a uint8, where a bit is 1 if the LED
 * is on and 0 if it's not. The LEDs are ordered such that bit 7 is LED8 and bit 0 is LED0.
     */
#define LEDS_GET() (LATE & 0xFF)

/**
 * This macro sets the LEDs on according to which bits are high in the argument. Bit 0 corresponds
 * to LED0.
 * @param leds Set the LEDs to this value where 1 means on and 0 means off.
 */
#define LEDS_SET(leds) do { LATE = (leds); } while (0)
#endif	/* PROTOCOL_H */

