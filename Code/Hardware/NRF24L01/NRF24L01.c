/******************************************************
*		本文件由MrWei95开源
*		文件使用UTF-8编码
*		文件名称：NRF24L01.c
*		文件创建日期：2025/04/09
*		最后修改日期：2025/12/12
*		最后修改：MrWei95
******************************************************/

#include "NRF24L01/NRF24L01.h"

uint8_t NRF24L01_Send_ADDR[5] = {0xF0, 0xF0, 0xF0, 0xF0, 0xF0};
uint8_t NRF24L01_Receive_ADDR[5] = {0xF0, 0xF0, 0xF0, 0xF0, 0xF0};


/***********************************************************/
/******************** NRF24L01 引脚操作 ********************/

/****
	* @brief	NRF24L01写CSN
	* @param	Value:电平值
	* @retval	None
	*/
void NRF24L01_W_CSN(uint8_t Value)
{
	GPIO_WriteBit(NRF24L01_CSN_Port, NRF24L01_CSN_Pin, (BitAction)Value);
}

/****
	* @brief	NRF24L01写CE
	* @param	Value:电平值
	* @retval	None
	*/
void NRF24L01_W_CE(uint8_t Value)
{
	GPIO_WriteBit(NRF24L01_CE_Port, NRF24L01_CE_Pin, (BitAction)Value);
}

/****
	* @brief	NRF24L01读取IRQ电平
	* @param	None
	* @retval	1:高电平
	* @retval	0:低电平
	*/
uint8_t NRF24L01_R_IRQ(void)
{
	return GPIO_ReadInputDataBit(NRF24L01_IRQ_Port, NRF24L01_IRQ_Pin);
}

/******************** NRF24L01 引脚操作 ********************/
/***********************************************************/


/*************************************************************/
/******************** NRF24L01 寄存器操作 ********************/

/****
	* @brief	NRF24L01 SPI交换一个字节
	* @param	Byte:字节数据
	* @retval	None
	*/
uint8_t NRF24L01_SPI_SwapByte(uint8_t Byte)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, Byte);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}

/****
	* @brief	NRF24L01写寄存器
	* @param	Reg:寄存器地址
	* @param	Value:值
	* @retval	None
	*/
void NRF24L01_W_Reg(uint8_t Reg, uint8_t Value)
{
	NRF24L01_W_CSN(0);						// 表示选中NRF24L01
	NRF24L01_SPI_SwapByte(Reg);				// 交换的第一个字节就是指令
	NRF24L01_SPI_SwapByte(Value);			// 交换的第二个字节就是交换的数据
	NRF24L01_W_CSN(1);						// 停止选中NRF24L01
}

/****
	* @brief	NRF24L01读寄存器
	* @param	Reg:寄存器地址
	* @retval	Value:寄存器数据
	*/
uint8_t NRF24L01_R_Reg(uint8_t Reg)
{
	uint8_t Value;
	NRF24L01_W_CSN(0);									// 表示选中NRF24L01
	NRF24L01_SPI_SwapByte(Reg);							// 交换的第一个字节就是指令
	Value = NRF24L01_SPI_SwapByte(NRF24L01_NOP);		// 交换的第二个字节就是交换的数据
	NRF24L01_W_CSN(1);									// 停止选中NRF24L01
	return Value;
}

/****
	* @brief	NRF24L01写数据包
	* @param	Reg:寄存器地址
	* @param	Buf:传递数据包数组
	* @param	Len:数据包长度
	* @retval	None
	*/
void NRF24L01_W_Buf(uint8_t Reg, uint8_t* Buf, uint8_t Len)
{
	uint8_t i;
	NRF24L01_W_CSN(0);					// 选中NRF24L01
	NRF24L01_SPI_SwapByte(Reg);
	for(i = 0; i < Len; i++)
		NRF24L01_SPI_SwapByte(Buf[i]);
	NRF24L01_W_CSN(1);					// 停止选中NRF24L01
}

/****
	* @brief	NRF24L01读数据包
	* @param	Reg:寄存器地址
	* @param	Buf:传递数据包数组
	* @param	Len:数据包长度
	* @retval	None
	*/
void NRF24L01_R_Buf(uint8_t Reg, uint8_t* Buf, uint8_t Len)
{
	uint8_t i;
	NRF24L01_W_CSN(0);					// 选中NRF24L01
	NRF24L01_SPI_SwapByte(Reg);
	for(i = 0; i<Len; i++)
		Buf[i] = NRF24L01_SPI_SwapByte(NRF24L01_NOP);
	NRF24L01_W_CSN(1);					// 停止选中NRF24L01
}

/******************** NRF24L01 寄存器操作 ********************/
/*************************************************************/


/*************************************************************/
/******************** NRF24L01 初始化操作 ********************/

