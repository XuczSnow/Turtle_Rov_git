/**
  ******************************************************************************
  * @file    turtle_dbg.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 调试相关函数
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

void Turtle_DBG_TimeStart(uint8_t ch, uint8_t sub)
{
#if TIMESTAMP_EN == G_TURE
	switch(ch)
	{
		case DBGTS_CH1:
			EventStartA(sub);
			break;
		case DBGTS_CH2:
			EventStartB(sub);
			break;
		case DBGTS_CH3:
			EventStartC(sub);
			break;
		case DBGTS_CH4:
			EventStartD(sub);
			break;
	}
#endif
}

void Turtle_DBG_TimeStop(uint8_t ch, uint8_t sub)
{
#if TIMESTAMP_EN == G_TURE
	switch(ch)
	{
		case DBGTS_CH1:
			EventStopA(sub);
			break;
		case DBGTS_CH2:
			EventStopB(sub);
			break;
		case DBGTS_CH3:
			EventStopC(sub);
			break;
		case DBGTS_CH4:
			EventStopD(sub);
			break;
	}
#endif
}


/**
 * Function to perform jump to system memory boot from user application
 *
 * Call function when you want to jump to system memory
 */
void JumpToBootloader(void)
{
	uint8_t i;
	
	void (*SysMemBootJump)(void);

	volatile uint32_t addr = 0x1FFFF000; //此地址为STM32F103RE的地址，其余型号地址自行在数据手册中进行查找
	 
	/**
	 * Step: Disable RCC, set it to default (after reset) settings
	 *       Internal clock, no PLL, etc.
	 */
#if defined(USE_HAL_DRIVER)
	HAL_RCC_DeInit();
#endif /* defined(USE_HAL_DRIVER) */
#if defined(USE_STDPERIPH_DRIVER)
	RCC_DeInit();
#endif /* defined(USE_STDPERIPH_DRIVER) */
	
	/**
	 * Step: Disable systick timer and reset it to default values
	 */
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;

	/**
	 * Step: Disable all interrupts
	 */
	__disable_irq();
	
	/* 关闭所有中断，清除所有中断挂起标志 */
	for (i = 0; i < 8; i++)
	{
		NVIC->ICER[i]=0xFFFFFFFF;
		NVIC->ICPR[i]=0xFFFFFFFF;
	}	
	/**
	 * Step: Remap system memory to address 0x0000 0000 in address space
	 *       For each family registers may be different. 
	 *       Check reference manual for each family.
	 *
	 *       For STM32F4xx, MEMRMP register in SYSCFG is used (bits[1:0])
	 *       For STM32F0xx, CFGR1 register in SYSCFG is used (bits[1:0])
	 *       For others, check family reference manual
	 */
	//Remap by hand... {
#if defined(STM32F4)
	SYSCFG->MEMRMP = 0x01;
#endif
#if defined(STM32F0)
	SYSCFG->CFGR1 = 0x01;
#endif
	//} ...or if you use HAL drivers
	//__HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();	//Call HAL macro to do this for you
	
	/**
	 * Step: Set jump memory location for system memory
	 *       Use address with 4 bytes offset which specifies jump location where program starts
	 */
	SysMemBootJump = (void (*)(void)) (*((uint32_t *)(addr + 4)));
	
	/**
	 * Step: Set main stack pointer.
	 *       This step must be done last otherwise local variables in this function
	 *       don't have proper value since stack pointer is located on different position
	 *
	 *       Set direct address location which specifies stack pointer in SRAM location
	 */
	//__set_MSP(*(uint32_t *)addr);
	
	/**
	 * Step: Actually call our function to jump to set location
	 *       This will start system memory execution
	 */
	SysMemBootJump();
	
	/**
	 * Step: Connect USB<->UART converter to dedicated USART pins and test
	 *       and test with bootloader works with STM32 Flash Loader Demonstrator software
	 */
}

void Soft_Reset(void)
{
	NVIC_SystemReset();
}
