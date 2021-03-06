/*
**************************************************************************************************************
*	@file		KQ6600.c
*	@author 	Jason
*   @contact    Jason_531@163.com
*	@version  	V0.1
*	@date     	2019/12/15
*	@brief		KQ6600甲醛驱动
***************************************************************************************************************
*/ 
#include "KQ6600.h"

xKQUart_t xKQUart = {0, {0}};

/**
  * 函数功能：初始化KQ6600
  * 输入参数：无
  * 返 回 值：无
  * 说    明：
  */
void vKQInit(void)
{
	__HAL_UART_CLEAR_IDLEFLAG(&usartKQ6600);
	__HAL_UART_ENABLE_IT(&usartKQ6600,UART_IT_IDLE);	
	
	HAL_UART_AbortReceive_IT(&usartKQ6600);
	HAL_UART_Receive_IT(&usartKQ6600, xKQUart.ucRxBuf, RX_MAX_LEN );
}

/**
  * 函数功能：获取传感器数据
  * 输入参数：传感器数据缓存
  * 返 回 值：无
  * 说    明：
  */
void vKQReadSensor(uint8_t ucKQBuf[4])
{
	uint8_t ucCrc = 0;
	
	if(xKQUart.ucLen>1)
	{
		DEBUG(2,"KQRecv: ");
		for(uint8_t ucI = 0; ucI < xKQUart.ucLen; ++ucI)
		{
			DEBUG(2,"%02X ",xKQUart.ucSensorBuf[ucI]);
		}
		DEBUG(2,"\r\n");
		for(uint8_t ucI = 0; ucI < xKQUart.ucLen-1; ++ucI)
		{
			ucCrc += xKQUart.ucSensorBuf[ucI];
		}
		if(ucCrc == xKQUart.ucSensorBuf[xKQUart.ucLen-1])
		{
			memcpy(ucKQBuf, &xKQUart.ucSensorBuf[1], 4);
		}
	}
}

