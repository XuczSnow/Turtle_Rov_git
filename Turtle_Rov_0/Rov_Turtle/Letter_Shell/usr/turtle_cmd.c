/**
  ******************************************************************************
  * @file    turtle_cmd.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列机器人shell指令及函数定义
  *
  ******************************************************************************
  * @attention
  *	
  * 
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"
#include "stdlib.h"
#include "string.h"

/****************基础函数定义****************/

char wait_char[5] = {'\\','|','/','-','.'};

void Shell_PutStr(char* data, uint16_t len)
{
	HAL_UART_Transmit(&HOST_UART,(uint8_t*)data,len,0xFF);
}

void Turtle_Shell_Wait(uint8_t times)
{
	uint8_t	i;
	uint8_t	j;
	
	for (j=0;j<times;j++)
	{
		Shell_PutStr(".",1u);
		for (i=0;i<5;i++)
		{
			Shell_PutStr("\b",1u);
			HAL_Delay(200);
			Shell_PutStr(&wait_char[i],1u);
		}
	}
	Shell_PutStr("\r\n",2);
}

void Turtle_Shell_ArgErr(void)
{
	printf("Input parameter error !\r\n");
}

void Turtle_Shell_CmdErr(void)
{
	printf("Command not found !\r\n");
}

/****************Shell函数****************/

int Turtle_HelloWorld(int argc, char *argv[])
{
	Shell_PutStr(turtle_start_info,turtle_start_infolen);
	return NO_ERR;
}

char download_msg1[] = "Enter Download Mode, Please Waiting!\r\n";
char download_msg2[] = "Enter Download Mode, Check Your Programmer!\r\n";

int Turtle_DownloadProgram(int argc, char *argv[])
{
	uint16_t flash_buf = 0x0001;
	
	Shell_PutStr(download_msg1,sizeof(download_msg1));
	FLASH_Write(JUMP_BOOTLOADER_ADD,&flash_buf,2u);
	Turtle_Shell_Wait(5);
	Shell_PutStr(download_msg2,sizeof(download_msg2));
	Soft_Reset();
	
	return NO_ERR;
}

char reset_info[] = "The system will restart in 10 seconds!\r\n";

int Turtle_RovReset(int argc, char *argv[])
{
	Shell_PutStr(reset_info,sizeof(reset_info));
	Turtle_Shell_Wait(10);
	Soft_Reset();
	
	return NO_ERR;
}

#define AUTO_FLAG_L 2u
#define AUTO_MODE_L 3u
#define AUTO_OUT_L  2u

char *auto_flag_c[AUTO_FLAG_L]	= {"AUTO_DEPTH","AUTO_HEADING"};
char *auto_mode_c[AUTO_MODE_L]	= {"MODE_PID","MODE_FOPID","MODE_CCPID"};
char *auto_out_c[AUTO_OUT_L]		= {"OUT_ORG","OUT_DC"};

int Turtle_Enable_Auto(int argc, char *argv[])
{
	uint8_t auto_flag = 4;
	uint8_t auto_en = 0;
	
	if (argc < 3 || argc > 6)
	{
		Turtle_Shell_ArgErr();
		return ARG_ERR;
	}
	
	for (uint8_t i=0;i<AUTO_FLAG_L;i++)
	{
		if(strcmp(auto_flag_c[i],argv[1]) == 0)
		{
			auto_flag = i;
			break;
		}
	}
	
	if (auto_flag == 4)
	{
		Turtle_Shell_ArgErr();
		return ARG_ERR;
	}
	
	auto_en = atoi(argv[2]);
	
	hAuto[auto_flag].pid_setpoint = atof(argv[3])*10;
	
	printf("Set %s at %s",argv[1],argv[3]);
	
	switch(argc)
	{
		case 6:
			for (uint8_t i=0;i<AUTO_OUT_L;i++)
				if(strcmp(auto_out_c[i],argv[5]) == 0)
				{
					hAuto[auto_flag].out_mode = OUT_ORG|(uint32_t)i;
					printf(", out mode: %s",argv[5]);
				}
		case 5:
			for (uint8_t i=0;i<AUTO_MODE_L;i++)
				if(strcmp(auto_mode_c[i],argv[4]) == 0)
				{
					hAuto[auto_flag].pid_mode = MODE_PID|(uint32_t)i;
					printf(", mode: %s",argv[4]);
				}
			for (uint8_t i=0;i<AUTO_OUT_L;i++)
				if(strcmp(auto_out_c[i],argv[4]) == 0)
				{
					hAuto[auto_flag].out_mode = OUT_ORG|(uint32_t)i;
					printf(", out mode: %s",argv[4]);
				}
				break;
	}
	
	if (auto_en == 1)
	{
		host[1].auto_flag |= 1<<auto_flag;
		printf("%s%s%s%s\r\n",", ",RED,"enbale",NONE);
	}
	else
	{
		host[1].auto_flag &= ~(1<<auto_flag);
		printf("%s%s%s%s\r\n",", ",GREEN,"disbale",NONE);
	}
	
	return NO_ERR;
}

