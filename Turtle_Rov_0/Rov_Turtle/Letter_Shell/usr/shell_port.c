/**
  ******************************************************************************
  * @file    shell_port.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Letter SHELL 官方方例程中 shell_port.c 文件移植
  *
  ******************************************************************************
  * @attention
  *	使用HAL库
  * 
  ******************************************************************************
  */


/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */
#include "turtle_g.h"
#include "shell.h"
#include "log.h"

Shell shell;
char shellBuffer[512];
char readBuffer[32];

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
    HAL_UART_Transmit(&HOST_UART,(uint8_t *)data,len,0xff);
    return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到的数据长度
 */
short userShellRead(char *data, unsigned short len)
{
		static unsigned short read_cnt = 0;
	  static uint8_t res=0;
	  for (read_cnt=0; read_cnt<len; read_cnt ++)
		{
			res = QueueOut(&Terminal_RxQueue,(uint8_t *)&data[read_cnt]);
			if (res == QueueEmpty)
				break;
		}
    return read_cnt;
}

/**
 * @brief 用户shell上锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellLock(Shell *shell)
{
    return 0;
}

/**
 * @brief 用户shell解锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellUnlock(Shell *shell)
{
    return 0;
}

int func(int argc, char *argv[])
{
    printf("%dparameter(s)\r\n", argc);
    for (char i = 1; i < argc; i++)
    {
        printf("%s\r\n", argv[i]);
    }
		return 0;
}

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), func, func, test);

/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void)
{

    shell.write = userShellWrite;
    shell.read = userShellRead;
//    shell.lock = userShellLock;
//    shell.unlock = userShellUnlock;
    shellInit(&shell, shellBuffer, 512);
	
}
