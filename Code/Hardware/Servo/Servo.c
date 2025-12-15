/******************************************************
*		本文件由MrWei95开源
*		文件使用UTF-8编码
*		文件名称：Servo.c
*		文件创建日期：2025/12/15
*		最后修改日期：2025/12/15
*		最后修改：MrWei95
******************************************************/

#include "Servo/Servo.h"


/*******************************************************/
/******************** 舵机 PWM 操作 ********************/

/****
	* @brief	舵机PWM初始化(用户不应直接使用此函数)
	* @param	None
	* @retval	None
	*/
void Servo_PWM_Init(void)
{
	/* 配置TIM2 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		// 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// TIM2_CH2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// TIM2_CH3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;				// 计数周期ARR值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				// 预分频器PSC值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;		// 定义结构体变量
	TIM_OCStructInit(&TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	// 输出极性高
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);	// 初始化通道2
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);	// 初始化通道3
	
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	TIM_Cmd(TIM2, ENABLE);		// 使能TIM2，定时器开始运行
}

/****
	* @brief	舵机设置TIM2的CCR2
	* @param	Compare:要写入的CCR2的值(范围：0~100)
	* @retval	None
	*/
void Servo_PWM_SetCompare2(uint16_t Compare)
{
	// CCR和ARR共同决定占空比，Duty = CCR / (ARR + 1)
	TIM_SetCompare2(TIM2, Compare);		// 设置CCR2的值
}

/****
	* @brief	舵机设置TIM2的CCR3
	* @param	Compare:要写入的CCR3的值(范围：0~100)
	* @retval	None
	*/
void Servo_PWM_SetCompare3(uint16_t Compare)
{
	// CCR和ARR共同决定占空比，Duty = CCR / (ARR + 1)
	TIM_SetCompare3(TIM2, Compare);		// 设置CCR3的值
}

/******************** 舵机 PWM 操作 ********************/
/*******************************************************/


/******************************************************/
/******************** 舵机角度设置 ********************/

/****
	* @brief	舵机R设置角度
	* @param	Angle:要设置的舵机角度(范围：0~SERVO_MAX_ANGLE)
	* @retval	None
	*/
void Servo_R_SetAngle(float Angle)
{
	Servo_PWM_SetCompare2(Angle / SERVO_MAX_ANGLE * 2000 + 500);	// 设置占空比
}

/****
	* @brief	舵机L设置角度
	* @param	Angle:要设置的舵机角度(范围：0~SERVO_MAX_ANGLE)
	* @retval	None
	*/
void Servo_L_SetAngle(float Angle)
{
	Servo_PWM_SetCompare3(Angle / SERVO_MAX_ANGLE * 2000 + 500);	// 设置占空比
}

/******************** 舵机角度设置 ********************/
/******************************************************/


/*****************************************************/
/******************** 舵机 初始化 ********************/

/****
	* @brief	舵机初始化(用户应使用此函数初始化舵机)
	* @param	None
	* @retval	None
	*/
void Servo_Init(void)
{
	Servo_PWM_Init();
	Servo_R_SetAngle(90);
	Servo_L_SetAngle(90);
}

/******************** 舵机 初始化 ********************/
/*****************************************************/
