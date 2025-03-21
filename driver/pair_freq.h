#ifndef _pair_freq_h_
#define _pair_freq_h_



#include "stm32f10x.h"

typedef enum{
	NOT = 0,
	STEP1,
	DONE,
}PairStep;

typedef struct
{
	PairStep step;
	uint8_t addr[5];
	uint8_t freq_channel;
}Pair;

void WaitPairing(void);
void get_chip_id(void);


#endif

