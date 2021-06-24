

#ifndef RINGBUFF_H
#define RINGBUFF_H

typedef struct {
	uint16_t iIndexIN;
	uint16_t iIndexOUT;
	uint16_t iCnt_Handle;
	uint16_t iCnt_OverFlow;
}RING_BUFF_INFO;


//================================================
// FUNCTION
//================================================

void DRV_RingBuffPtr__Clean(RING_BUFF_INFO *ptr);

void DRV_RingBuffPtr__Input(RING_BUFF_INFO *ptr, uint16_t iBufferSize);

void DRV_RingBuffPtr__Output(RING_BUFF_INFO *ptr, uint16_t iBufferSize);
void DRV_RingBuffPtr__Output_JumpToNew(RING_BUFF_INFO *ptr, uint16_t iBufferSize);

#endif // RINGBUFF_H

