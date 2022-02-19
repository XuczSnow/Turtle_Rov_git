/**
  ******************************************************************************
  * @file    shell_port.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Letter SHELL 瀹樻柟鏂逛緥绋嬩腑 shell_port.c 鏂囦欢绉绘
  *
  ******************************************************************************
  * @attention
  *	浣跨敤HAL搴?  * 
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
 * @brief 鐢ㄦ埛shell鍐? * 
 * @param data 鏁版嵁
 * @param len 鏁版嵁闀垮害
 * 
 * @return short 瀹为台鍐椤叆镄勬暟鎹昵搴? */
short userShellWrite(char *data, unsigned short len)
{
    HAL_UART_Transmit(&HOST_UART,(uint8_t *)data,len,0xff);
    return len;
}


/**
 * @brief 鐢ㄦ埛shell璇? * 
 * @param data 鏁版嵁
 * @param len 鏁版嵁闀垮害
 * 
 * @return short 瀹为台璇诲彇鍒扮殑鏁版嵁闀垮害
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
 * @brief 鐢ㄦ埛shell涓婇挛
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
 * @brief 鐢ㄦ埛shell瑙ｉ挛
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
 * @brief 鐢ㄦ埛shell鍒濆鍖? * 
 */
void userShellInit(void)
{

    shell.write = userShellWrite;
    shell.read = userShellRead;
//    shell.lock = userShellLock;
//    shell.unlock = userShellUnlock;
    shellInit(&shell, shellBuffer, 512);
	
}
