/**
  ******************************************************************************
  * @file    turtle_g.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 全局头文件
	*          包含以下定义：
	*          + 
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
#ifndef TURTLE_G_H
#define TURTLE_G_H

/* STM32 Includes ------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
/* KEIL5 Includes ------------------------------------------------------------*/
#include "EventRecorder.h"
#include "stdio.h"
/* Exlib Includes ------------------------------------------------------------*/
#include "crc16_lib.h"
#include "sht_typedefs.h"
#include "onchip_flash.h"

#include "shell_port.h"
#include "turtle_cmd.h"

/** @defgroup 全局相关定义
  * @{
  */
#define G_TURE				1u
#define G_FALSE				0u

#define TIMESTAMP_EN	G_TURE
#define PROP_FBD_EN		G_TURE

//以下暂未使用
#define PROP_485			0u
#define PROP_PWM			1u

#define PROP_MODE			PROP_PWM

//选择电子罗盘或者陀螺仪
#define G_GYRO				0u
#define G_COMP				1u

#define G_ANGLE				G_COMP

//推进器布局
#define G_PROP_N			0u
#define G_PROP_P			1u

#define G_PROP				G_PROP_N


/** @defgroup FIFO相关定义
  * @{
  */

#define ElemType       uint8_t
#define QueueSize      64					//FIFO队列的大小
#define QueueFull      1					//FIFO满置1
#define QueueEmpty     0					//FIFO空置0
#define QueueOperateOk 2					//队列操作完成 赋值为2

typedef struct FifoQueue
{
    uint16_t front;			//队列头
    uint16_t rear;			//队列尾
    uint16_t count;			//队列计数
    ElemType dat[QueueSize];
}FifoQueueTypedef;

extern FifoQueueTypedef Terminal_RxQueue;

/** @defgroup 串口相关定义
  * @{
  */

#define HOST_UART			huart1
#define DPH_UART			huart2
#define CAM_UART			DPH_UART
#define PROP_UART			huart3
#define GYRO_UART			huart4
#define COMP_UART			GYRO_UART
#define TEMP_UART			huart5

#define UART1_ID			0u
#define UART2_ID			1u
#define UART3_ID			2u
#define UART4_ID			3u
#define UART5_ID			4u

#define RX_DATA_SIZE  64u
#define TX_DATA_SIZE  64u

typedef enum
{
  HOST_UART_ID = UART1_ID,
  DPH_UART_ID,
	PROP_UART_ID,
	GYRO_UART_ID,
	TEMP_UART_ID
}UART_IDTypeDef;

#define CAM_UART_ID	DPH_UART_ID
#define COMP_UART_ID GYRO_UART_ID

typedef struct
{
	uint8_t                  rxBuf[RX_DATA_SIZE];
	uint16_t                 rxBufCount;
	uint8_t                  rxFlag;
	uint8_t									 txBuf[TX_DATA_SIZE];
	uint16_t                 txBufCount;
	uint8_t                  txFlag;
}UART_DataTypeDef;

extern UART_DataTypeDef huart[5];

/** @defgroup 定时器相关定义
  * @{
  */

#define LED1_2_TIM			htim1
#define YT_TIM					htim2
#define PROP1_4_TIM			htim3
#define PROP5_8_TIM			htim4
#define PROPC_TIM				htim5
#define ACTRL_TIM				htim6
#define LSTASK_TIM			htim7
#define HSTASK_TIM			htim8

#define LED_CH1					TIM_CHANNEL_1
#define LED_CH2					TIM_CHANNEL_4

#define YT_CH						TIM_CHANNEL_2

#define PROP_CH1				TIM_CHANNEL_1
#define PROP_CH2				TIM_CHANNEL_2
#define PROP_CH3				TIM_CHANNEL_3
#define PROP_CH4				TIM_CHANNEL_4
#define PROP_CH5				TIM_CHANNEL_1
#define PROP_CH6				TIM_CHANNEL_2
#define PROP_CH7				TIM_CHANNEL_3
#define PROP_CH8				TIM_CHANNEL_4


