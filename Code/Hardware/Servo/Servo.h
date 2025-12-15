/******************************************************
*		本文件由MrWei95开源
*		文件使用UTF-8编码
*		文件名称：Servo.h
*		文件创建日期：2025/12/15
*		最后修改日期：2025/12/15
*		最后修改：MrWei95
******************************************************/

#ifndef __SERVO_H
#define __SERVO_H

#include "main.h"

#define	SERVO_MAX_ANGLE		180		// 定义舵机最大角度

void Servo_R_SetAngle(float Angle);
void Servo_L_SetAngle(float Angle);

void Servo_Init(void);

#endif
