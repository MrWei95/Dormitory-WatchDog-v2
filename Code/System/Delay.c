/******************************************************
*		本文件由MrWei95开源
*		文件使用UTF-8编码
*		文件名称：Delay.c
*		文件创建日期：2025/11/17
*		最后修改日期：2025/12/12
*		最后修改：MrWei95
******************************************************/

#include "Delay.h"

/****
	* @brief	延时微秒
	* @param	xus:延时时长(范围：0~233015)
	* @retval	None
	*/
void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				// 设置定时器重装值
	SysTick->VAL = 0x00;					// 清空当前计数值
	SysTick->CTRL = 0x00000005;				// 设置时钟源为HCLK，启动定时器
	while(!(SysTick->CTRL & 0x00010000));	// 等待计数到0
	SysTick->CTRL = 0x00000004;				// 关闭定时器
}

/****
	* @brief	延时毫秒
	* @param	xms:延时时长(范围：0~4294967295)
	* @retval	None
	*/
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
}
 
/****
	* @brief	延时秒
	* @param	xs:延时时长(范围：0~4294967295)
	* @retval	None
	*/
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 
