#ifndef REMOTERCTRL_TASK_H
#define REMOTERCTRL_TASK_H

double Remoter_GetX(void);
double Remoter_GetY(void);
double Remoter_GetZ(void);

void RemoterCtrlHandleTask(void * pvParam);

#endif

