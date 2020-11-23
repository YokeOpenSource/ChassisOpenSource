#ifndef SR73_RADAR_H_
#define SR73_RADAR_H_
#include "module_vehicle.h"

typedef struct
{	
	uint8_t target_num ;
	uint8_t object_id;
	double dislong;
	double dislat;
	double vrelong;
    uint8_t dynprop;
	double vrelat;
	double RCS;
	double angle;
	double radius;
	double target_speed;
}Radar_Object_Data_t;


void SR73_Radar_MessageDispatch(CanRxMsg *can_rx);
void SR28F_Radar_MessageDispatch(CanRxMsg *can_rx);
void Radar_CheckObstract(void * param);
uint8_t Radar_GetObstractStatus(void);

#endif

