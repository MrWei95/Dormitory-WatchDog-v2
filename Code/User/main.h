/******************************************************
*		本文件由MrWei95开源
*		文件使用UTF-8编码
*		文件名称：main.h
*		文件创建日期：2025/04/09
*		最后修改日期：2025/12/15
*		最后修改：MrWei95
******************************************************/

#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"		// Device header
#include "Delay.h"


// 控制NRF24L01收发模式
#define		NRF24L01_MODE		0
#if NRF24L01_MODE == 0		// NRF24L01收发模式判断
#define		NRF24L01_TX_MODE		// 发送模式
#else
#define		NRF24L01_RX_MODE		// 接收模式
#endif	// NRF24L01收发模式判断

#endif
