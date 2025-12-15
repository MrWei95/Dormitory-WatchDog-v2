/******************************************************
*		本文件由MrWei95开源
*		文件使用UTF-8编码
*		文件名称：Key.c
*		文件创建日期：2025/11/17
*		最后修改日期：2025/12/15
*		最后修改：MrWei95
******************************************************/

#include "Key/Key.h"

/*******************************************************/
/******************** 按键 引脚操作 ********************/

/****
	* @brief	获取按键值(按键按住不放程序阻塞)
	* @param	None
	* @retval	KeyNum:0:无按键操作
	* @retval	KeyNum:n:n按键操作
	*/
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0);
		Delay_ms(20);
		KeyNum = 1;
	}
	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == 0);
		Delay_ms(20);
		KeyNum = 2;
	}
	return KeyNum;
}

/******************** 按键 引脚操作 ********************/
/*******************************************************/


/*****************************************************/
/******************** 按键 初始化 ********************/

/****
	* @brief	按键初始化
	* @param	None
	* @retval	None
	*/
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/******************** 按键 初始化 ********************/
/*****************************************************/
