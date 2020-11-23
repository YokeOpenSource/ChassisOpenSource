#ifndef SERIALCTRLTASK_H_
#define SERIALCTRLTASK_H_
#include "stdint.h"

#pragma pack(1)

typedef struct
{
	uint16_t header;
	uint8_t uid;
	uint8_t len;
	int16_t line_speed;
	int16_t angular_speed;
	int32_t odom_l;
	int32_t odom_r;
	int16_t yaw;
	int16_t gyro[3];
	int16_t acc[3];
	uint16_t battery_vlot;
    uint16_t avoidance_dis;
	uint16_t checksum;
}SerialFdb_Frame_t;


typedef struct
{
	uint16_t header;
	uint8_t len;
	uint8_t frame_id;
	int16_t line_speed;
	int16_t angular_speed;
	uint8_t checksum;
}SerialCtrl_Frame_t;

typedef struct
{
	uint16_t header;
	uint8_t len;
	uint8_t frame_id;
    uint16_t avoidance_dis;
	uint8_t save_flag;
	uint8_t checksum;
}SerialRadarParam_Frame_t;

#pragma pack()


void SerialHandleTask(void *pvParam);

#endif