/****
	* @brief	NRF24L01硬件SPI初始化
	* @param	None
	* @retval	None
	*/
void NRF24L01_SPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	// SCK 和 MOSI 配置为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = NRF24L01_SCK_Pin | NRF24L01_MOSI_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// MISO 配置为浮空输入
	GPIO_InitStructure.GPIO_Pin = NRF24L01_MISO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// SPI 初始化
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		// 适当分频
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}

/****
	* @brief	NRF24L01初始化
	* @param	None
	* @retval	None
	*/
void NRF24L01_Init(void)
{
	NRF24L01_SPI_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CSN_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(NRF24L01_CSN_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(NRF24L01_CE_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(NRF24L01_IRQ_Port, &GPIO_InitStructure);
	
	NRF24L01_W_CE(0);
	
	NRF24L01_W_Buf(NRF24L01_W_REGISTER + NRF24L01_TX_ADDR, NRF24L01_Send_ADDR, 5);			// 配置发送地址
	NRF24L01_W_Buf(NRF24L01_W_REGISTER + NRF24L01_RX_ADDR_P0, NRF24L01_Receive_ADDR, 5);	// 配置接收通道0
	NRF24L01_W_Reg(NRF24L01_W_REGISTER + NRF24L01_CONFIG, 0x0F);							// 配置成接收模式
	NRF24L01_W_Reg(NRF24L01_W_REGISTER + NRF24L01_EN_AA, 0x01);								// 通道0开启自动应答
	NRF24L01_W_Reg(NRF24L01_W_REGISTER + NRF24L01_RF_CH, 0x00);								// 配置通信频率2.4G
	NRF24L01_W_Reg(NRF24L01_W_REGISTER + NRF24L01_RX_PW_P0, 32);							// 配置接收通道0接收的数据宽度32字节
	NRF24L01_W_Reg(NRF24L01_W_REGISTER + NRF24L01_EN_RXADDR, 0x01);							// 接收通道0使能
	NRF24L01_W_Reg(NRF24L01_W_REGISTER + NRF24L01_SETUP_RETR, 0x1A);						// 配置580us重发时间间隔,重发10次
	NRF24L01_W_Reg(NRF24L01_FLUSH_RX, NRF24L01_NOP);
	
	NRF24L01_W_CE(1);
}

/******************** NRF24L01 初始化操作 ********************/
/*************************************************************/


/***********************************************************/
/******************** NRF24L01 收发操作 ********************/

/****
	* @brief	NRF24L01接收数据
	* @param	Buf:接收数据存放的数组
	* @retval	None
	*/
void NRF24L01_Receive(uint8_t* Buf)
{
	uint8_t Status;
	Status = NRF24L01_R_Reg(NRF24L01_R_REGISTER + NRF24L01_STATUS);
	if(Status & NRF24L01_RX_OK)
	{
		NRF24L01_R_Buf(NRF24L01_R_RX_PAYLOAD, Buf, 32);
		NRF24L01_W_Reg(NRF24L01_FLUSH_RX, NRF24L01_NOP);
		NRF24L01_W_Reg(NRF24L01_W_REGISTER + NRF24L01_STATUS, Status);
		Delay_us(150);
	}
}

/****
	* @brief	NRF24L01发送数据
	* @param	Buf:接收数据存放的数组
	* @retval	NRF24L01状态
	*/
uint8_t NRF24L01_Send(uint8_t* Buf)
{
	uint8_t Status;
	NRF24L01_W_Buf(NRF24L01_W_TX_PAYLOAD, Buf, 32);						// 在发送数据缓存器发送要发送的数据
	
	NRF24L01_W_CE(0);
	NRF24L01_W_Reg(NRF24L01_W_REGISTER + NRF24L01_CONFIG, 0x0E);
	NRF24L01_W_CE(1);

	while(NRF24L01_R_IRQ() == 1);										// 等待中断
	Status = NRF24L01_R_Reg(NRF24L01_R_REGISTER + NRF24L01_STATUS);
	
	if(Status & NRF24L01_MAX_TX)										// 如果发送达到最大次数
	{
		NRF24L01_W_Reg(NRF24L01_FLUSH_TX, NRF24L01_NOP);				// 清除发送数据缓存器
		NRF24L01_W_Reg(NRF24L01_W_REGISTER + NRF24L01_STATUS, Status);	// 中断位写1清除中断
		return NRF24L01_MAX_TX;
	}
	if(Status & NRF24L01_TX_OK)											// 如果发送成功,接收到应答信号
	{
		NRF24L01_W_Reg(NRF24L01_W_REGISTER + NRF24L01_STATUS, Status);	// 清除中断
		return NRF24L01_TX_OK;
	}
	return 0;
}

/******************** NRF24L01 收发操作 ********************/
/***********************************************************/
