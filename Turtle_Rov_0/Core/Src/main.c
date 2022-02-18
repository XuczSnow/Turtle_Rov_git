/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
	
	/*
	一、时间测量说明
			1.通道1测量个任务运行时间
			2.通道2测量逻辑控制运行时间
	*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "iwdg.h"
#include "turtle_g.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static int16_t int16_buf;
static uint8_t key_led_cnt = 0;
static uint8_t key_gear_cnt= 0;
static uint8_t key_gmax_cnt= 0;
static uint16_t flash_buf = 0x0000;

char turtle_start_info[] =
{
	"\r\n"
	"|,--------.                 ,--.  ,--.           ,------.  ,-----.,--.   ,--. |\r\n"
	"|'--.  .--',--.,--.,--.--.,-'  '-.|  | ,---.     |  .--. ''  .-.  '\\  `.'  /  |\r\n"
	"|   |  |   |  ||  ||  .--''-.  .-'|  || .-. :    |  '--'.'|  | |  | \\     /   |\r\n"
	"|   |  |   '  ''  '|  |     |  |  |  |\\   --.    |  |\\  \\ '  '-'  '  \\   /    |\r\n"
	"|   `--'    `----' `--'     `--'  `--' `----'    `--' '--' `-----'    `-'     |\r\n"
	"\r\n"
	"Build:       "__DATE__" "__TIME__"\r\n"
	"Version:     ver 1.0.0_x\r\n"
	"Copyright:   (c) 2021 Xucz(OUC Fab U+/ROV Team) \r\n\r\n",
};

uint16_t turtle_start_infolen = sizeof(turtle_start_info);

char turtle_init_info[] = "System Initing ......";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int stdout_putchar (int ch)
{
	HAL_UART_Transmit(&HOST_UART,(uint8_t *)&ch,1u,0xff);
	return ch;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
	
	FLASH_Init();
	HAL_Delay(10);
	
#if FLASH_INIT == G_TURE
	FLASH_Write(JUMP_BOOTLOADER_ADD,&flash_buf,2u);
#endif
	FLASH_Read(JUMP_BOOTLOADER_ADD,&flash_buf,2u);
	
	if (flash_buf == 0x0001)
	{
		flash_buf = 0x0000;
		FLASH_Write(JUMP_BOOTLOADER_ADD,&flash_buf,2u);
		JumpToBootloader();
	}
	
	//等待网络准备好
	HAL_Delay(8000);
	
	Shell_PutStr(DISPLAY_CLR,sizeof(DISPLAY_CLR));
	Shell_PutStr(turtle_start_info,sizeof(turtle_start_info));
	HAL_Delay(1000);
	Shell_PutStr(turtle_init_info,sizeof(turtle_init_info));
	
	Turtle_Bsp_Init();
	
	userShellInit();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	//MX_IWDG_Init();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//设备控制
#if 0  //旧的操作方式
		if (host[1].joy_stick[KID_RB])
		{
			//控制前灯亮度
			if (host[1].joy_stick[KID_RIGHT] == 0x01 && key_led_cnt>0x40)
			{
				hRov.led_lumin1 ++;
				hRov.led_lumin2 ++;
				if (hRov.led_lumin1>LED_PWM_MAX)
					hRov.led_lumin1=LED_PWM_MAX;
				if (hRov.led_lumin2>LED_PWM_MAX)
					hRov.led_lumin2=LED_PWM_MAX;
				key_led_cnt = 0;
			}
			else if (host[1].joy_stick[KID_LEFT] == 0x01 && key_led_cnt>0x40)
			{
				hRov.led_lumin1 --;
				hRov.led_lumin2 --;
				if	(hRov.led_lumin1<LED_PWM_MIN)
					hRov.led_lumin1=LED_PWM_MIN;
				if	(hRov.led_lumin2<LED_PWM_MIN)
					hRov.led_lumin2=LED_PWM_MIN;
				key_led_cnt = 0;
			}
			else
			{
				key_led_cnt ++;
			}
			
			//控制挡位
			static uint8_t gear_cnt = 1;
			if (host[1].joy_stick[KID_UP] == 0x01)
			{
				if (key_gear_cnt > 3)
				{
					if (gear_cnt < 3)
						gear_cnt ++;
					
					host[1].prop_gear = gear_switch[gear_cnt];
				}
				key_gear_cnt = 0;
			}
			else if (host[1].joy_stick[KID_DOWN] == 0x01)
			{
				if (key_gear_cnt > 3)
				{
					if (gear_cnt > 1)
						gear_cnt --;
					
					host[1].prop_gear = gear_switch[gear_cnt];
				}
				key_gear_cnt = 0;
			}
			else
			{
				key_gear_cnt ++;
			}
							
			Turtle_LED_SetLumin(LED_CH1,hRov.led_lumin1);
			Turtle_LED_SetLumin(LED_CH2,hRov.led_lumin2);
		}
		else if (host[1].joy_stick[KID_RT] < 0x70)
		{
			//控制后灯亮度
//			if (host[1].joy_stick[KID_RIGHT] == 0x01)
//				hRov.led_lumin2 ++;
//			else if (host[1].joy_stick[KID_LEFT] == 0x01)
//				hRov.led_lumin2 ++;
//			Turtle_LED_SetLumin(LED_CH2,hRov.led_lumin2);
		}
		else if (host[1].joy_stick[KID_LB])
		{
			//云台及前摄像机调节
			hRov.yt_angle = Turtle_YT_GetAngle();
			if (host[1].joy_stick[KID_UP] == 0x01)
				Turtle_YT_MoveC(YT_UP);
			else if (host[1].joy_stick[KID_DOWN] == 0x01)
				Turtle_YT_MoveC(YT_DOWN);
			else
				Turtle_YT_MoveC(YT_MID);
			
			if (host[1].joy_stick[KID_RIGHT] == 0x01 && cam_run_state == CAM_STBY)
				cam_run_state = CAM_TELE;
			else if (host[1].joy_stick[KID_LEFT] == 0x01 && cam_run_state == CAM_STBY)
				cam_run_state = CAM_WIDE;
			else if (host[1].joy_stick[KID_RIGHT] == 0xFF && host[1].joy_stick[KID_LEFT] == 0xFF && cam_run_state == CAM_BUSY)
				cam_run_state = CAM_STOP;
		}
		else if (host[1].joy_stick[KID_LT] > 0x90)
		{
			//后摄像机调节（此处暂时为空）
		}
		else if (host[1].joy_stick[KID_START])
		{
			if (host[1].joy_stick[KID_BACK])
			{
				if (key_gmax_cnt > 3)
				{
					if (hRov.prop_cq_gmax == PROP_CQ_GMAX)
						hRov.prop_cq_gmax = PROP_CQ_MAX - PROP_CQ_MID;
					else
						hRov.prop_cq_gmax = PROP_CQ_GMAX;
					key_gmax_cnt = 0;
				}
			}
			else
				key_gmax_cnt ++;
		}
		else
		{
			//云台停止及其他需要复位的设备
			Turtle_YT_MoveC(YT_MID);
			if (cam_run_state == CAM_BUSY)
				cam_run_state = CAM_STOP;
			key_gmax_cnt ++;
			key_gear_cnt ++;
		}
#else
		//控制前灯亮度
		if (host[1].joy_stick[KID_RIGHT] == 0x01 && key_led_cnt>0x40)
		{
			hRov.led_lumin1 ++;
			hRov.led_lumin2 ++;
			if (hRov.led_lumin1>LED_PWM_MAX)
				hRov.led_lumin1=LED_PWM_MAX;
			if (hRov.led_lumin2>LED_PWM_MAX)
				hRov.led_lumin2=LED_PWM_MAX;
			key_led_cnt = 0;
			Turtle_LED_SetLumin(LED_CH1,hRov.led_lumin1);
			Turtle_LED_SetLumin(LED_CH2,hRov.led_lumin2);
		}
		else if (host[1].joy_stick[KID_LEFT] == 0x01 && key_led_cnt>0x40)
		{
			hRov.led_lumin1 --;
			hRov.led_lumin2 --;
			if	(hRov.led_lumin1<LED_PWM_MIN)
				hRov.led_lumin1=LED_PWM_MIN;
			if	(hRov.led_lumin2<LED_PWM_MIN)
				hRov.led_lumin2=LED_PWM_MIN;
			key_led_cnt = 0;
			Turtle_LED_SetLumin(LED_CH1,hRov.led_lumin1);
			Turtle_LED_SetLumin(LED_CH2,hRov.led_lumin2);
		}
		else
		{
			key_led_cnt ++;
		}
		
		//控制挡位
		static uint8_t gear_cnt = 1;
		if (host[1].joy_stick[KID_RSTICK] == 0x01)
		{
			if (key_gear_cnt > 3)
			{
				if (gear_cnt < 3)
					gear_cnt ++;
				
				host[1].prop_gear = gear_switch[gear_cnt];
			}
			key_gear_cnt = 0;
		}
		else if (host[1].joy_stick[KID_LSTICK] == 0x01)
		{
			if (key_gear_cnt > 3)
			{
				if (gear_cnt > 1)
					gear_cnt --;
				
				host[1].prop_gear = gear_switch[gear_cnt];
			}
			key_gear_cnt = 0;
		}
		else
		{
			key_gear_cnt ++;
		}


		//云台及前摄像机调节
		hRov.yt_angle = Turtle_YT_GetAngle();
		if (host[1].joy_stick[KID_UP] == 0x01)
		{
			Turtle_YT_MoveC(YT_UP);
		}
		else if (host[1].joy_stick[KID_DOWN] == 0x01)
		{
			Turtle_YT_MoveC(YT_DOWN);
		}
		else
		{
			Turtle_YT_MoveC(YT_MID);
		}
		
		if (host[1].joy_stick[KID_START] == 0x01 && cam_run_state == CAM_STBY)
			cam_run_state = CAM_TELE;
		else if (host[1].joy_stick[KID_BACK] == 0x01 && cam_run_state == CAM_STBY)
			cam_run_state = CAM_WIDE;
		else if (host[1].joy_stick[KID_START] == 0xFF && host[1].joy_stick[KID_BACK] == 0xFF && cam_run_state == CAM_BUSY)
			cam_run_state = CAM_STOP;
		else
		{
			if (cam_run_state == CAM_BUSY)
				cam_run_state = CAM_STOP;
		}

//		if (host[1].joy_stick[KID_START])
//		{
//			if (host[1].joy_stick[KID_BACK])
//			{
//				if (key_gmax_cnt > 3)
//				{
//					if (hRov.prop_cq_gmax == PROP_CQ_GMAX)
//						hRov.prop_cq_gmax = PROP_CQ_MAX - PROP_CQ_MID;
//					else
//						hRov.prop_cq_gmax = PROP_CQ_GMAX;
//					key_gmax_cnt = 0;
//				}
//			}
//			else
//				key_gmax_cnt ++;
//		}
#endif

		//低速任务调度
		switch (lstask_cnt)
		{
			case 0:
				//推进器运行状态检测
				Turtle_Prop_RunCheck();
#if PROP_FBD_EN == G_TURE
				Turtle_Prop_ErrPro();
#endif
				HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
				lstask_cnt ++;
				break;
			case 2:
				//温湿度传感器
				hSHT30.error = SHT3X_ReadMeasurementBuffer(&hSHT30.temperature, &hSHT30.humidity);
				if (hSHT30.temperature > TEMP_YC_GATE)
					hRov.run_state |= ROV_CTEMP_YC;
				else
					hRov.run_state &= ~ROV_CTEMP_YC;
				HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
				lstask_cnt ++;
				break;
			case 4:
				//漏水检测
				hRov.lsjc_state[0] = Turtle_LSJC(LSJC_CH1);
				hRov.lsjc_state[1] = Turtle_LSJC(LSJC_CH2);
				//漏水检测后续处理
				if (hRov.lsjc_state[0] == LSJC_LS)
					hRov.run_state |= ROV_BBOARD_LS;
				else
					hRov.run_state &= ~ROV_BBOARD_LS;
				
				if (hRov.lsjc_state[1] == LSJC_LS)
					hRov.run_state |= ROV_FBOARD_LS;
				else
					hRov.run_state &= ~ROV_FBOARD_LS;
				
				HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
				lstask_cnt ++;
				break;
			case 6:
				//电源模块温度查询
				Temp_Check();
				//电源模块温度处理
				if (huart[TEMP_UART_ID].rxFlag == 1)
				{
					int16_buf = Temp_DataPro(huart[TEMP_UART_ID].rxBuf);
					if (int16_buf != 0)
					{
						hRov.pwr_temp  = int16_buf;
						if (hRov.pwr_temp > TEMP_YC_GATE*10)
							hRov.run_state |= ROV_PTEMP_YC;
						else
							hRov.run_state &= ~ROV_PTEMP_YC;
					}
					huart[TEMP_UART_ID].rxFlag = 0;
				}
				lstask_cnt ++;
				break;
		}
		
		if (lstask_flag == 1)
		{
			if (cam_run_state == CAM_TELE)
			{
				Turtle_Cam_SetFocus(CAM_TELE);
				cam_run_state = CAM_BUSY;
			}
			else if (cam_run_state == CAM_WIDE)
			{
				Turtle_Cam_SetFocus(CAM_WIDE);
				cam_run_state = CAM_BUSY;
			}
			else if (cam_run_state == CAM_STOP)
			{
				Turtle_Cam_SetFocus(CAM_STOP);
				cam_run_state = CAM_STBY;
			}
			else if (cam_run_state == CAM_STBY)
				OSD_ReFresh();
			
			lstask_flag = 0;
		}
				
		//Shell 任务
		shellTask(&shell);
		
		//HAL_IWDG_Refresh(&hiwdg);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
