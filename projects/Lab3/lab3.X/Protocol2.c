/* 
 * File:   Protocol2.c
 * Author: Mathias Eriksen
 * Brief: 
 * Created on February 1 2023 3:28 pm
 * Modified on <month> <day>, <year>, <hour> <pm/am>
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/

#include "Protocol2.h" 
#include "uart.h"
#include "MessageIDs.h"
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "BOARD.h"
#include <sys/attribs.h>

//#include "RCServo.h"
//#include "PingSensor.h"
//#include "RotaryEncoder.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

#define PACKET_BUFF_SIZE 8
#define BR 115200

#define HEAD 204
#define TAIL 185
#define BACKSLASHR 13
#define BACKSLASHN 10

#define ID_LEDS_GET_LEN 2
#define UINT_SIZE 4

//#define TestHarness
/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

typedef struct {
    uint8_t ID;      
    uint8_t len;
    uint8_t checkSum;
    unsigned char payLoad[MAXPAYLOADLENGTH];
} rxpADT;

typedef enum {
    WAITING_FOR_HEAD,
    HEAD_RECEIVED,
    RECORD_PAYLOAD,
    GET_CHECKSUM,
} BuildStates;

typedef enum {
    SEND_HEAD,
    SEND_LEN,
    SEND_ID,
    SEND_PAYLOAD,
    SEND_TAIL,
    SEND_CHECKSUM,
    SEND_R,
    SEND_N,
} SendStates;

static BuildStates BuildState = WAITING_FOR_HEAD;
static SendStates SendState = SEND_HEAD;

static unsigned char Payload_Count = 0;
static unsigned char Check_ID = 0 ;
static unsigned char Checksum = 0 ;

static rxpADT PacketBuffer[PACKET_BUFF_SIZE];
static rxpADT CurrentPacket;
static rxpADT OutPacket;

static unsigned char PBhead;
static unsigned char PBtail;

static int PacketReady  = 0;
static int PacketSending = FALSE;

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

// Helper Functions 
unsigned char Protocol_CalcIterativeChecksum(unsigned char charIn, unsigned char curChecksum);
unsigned int Convert_to_int(unsigned char *c);

// Packet Handling Functions
uint8_t BuildRxPacket (unsigned char inchar);
uint8_t Protocol_QueuePacket (void);
int ReadPacket(rxpADT *ADT);
void PacketHandling(void);
void SendPacket(unsigned char *outchar);

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/

/**
 * @Function Protocol_Init(void)
 * @param baudrate
 * @return NONE
 * @brief Initializes the device driver by calling UART_Init
 * @note 
 * @author <Mathias Eriksen>*/
void Protocol_Init(unsigned long baudrate){
    
    Uart_Init(baudrate);                // initialize uart and associated interrupts
    
    PBhead = 0;                         // circular packet buffer head/tail initialization
    PBtail = 0;
    
    BuildState = WAITING_FOR_HEAD;      // initialize statics
    SendState = SEND_HEAD;   
    Payload_Count = 0;
    Check_ID = 0 ;
    Checksum = 0 ;  
    PacketReady  = 0;
    PacketSending = FALSE;
    
}

/**
 * @Function Protocol_CalcIterativeChecksum(unsigned char charIn, unsigned char curChecksum)
 * @param charIn, curChecksum
 * @return Iterated checksum
 * @brief Iterates a checksum value one chat at a time
 * @note 
 * @author <Mathias Eriksen>*/
unsigned char Protocol_CalcIterativeChecksum(unsigned char charIn, unsigned char curChecksum){
    
    curChecksum = (curChecksum >> 1) + (curChecksum << 7);
    curChecksum += charIn;
    return curChecksum;             // uses given pseudo code to implement iterative checksum
    
}

/**
 * @Function BuildRxPacket (unsigned char inchar)
 * @param inchar
 * @return NONE
 * @brief Implements a state machine to build a packet one char per call
 * @note Packet is built in the static CurrentPacket
 * @author <Mathias Eriksen>*/