/** @defgroup 推进器相关定义
  * @{
  */
	
#define		PROPL_RF		0u
#define		PROPL_RB		1u
#define		PROPL_LB		2u
#define		PROPL_LF		3u
#define		PROPV_RF		4u
#define		PROPV_RB		5u
#define		PROPV_LB		6u
#define		PROPV_LF		7u

#define		PROP_ID1		PROPL_RF
#define		PROP_ID2		PROPL_RB
#define		PROP_ID3		PROPL_LB
#define		PROP_ID4		PROPL_LF
#define		PROP_ID5		PROPV_RF
#define		PROP_ID6		PROPV_RB
#define		PROP_ID7		PROPV_LB
#define		PROP_ID8		PROPV_LF

//#define		PROP_VAL_INTER	2.8				//（最大PWM值-最小PWM值）/（0xFF）
#define 	PROP_CQ_MAX			1850
#define 	PROP_CQ_MID			1500
#define 	PROP_CQ_MIN			1150
#define 	PROP_CQ_GATE		8

#define		PROP_RSP_MAX		3200

#define 	PROP_RSP_GMAX		5000
#define 	PROP_RSP_GATE		1000
#define 	PROP_RSP_GMIN		100

#define 	PROP_ERR_MAX		10u

typedef enum
{
  PROP_READY = 0x01,
  PROP_BUSY = 0x02,
	PROP_FBD = 0x04,
	PROP_ERR = 0x08,
	PROP_RERR = 0x10,
}Prop_StateTypeDef;

typedef struct
{
	uint8_t                  Prop_ID;				//推进器编号
	uint16_t                 Prop_CQ;				//推进器控制量
	uint16_t                 Prop_OCQ;			//推进器旧控制量
	uint8_t                  Prop_Vot;			//推进器电压 0.1V
	uint8_t                  Prop_Cur;			//推进器电流 0.1A
	uint16_t								 Prop_RSp;			//推进器转速 r/min
	int16_t									 Prop_DRSp;			//推进器转速 r/min
	uint8_t									 Prop_Gear;			//推进器挡位（未使用）
	Prop_StateTypeDef				 Prop_State;		//推进器运行状态
}Prop_CtrlTypeDef;

extern Prop_CtrlTypeDef hprop[8];

/** @defgroup 漏水检测相关定义
  * @{
  */

#define LSJC_CH1		1u
#define LSJC_CH2		2u

typedef enum
{
  LSJC_NORM = 0,
	LSJC_LS,
	LSJC_NONE
}LSJC_StateTypeDef;

/** @defgroup 摄像机相关定义
  * @{
  */
typedef enum
{
	CAM_STOP = 0x00,
	CAM_STBY = 0x05,
	CAM_BUSY = 0x06,
	CAM_TELE = 0x23,
	CAM_WIDE = 0x33
}Cam_RunStateTypeDef;

extern Cam_RunStateTypeDef cam_run_state;

/** @defgroup 压力传感器相关定义
  * @{
  */
typedef struct
{
	uint16_t		init_press;					//初始压力
	uint16_t		press;							//mbar
	int16_t			temp;								//0.01°
	uint16_t		depth;							//cm
	uint16_t		conv_depth[10];			//换算深度 cm
	float				conv_speed[10];			//换算速度 mm/s
}Press_StateTypeDef;

extern Press_StateTypeDef hPress;

/** @defgroup 上下位机相关定义
  * @{
  */

//运行状态定义
#define ROV_TURN_RIGHT		0x000200
#define ROV_TURN_LEFT			0x000100
#define ROV_AUTO_DEEP			0x000080
#define ROV_AUTO_HEADING	0x000040
#define ROV_AUTO_PITCH		0x000020
#define ROV_RUN_UP				0x000010
#define ROV_RUN_DOWN			0x000008
#define ROV_RUN_FORWARD		0x000004
#define ROV_RUN_BACK			0x000002

