/* 
 * File:   uart.c
 * Author: Mathias Eriksen
 * Brief: 
 * Created on January 23 2023 11:58 pm
 * Modified on <month> <day>, <year>, <hour> <pm/am>
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/

#include "uart.h"           // The header file for this source file. 
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "BOARD.h"
#include <sys/attribs.h>

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

//#define CLK 80000000L     // Peripheral clock frequency, 40 MHz
//#define PBCLK CLK/2
//#define BR 115200         // Baud rate, python interface is 115.2 kb/s
#define BUFFER_SIZE 512     // Arbitrary buffer size of 512  bytes (must be power of 2)

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

typedef struct {
    unsigned char data[BUFFER_SIZE];        // array contains in/out bytes 
    int head;                               // tracks head of buffer
    int tail;                               // tracks tail of buffer
} cBuffer;

static cBuffer *RXBuffer;                   // static to maintain value between calls
static cBuffer *TXBuffer;                   // TX/RX buffers

static int TXFlag = 0;                      // Flag raises while TX buffer is being modified
static int TXCollision = 0;                 // Flag raises when TX buffer add was interrupted

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

cBuffer* Buffer_Init(void);
int Buffer_Full(cBuffer *Buffer);
int Buffer_Empty(cBuffer *Buffer);
int Buffer_Write(cBuffer *Buffer, unsigned char data);
int Buffer_Read(unsigned char *val, cBuffer *Buffer);
void Uart_Interrupt_Init(void);
void Uart_Loop(void);

/*******************************************************************************
 * Circular Buffer Functions                                                   *
 ******************************************************************************/

/**
 * @Function Buffer_Init(void)
 * @param NONE
 * @return NONE
 * @brief Initializes a buffer at given address, sets head/tail to 0
 * @note 
 * @author <Mathias Eriksen>*/
cBuffer* Buffer_Init(void){
    
    cBuffer *Buffer = malloc(sizeof(cBuffer));  // allocate mem to buffer
    Buffer->head = 0;       
    Buffer->tail = 0;
    
}

/**
 * @Function Buffer_Full(cBuffer *Buffer)
 * @param cBuffer *Buffer, a circular buffer
 * @return TRUE or FALSE
 * @brief Checks if a buffer is full, returns TRUE if full, FALSE if not full
 * @note Using this, don't allow write if write will fill buffer
 * @author <Mathias Eriksen>*/
int Buffer_Full(cBuffer *Buffer){
    
    if (((Buffer->head) - (Buffer->tail) > 0) && 
       ((((Buffer->head)+1) % BUFFER_SIZE) == (Buffer->tail))){ // Given full formula
        return TRUE;
    } else {
        return FALSE;
    }    
}

/**
 * @Function Buffer_Empty(cBuffer *Buffer)
 * @param cBuffer *Buffer, a circular buffer
 * @return TRUE or FALSE
 * @brief Checks if a buffer is empty, returns TRUE if empty, FALSE if not empty
 * @note 
 * @author <Mathias Eriksen>*/
int Buffer_Empty(cBuffer *Buffer){
    
    if (Buffer->head == Buffer->tail){      // empty if head == tail
        return TRUE;
    } else {
        return FALSE;
    }   
}

/**
 * @Function Buffer_Write(cBuffer *Buffer, unsigned char data)
 * @param cBuffer *Buffer, a circular buffer, data, data to write
 * @return TRUE or FALSE
 * @brief Writes a passed value to head of a circular buffer
 * @note Returns FALSE if write was unsuccessful, TRUE if successful
 * @author <Mathias Eriksen>*/
int Buffer_Write(cBuffer *Buffer, unsigned char data){
    
    if (Buffer_Full(Buffer)){
        return ERROR;
    } else {
       Buffer->data[Buffer->head] = data;                   // set value
       Buffer->head = ((Buffer->head + 1) % BUFFER_SIZE);   // mod for wrap
       return SUCCESS;    
    }
}

/**
 * @Function Buffer_Read(cBuffer *Buffer, unsigned char data)
 * @param cBuffer *Buffer, a circular buffer
 * @return FALSE or value from read at buffer tail
 * @brief Reads and returns a value from buffer tail
 * @note Returns FALSE if unsuccessful, value if successful
 * @author <Mathias Eriksen>*/
int Buffer_Read(unsigned char *val, cBuffer *Buffer){
    
    if (Buffer_Empty(Buffer)){
        return ERROR;                                       // return ERROR to distinguish from 0
    } else {
        *val = Buffer->data[Buffer->tail];                  // alter value at *val to be data at tail
        Buffer->tail = ((Buffer->tail + 1) % BUFFER_SIZE);  // increment tail
        return SUCCESS;
    }   
}

/*******************************************************************************
 * UART Initialization  Functions                                              *
 ******************************************************************************/

/**
 * @Function Uart_Interrupt_Init(void)
 * @param NONE
 * @return NONE
 * @brief Sets up interrupt register bits
 * @note Interrupt priority = 4, RX and TX  interrupts enabled 
 * @author <Mathias Eriksen>*/