uint8_t BuildRxPacket (unsigned char inchar){
                        
    switch (BuildState){                            // Builds packet items based on state of build
        
        case (WAITING_FOR_HEAD):
            if (inchar == HEAD){                    
                BuildState = HEAD_RECEIVED;
            }
            break;
        
        case (HEAD_RECEIVED):
            CurrentPacket.len = inchar;             // length includes ID              
            BuildState = RECORD_PAYLOAD;            // build in Current Packet static
            break;
            
        case (RECORD_PAYLOAD):
                          
            if (Check_ID == 0){
                CurrentPacket.ID = inchar;          // record ID
                Check_ID++;                         // ID check variable
                Checksum = Protocol_CalcIterativeChecksum(inchar, Checksum);
            } else if (Payload_Count >= CurrentPacket.len - 1){    
                Payload_Count = 0;                  // use this so we can error check (instead of TAIL check)
                Check_ID = 0;                       // clear count variables
                BuildState = GET_CHECKSUM;          // change state after tail              
            } else {                
                CurrentPacket.payLoad[Payload_Count] = inchar;
                Payload_Count++;                    // build array in packet payload
                Checksum = Protocol_CalcIterativeChecksum(inchar, Checksum);    
            }
            break;
        
        case (GET_CHECKSUM):
                       
            if (Checksum == inchar){                // check the checksum against the one we calculated
                CurrentPacket.checkSum = Checksum;  // assign checksum
                Checksum = 0;                       // reset value
                BuildState = WAITING_FOR_HEAD;
                PacketReady = TRUE;                 // packet is ready if checksum was correct
            }
            BuildState = WAITING_FOR_HEAD;          // always move to head after this
            break;
                                                    // every state is protected against invalid packets
    }      
}

/**
 * @Function Protocol_QueuePacket (void)
 * @param NONE
 * @return TRUE or FALSE
 * @brief Places a packet into the packet buffer
 * @note Packet is built in the static CurrentPacket
 * @author <Mathias Eriksen>*/
uint8_t Protocol_QueuePacket(void){
    
    if (PacketReady == TRUE){                               // queues packets if  they were built successfully
        if (((PBhead) - (PBtail) > 0) && (((PBhead+1) % PACKET_BUFF_SIZE) == (PBtail))){
            PacketReady = FALSE;                            // return false if packet buffer was full
            return FALSE;                                   // and lower ready flag
            } else {
                PacketBuffer[PBhead] = CurrentPacket;       // put packet CurrentPacket at head              
                PBhead = (PBhead + 1) % PACKET_BUFF_SIZE;   // increment head
                PacketReady = FALSE;                        // lower packet ready flag
                return TRUE;                                // returns true if queue successful
            }                                               // this return is condition for data handling
    } else {
        return FALSE;
    }
   
}

/**
 * @Function  ReadPacket(rxpADT *ADT)
 * @param rxpADT *Packet
 * @return ERROR or SUCCESS
 * @brief Reads a rxpADT from tail, and places it in rxpADT passed by reference
 * @note Increments packet buffer tail
 * @author <Mathias Eriksen>*/
int ReadPacket(rxpADT *ADT){
    
    if (PBtail == PBhead){                          // if packet buffer empty can't read
        return ERROR;
    } else {
        *ADT = (PacketBuffer[PBtail]);              // sets ADT passed by reference to the packet at tail
        PBtail = (PBtail + 1) % PACKET_BUFF_SIZE;   // move the tail
        return SUCCESS;
    }
}

/**
 * @Function DataHandling(void)
 * @param NONE
 * @return NONE
 * @brief Uses read packet to read a packet from buffer, and decides what to do with it
 * @note If needed, builds an outgoing packet in static OutPacket
 * @author <Mathias Eriksen>*/