char *auto_param_flag[5] = {"AUTO_DEPTH", "AUTO_HEADING", "AUTO_PITCH", "CC_PID_OUT", "CC_PID_IN"};
int Turtle_Auto_Param(int argc, char *argv[])
{
  uint8_t auto_flag = 0;
  TurtlePid_Type *pAuto = NULL;

  if (argc != 7)
	{
		Turtle_Shell_ArgErr();
		return ARG_ERR;
	}

  for (uint8_t i=0;i<AUTO_FLAG_L;i++)
	{
		if(strcmp(auto_flag_c[i],argv[i]) == 0)
		{
			auto_flag = i;
			break;
		}
	}

  if (auto_flag < 3)
    pAuto = &hAuto[auto_flag];
  else if (auto_flag < 5)
    pAuto = &hDepthAuto[auto_flag-3];
  else
    return ARG_ERR;

  pAuto->pid_Kp = atof(argv[2]);
  pAuto->pid_Ki = atof(argv[3]);
  pAuto->pid_Kd = atof(argv[4]);
  pAuto->fopid_lam = atof(argv[5]);
  pAuto->fopid_u = atof(argv[6]);

  if (pAuto->pid_mode == MODE_FOPID)
    Turtle_FopidInit(pAuto);

  printf("%s%s%s%s%s%s\r\n","Set ",auto_param_flag[auto_flag]," param ",GREEN,"OK!",NONE);
  return NO_ERR;
}

char *prop_ctrl_c[8]	= {"PROP1","PROP2","PROP3","PROP4","PROP5","PROP6","PROP7","PROP8"};

int Turtle_SProp_Ctrl(int argc, char *argv[])
{
	for(uint8_t i=0;i<8;i++)
	{
		if (strcmp(prop_ctrl_c[i],argv[1]) == 0)
		{
			if (atoi(argv[2]))
			{
				hprop[i].Prop_CQ = 1500 + atoi(argv[3]);
			}
			else
			{
				hprop[i].Prop_CQ = 1500;
			}
		}
	}
	return 0;
}

int Turrle_AutoAt_En(int argc, char *argv[])
{
	if (argc == 2)
	{
		if (atoi(argv[1]) == 1)
			at_en = 1;
		else
			at_en = 0;
		printf ("Set Auto Attitude as %d\r\n",atoi(argv[1]));
	}
	else
	{
		Turtle_Shell_ArgErr();
		return ARG_ERR;
	}
	return 0;
}

int Turrle_SetDCValue(int argc, char *argv[])
{
	if (argc == 2)
	{
		if (atoi(argv[1]) > 1)
			depth_dc = (float)atoi(argv[1]);
		printf ("Set Dynamic Compensation Value as %f\r\n", depth_dc);
	}
	else
	{
		Turtle_Shell_ArgErr();
		return ARG_ERR;
	}
	return 0;
}

int Turrle_SetAutoCycle(int argc, char *argv[])
{
	if (argc == 2)
	{
		if (atoi(argv[1]) > 1)
			auto_cycle = atoi(argv[1])/10;
		printf ("Set Control Cycle Value as %dms\r\n",auto_cycle*10);
	}
	else
	{
		Turtle_Shell_ArgErr();
		return ARG_ERR;
	}
	return 0;
}

int Turrle_SetPressLPTk(int argc, char *argv[])
{
	float tmp;
	if (argc == 2)
	{
		tmp = atof(argv[1]);
		if (tmp <= 1 && tmp > 0)
			lpt_k = tmp;
		printf ("Set LPT Gain as %.1f\r\n",tmp);
	}
	else
	{
		Turtle_Shell_ArgErr();
		return ARG_ERR;
	}
	return 0;
}

/****************Shell列表****************/

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), t_hello, Turtle_HelloWorld, turtle hello world);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), t_download, Turtle_DownloadProgram, jump to system bootloader);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), t_reset, Turtle_RovReset, reset rov);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), t_auto, Turtle_Enable_Auto, set rov aoto control);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), t_auto_param, Turtle_Auto_Param, set rov aoto control parameter);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), t_prop, Turtle_SProp_Ctrl, coontrol single propeller);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), t_autoAttitude_en, Turrle_AutoAt_En, set attitude coontrol en);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), t_autoSetDc_value, Turrle_SetDCValue, set dynamic compensation value);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), t_autoSet_cycle, Turrle_SetAutoCycle, set automatic control cycle);

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), t_pressSet_lptk, Turrle_SetPressLPTk, set low pass filter coefficient);