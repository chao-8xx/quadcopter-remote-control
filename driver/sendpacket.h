#ifndef _sendpacket_h_
#define _sendpacket_h_

#include "stm32f10x.h"

typedef struct
{
    uint16_t thr;
    uint8_t pit;
    uint8_t rol;
    uint8_t yaw;
    uint8_t key;
    
}RemoteData;


extern RemoteData tx;
void NrfRxPacket(void);
void OledDisplayPairStatus(void);

#endif