void PacketHandling(void){
    
    int i;                                              // counting variable                      
    rxpADT Read_Packet;                                 // Packet from tail to handle
    
    if (PacketSending == TRUE){                         // If we are already sending a packet
        return;                                         // leave the function
    }
    else {          
        if (ReadPacket(&Read_Packet) == SUCCESS){       // If we can successfully read from buffer
            
            MessageIDS_t ID = Read_Packet.ID;           // read off the ID
            unsigned char checksum = 0;                 // reset checksum calculation
            unsigned int intval;
            unsigned char uint_array[UINT_SIZE];        // temporary array for ping response

            switch (ID){                                // switch based of of packet ID 

                case (ID_LEDS_SET):     

                    LEDS_SET(Read_Packet.payLoad[0]);   // ID set is one bit payload, so only need index 0
                    break;                              // just sets LEDs, no response

                case (ID_LEDS_GET):

                    OutPacket.len = ID_LEDS_GET_LEN;    // length for LEDS_GET is always 2
                    OutPacket.ID = ID_LEDS_STATE;       // response ID to set is state
                    checksum = Protocol_CalcIterativeChecksum(OutPacket.ID, checksum);

                    OutPacket.payLoad[0] = LEDS_GET();  // use get led function  to return led state  
                    checksum = Protocol_CalcIterativeChecksum(OutPacket.payLoad[0], checksum);

                    OutPacket.checkSum = checksum;      // set packet checksum
                    PacketSending = TRUE;               // raise flag to indicate a packet is ready to be sent
                    break;

                case (ID_DEBUG):

                    OutPacket.len = Read_Packet.len;    // Length is same as sent length
                    OutPacket.ID = ID_DEBUG;            // ID is still debug
                    checksum = Protocol_CalcIterativeChecksum(OutPacket.ID, checksum);

                    for (i=0; i<(Read_Packet.len - 1); i++){            // -1 to get rid of ID
                        OutPacket.payLoad[i] = Read_Packet.payLoad[i];  // set values
                        checksum = Protocol_CalcIterativeChecksum(OutPacket.payLoad[i], checksum);
                    }
                    OutPacket.checkSum = checksum;
                    PacketSending = TRUE;
                    break;

                case (ID_PING):

                    OutPacket.len = Read_Packet.len;    // ping is 4 bytes, pong is 4 bytes
                    OutPacket.ID = ID_PONG;             // ping response is pong
                    checksum = Protocol_CalcIterativeChecksum(OutPacket.ID, checksum);

                    for (i=0;i<UINT_SIZE;i++){
                        uint_array[i] = Read_Packet.payLoad[i];         // move values to temp array
                    }
        
                    intval = Convert_to_int(uint_array);   // conversion to int swaps endianess
                    intval = (intval >> 1);                             // divide by two
                    Convert_to_char(intval, uint_array);                // convert to char also swaps endianness

                    for(i=0; i<UINT_SIZE; i++){
                        OutPacket.payLoad[i] = uint_array[i];           // put modified temp array into out packet
                        checksum = Protocol_CalcIterativeChecksum(OutPacket.payLoad[i], checksum);
                    }
                    
                    OutPacket.checkSum = checksum;      // assign calculated checksum                        
                    PacketSending = TRUE;               // raise flag
                    break;
                
#ifdef Lab2
                case (ID_COMMAND_SERVO_PULSE):
                    
                    OutPacket.len = Read_Packet.len;
                    OutPacket.ID = ID_SERVO_RESPONSE;
                    checksum = Protocol_CalcIterativeChecksum(OutPacket.ID, checksum);
                    
                    for (i=0;i<UINT_SIZE;i++){
                        uint_array[i] = Read_Packet.payLoad[i];         // move values to temp array
                    }
                    
                    intval = Convert_to_int(uint_array);
                    RCServo_SetPulse(intval);
                    
                    for(i=0; i<UINT_SIZE; i++){
                        OutPacket.payLoad[i] = uint_array[i];           // put modified temp array into out packet
                        checksum = Protocol_CalcIterativeChecksum(OutPacket.payLoad[i], checksum);
                    }
                    
                    OutPacket.checkSum = checksum;      // assign calculated checksum                        
                    PacketSending = TRUE;               // raise flag
                    break;
                
                case (ID_LAB2_INPUT_SELECT):
                    intval = 0;
                    PingSensor_Init(Read_Packet.payLoad[0]);
                    RotaryEncoder_Init(Read_Packet.payLoad[0]);
                    break;
#endif
            }           
        } 
    } 
}

/**
 * @Function Convert_to_int(unsigned char *c)
 * @param *c, array of 4 unsigned chars
 * @return int, integer representation of the unsigned chars
 * @brief converts 4 unsigned chars in an array to an int
 * @note swaps endianness during int conversion, so no need for endianness conversion
 * @author <Mathias Eriksen>*/
unsigned int Convert_to_int(unsigned char *c){

  unsigned int result;
  int i;
  
  for (i=0; i<4; i++){     
    result = result << 8 | c[i];    // char is put in at last byte of int then shifted left
  }                                 // swaps endianess b/c c[0] will end up at left side
  return result;                    // return resulting int
  
}

