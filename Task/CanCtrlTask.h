#ifndef CANCTRLTASK_H
#define CANCTRLTASK_H
#include "stm32f4xx.h"


void DispatchCANMessage(CanRxMsg *can_rx);
void CAN_HandleTask(void *pvParam);

#endif