void Uart_Interrupt_Init(void){
    
    IPC6bits.U1IP = 0x04;   // interrupt priority
    IPC6bits.U1IS = 0x00;   // interrupt sub priority
     
    IEC0bits.U1RXIE = 1;    // interrupt enable RX
    IEC0bits.U1TXIE = 1;    // interrupt enable TX
    
}

/**
 * @Function Uart_Loop(void)
 * @param NONE
 * @return NONE
 * @brief Sets up a UART test loop for part 0
 * @note NONE
 * @author <Mathias Eriksen>*/
void Uart_Loop(void){
    
    uint8_t val;

    if (U1STAbits.URXDA){
        val = U1RXREG;    
        U1TXREG = val;
    }
}

/**
 * @Function Uart_Init(unsigned long baudRate)
 * @param baudrate, baud rate to initialize UART for
 * @return NONE
 * @brief Sets up UART bits for 8-N-1, interrupts, and initializes buffers
 * @note Initializes RX and TX buffers
 * @author <Mathias Eriksen>*/
void Uart_Init(unsigned long baudRate){
    
    U1MODE = 0; // clear mode register
    U1STA = 0;  // clear STA register
    U1MODEbits.UARTEN = 1;  // enable UART 
    U1MODEbits.STSEL = 0;   // set stop bit to 1
    U1MODEbits.PDSEL = 0;   // set parity to No
    
    U1STAbits.URXEN = 1;    // enable RX
    U1STAbits.UTXEN = 1;    // enable TX
       
    U1STAbits.UTXISEL = 0x00;   // set TX interrupt selector bit
    U1STAbits.URXISEL = 0x00;   // set RX interrupt selector bit
    
    U1BRG = ((PBCLK/(16*baudRate))-1);    // calculate and assign baud rate
    
    RXBuffer = Buffer_Init();       // initialize buffers
    TXBuffer = Buffer_Init();
    
    Uart_Interrupt_Init();      // initialize interrupts 
}

/*******************************************************************************
 * GetChar and PutChar Functions                                               *
 ******************************************************************************/

/**
 * @Function PutChar(char ch)
 * @param char, char to put into TX buffer
 * @return TRUE or FALSE
 * @brief Puts a character (from app layer) into TXBuffer for subsequent sending
 * @note Checks for collisions with interrupt 
 * @author <Mathias Eriksen>*/
int PutChar(unsigned char ch){
    
    TXFlag = TRUE;              // raise flag to indicate that we are altering TXBuffer
    
    if (Buffer_Full(TXBuffer)){
        return ERROR;
    }
    Buffer_Write(TXBuffer, ch);
    TXFlag = FALSE;             // write  to TXBuffer and lower flag
    
    if (U1STAbits.TRMT){        // kick starts transmission interrupt 
        IFS0bits.U1TXIF = 1;    // enable flag if shift register is empty
        
    } else if (TXCollision == TRUE) {
        TXCollision = FALSE;
        IFS0bits.U1TXIF = 1;    // kick start to finish interrupted transmission
    }
    return SUCCESS;    
}

/**
 * @Function GetChar(void)
 * @param NONE
 * @return value from tail of of RX buffer
 * @brief Gets a char from tail of RX buffer
 * @note NONE
 * @author <Mathias Eriksen>*/
int GetChar(unsigned char *val){
    
    return Buffer_Read(val, RXBuffer);
                            // returns int ERROR code, alters value passed by reference
}

/**
 * @Function _mon_putc(char c)
 * @param c
 * @return NONE
 * @brief Prints value to stdout by overwriting putc
 * @note NONE
 * @author <Mathias Eriksen>*/
void _mon_putc(char c){
    
    PutChar(c);
    
}
/*******************************************************************************
 * Interrupt Service Routine for UART                                          *
 ******************************************************************************/

/**
 * @Function __ISR(_UART1_VECTOR) IntUart1Handler(void)
 * @param NONE
 * @return NONE
 * @brief Handles RX and TX interrupt  flags
 * @note Takes incoming chars and queues them in buffer, or transmits chars from
 *       TXBuffer
 * @author <Mathias Eriksen>*/
void __ISR(_UART1_VECTOR) IntUart1Handler(void){
    
    if (IFS0bits.U1RXIF){       
        IFS0bits.U1RXIF = 0;                // clear RX Flag        
        while (U1STAbits.URXDA){            // while data is available for read
            Buffer_Write(RXBuffer, U1RXREG);// write to  RXBuffer from REG
        }       
    }
   
    if (IFS0bits.U1TXIF) {
        IFS0bits.U1TXIF = 0;
        if (TXFlag == TRUE){                // check for collision w Putchar
            TXCollision = TRUE;             // raise collision flag if true
        } else if (TXFlag == FALSE) {        
            while (!Buffer_Empty(TXBuffer)&& !U1STAbits.UTXBF){
                unsigned char val;
                Buffer_Read(&val, TXBuffer);
                U1TXREG = val;
            }
        }
    }  
}
 