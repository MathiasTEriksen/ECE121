/***
 * File:    uart.h
 * Author:  Instructor
 * Created: ECE121 W2022 rev 0
 * This library implements a true UART device driver that enforces 
 * I/O stream abstraction between the physical and application layers.
 * All stream accesses are on a per-character basis. 
 */
#ifndef UART_H
#define UART_H

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "BOARD.h"
#include <sys/attribs.h>

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/

#define CLK 80000000L       // Peripheral clock frequency, 40 MHz
#define PBCLK CLK/2
#define BR 115200           // Baudrate, python interface is 115.2 kb/s

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

void Uart_Init(unsigned long baudRate);

int PutChar(unsigned char ch);

int GetChar(unsigned char *val);

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

void _mon_putc(char c);	


#endif // UART_H