/**
 * @Function Convert_to_char(unsigned int val, unsigned char *c)
 * @param *c, array of 4 unsigned chars, int val to be converted
 * @return NONE
 * @brief converts an int into array of 4 unsigned chars
 * @note swaps endianness during conversion, so no need for endianness conversion
 * @author <Mathias Eriksen>*/
void Convert_to_char(unsigned int val, unsigned char *c){
    
    c[0] = (val >> 24) & 0xff;      // masks the value at last byte
    c[1] = (val >> 16) & 0xff;      // and the shift ensures each byte is put into array
    c[2] = (val >> 8) & 0xff; 
    c[3] = (val >> 0 ) & 0xff;  
  
}

/**
 * @Function SendPacket(unsigned char *outchar)
 * @param unsigned char *outchar
 * @return NONE
 * @brief sends a packet based on out packet built in DataHandling();
 * @note sends packet one char at a time, char selected based on states
 * @author <Mathias Eriksen>*/
void SendPacket(unsigned char *outchar){
    
    static int i = 0;                       // file level static since counter must be preserved
                                            // between calls
        switch (SendState){                 // switches based on current state
                                            // sets value outchar based on where state machine is
            case (SEND_HEAD):
                *outchar = HEAD;        
                SendState = SEND_LEN;
                break;
                
            case (SEND_LEN):
                *outchar = OutPacket.len;
                SendState = SEND_ID;
                break;
                
            case (SEND_ID):
                *outchar =  OutPacket.ID;
                SendState = SEND_PAYLOAD;
                break;  
                
            case (SEND_PAYLOAD):
                
                if (i >= ((OutPacket.len)-1)){
                    i = 0;                  // once payload is done (length - 1) since ID
                    *outchar = TAIL;        // send tail and reset payload counter
                    SendState = SEND_CHECKSUM;
                    break;
                } else {
                    *outchar = OutPacket.payLoad[i];
                    i++;
                }                
                break;
                                
            case (SEND_CHECKSUM):
                *outchar = OutPacket.checkSum;
                SendState = SEND_R;
                break;
                
            case (SEND_R):
                *outchar = BACKSLASHR;
                SendState = SEND_N;
                break;
                
            case (SEND_N):
                *outchar = BACKSLASHN;
                PacketSending = FALSE;      // reset packet sending flag since packet has been sent
                SendState = SEND_HEAD;      // put machine back in head state     
                break;                
        }       
       
}

int Send_Packet(unsigned char ID, unsigned char length, unsigned char *payload){
    
    int i;
    unsigned char checksum = 0;
    
    PutChar(HEAD);
    PutChar(length);
    PutChar(ID);
    checksum = Protocol_CalcIterativeChecksum(ID, checksum);
    
    for (i=0; i<length-1; i++){
        PutChar(payload[i]);
        checksum = Protocol_CalcIterativeChecksum(payload[i], checksum);        
    }
    
    PutChar(TAIL);
    PutChar(checksum);
    PutChar(BACKSLASHR);
    PutChar(BACKSLASHN); 
    
}
/**
 * @Function ProcessPackets(void)
 * @param NONE
 * @return NONE
 * @brief Only function that had to be placed in a while(1) to use packet interface over UART
 * @note Tries to avoid blocking code, chars can be put and get in one  while(1) loop
 * @author <Mathias Eriksen>*/
void ProcessPackets(void){
         
    unsigned char temp;

    if (GetChar(&temp) != ERROR){               // If there is a char to receive from RXBuffer
        BuildRxPacket(temp);                    // build packet using the recieved char
        if (Protocol_QueuePacket() == TRUE){    // packet must have been fully built/queued to return true
            PacketHandling();                   // handle the packet that has been bult
        }
    }

    if (PacketSending == TRUE){                 // if a packet has been handled, and out packet has been built
        SendPacket(&temp);                      // get char to be sent using Send Packet state machine
        PutChar(temp);                          // put that into TXBuffer for Transmission
    }           
    
}

#ifdef TestHarness
int main(){
    
    BOARD_Init();
    LEDS_INIT();
    Protocol_Init(BR);
    
    unsigned char str[MAXPAYLOADLENGTH];
    sprintf(str, "Welcome to Mterikse's Lab1, Serial Device Driver and Protocol Communications. Compiled at %s %s \0",__DATE__, __TIME__);
    
    Send_Packet(ID_DEBUG, strlen(str)-1 , str);
    
    while (1){
        
        ProcessPackets();
        
    }   
}
#endif