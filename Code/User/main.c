/******************************************************
*		本文件由MrWei95开源
*		文件使用UTF-8编码
*		文件名称：main.c
*		文件创建日期：2025/04/09
*		最后修改日期：2025/12/15
*		最后修改：MrWei95
******************************************************/

#include "main.h"
#include "NRF24L01/NRF24L01.h"

#ifdef	NRF24L01_TX_MODE	// 定义发送模式
#include "Key/Key.h"
#include "OLED/OLED.h"

#endif	// 定义发送模式

#ifdef	NRF24L01_RX_MODE	// 定义接收模式
#include "Servo/Servo.h"

// 检查接收数据
void Check_RXdata(uint8_t Buf[]);

#endif	// 定义接收模式

uint8_t Flag = 0;
uint8_t KeyNum;


int main(void)
{
	NRF24L01_Init();
#ifdef	NRF24L01_TX_MODE	// 定义发送模式
	Key_Init();
	OLED_Init();
	uint8_t Wireless_TXdata[32] = {0xFF, 0xFE, 0x00, 0x00, 0xEE, 0xED};
	
	OLED_ShowString(0, 12, "----------------", OLED_8X16);
	OLED_ShowNum(11, 0, 308, 3, OLED_8X16);
	OLED_ShowChinese(35, 0, "看门狗");
	OLED_ShowString(87, 0, "v2.0", OLED_8X16);
	OLED_ShowChinese(10, 30, "执行命令");
	OLED_ShowString(76, 30, ":", OLED_8X16);
	OLED_ShowChinese(88, 30, "无");
	OLED_Update();
	
	while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)		// 开
		{
			Wireless_TXdata[3] = 0x11;
			NRF24L01_Send(Wireless_TXdata);
			OLED_ShowChinese(88, 30, "开灯");
			OLED_Update();
		}
		else if (KeyNum == 2)	// 关
		{
			Wireless_TXdata[3] = 0x22;
			NRF24L01_Send(Wireless_TXdata);
			OLED_ShowChinese(88, 30, "关灯");
			OLED_Update();
		}
	}
}
#endif	// 定义发送模式

#ifdef	NRF24L01_RX_MODE	// 定义接收模式
	Servo_Init();
	uint8_t Wireless_RXBuf[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	while (1)
	{
		if(NRF24L01_R_IRQ() == 0)
		{
			NRF24L01_Receive(Wireless_RXBuf);
			Check_RXdata(Wireless_RXBuf);
			
			if (Flag == 1)
			{
				Flag = 0;
				if (Wireless_RXBuf[3] == 0x11)		// 开灯命令
				{
					Servo_R_SetAngle(70);
					Servo_L_SetAngle(110);
					Delay_s(1);
					Servo_R_SetAngle(90);
					Servo_L_SetAngle(90);
				}
				else if (Wireless_RXBuf[3] == 0x22)	// 关灯命令
				{
					Servo_R_SetAngle(110);
					Servo_L_SetAngle(70);
					Delay_s(1);
					Servo_R_SetAngle(90);
					Servo_L_SetAngle(90);
				}
			}
		}
	}
}


/****
	* @brief	检查接收数据
	* @param	Buf[]：接收的数据包
	* @retval	None
	*/
void Check_RXdata(uint8_t Buf[])
{
	if (Buf[0] == 0xFF && Buf[1] == 0xFE)
	{
		if (Buf[4] == 0xEE && Buf[5] == 0xED)
		{
			Flag = 1;
		}
	}
}

#endif	// 定义接收模式