#define ROV_CTEMP_YC			0x008000
#define ROV_PTEMP_YC			0x004000
#define ROV_FBOARD_LS			0x002000
#define ROV_BBOARD_LS			0x001000

#define TEMP_YC_GATE			60.0f

#define KEY_MIN_GATE			5u
#define KEY_MAX_GATE			16
#define KEY_AIR_K					2u

#define KEY_MIN						0x00
#define KEY_MID						0x80
#define KEY_MAX						0xFF

typedef enum
{
  KID_RSTICK_FB = 0,
	KID_RSTICK_RL,
	KID_LSTICK_FB,
	KID_LSTICK_RL,
	KID_UP,
	KID_DOWN,
	KID_LEFT,
	KID_RIGHT,
	KID_RB,
	KID_LB,
	KID_RT,
	KID_LT,
	KID_X,
	KID_Y,
	KID_A,
	KID_B,
	KID_START,
	KID_BACK,
	KID_RSTICK,
	KID_LSTICK,
}JOY_KidTypeDef;


typedef struct
{
	int16_t							pwr_temp;			//0.1度
	int16_t							yt_angle;			//0.1度
	uint8_t							comp_id;			//电子罗盘ID
	float								roll;					//度
	float								pitch;				//度
	float								yaw;					//度
	float								gyro_t;				//度(暂未使用)
	uint16_t						led_lumin1;		//PWM值
	uint16_t						led_lumin2;		//PWM值
	uint16_t						voltage;			//电压 0.1V
	uint16_t						current;			//电流 0.1A
	uint32_t						run_state;
	LSJC_StateTypeDef		lsjc_state[2];
	int16_t							prop_cq_gmax;
}Rov_StateTypeDef;

typedef struct
{
	uint8_t			joy_stick[20];
	uint8_t			fpid_change;
	uint8_t			fpid_check;
	uint8_t			auto_flag;
	float				prop_gear;
	uint8_t			fsi_change;
	uint8_t			fsi_check;
	uint8_t			si_flag;
	uint8_t			err_cnt;
}Host_StateTypeDef;

extern float gear_switch[4];
extern Rov_StateTypeDef		hRov;
extern Host_StateTypeDef 	host[2];


/** @defgroup 系统辨识相关定义
  * @{
  */

#define SI_ENABLE			1u
#define SI_DISABLE		0u

typedef enum
{
  SI_SIN = 0,
}SI_OutTypeDef;

typedef enum
{
  SI_DEPTH = 0x01,
  SI_HEADING = 0x02,
	SI_PITCH = 0x04,
	SI_ROLL = 0x08
}SI_ModeTypeDef;

typedef struct
{
	float											period;			//周期
	float											amp;				//幅值
	float											si_out;			//SI输出
	uint8_t										enable;			//SI使能
	SI_ModeTypeDef						mode;				//SI模式
	SI_OutTypeDef							si_outmode;	//SI输出模式
}SI_StateTypeDef;

extern SI_StateTypeDef hSI[4];

/* Turtle Includes -----------------------------------------------------------*/
#include "turtle_it.h"
#include "turtle_bsp.h"
#include "turtle_prop_485&pwm.h"
#include "turtle_sht_30dis.h"
#include "turtle_gyro_jy901b.h"
#include "turtle_cam_hk.h"
#include "turtle_led_j101.h"
#include "turtle_lsjc_gpio.h"
#include "turtle_press_x101.h"
#include "turtle_press_rovmaker.h"
#include "turtle_temp_nta8a01_ttl.h"
#include "turtle_yt_j101.h"
#include "turtle_comp_hcm365b.h"
#include "turtle_host.h"

#include "turtle_autoctrl.h"
#include "turtle_movectrl.h"
#include "turtle_timit.h"
#include "turtle_dbg.h"
#include "turtle_fifo.h"
#include "turtle_parameters.h"

#if TIMESTAMP_EN == G_TURE
	#include "turtle_dbg.h"
#endif

#endif
