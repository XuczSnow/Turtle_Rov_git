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

#include "FreeRTOS.h"
#include "task.h"
#include "shell.h"
#include "serial.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "cevent.h"
#include "log.h"


Shell shell;
char shellBuffer[512];

static SemaphoreHandle_t shellMutex;

/**
 * @brief 鐢ㄦ埛shell鍐? * 
 * @param data 鏁版嵁
 * @param len 鏁版嵁闀垮害
 * 
 * @return short 瀹为台鍐椤叆镄勬暟鎹昵搴? */
short userShellWrite(char *data, unsigned short len)
{
    serialTransmit(&debugSerial, (uint8_t *)data, len, 0x1FF);
    return len;
}


/**
 * @brief 鐢ㄦ埛shell璇? * 
 * @param data 鏁版嵁
 * @param len 鏁版嵁闀垮害
 * 
 * @return short 瀹为台璇诲彇鍒? */
short userShellRead(char *data, unsigned short len)
{
    return serialReceive(&debugSerial, (uint8_t *)data, len, 0);
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
    xSemaphoreTakeRecursive(shellMutex, portMAX_DELAY);
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
    xSemaphoreGiveRecursive(shellMutex);
    return 0;
}

/**
 * @brief 鐢ㄦ埛shell鍒濆鍖? * 
 */
void userShellInit(void)
{
    shellMutex = xSemaphoreCreateMutex();

    shell.write = userShellWrite;
    shell.read = userShellRead;
    shell.lock = userShellLock;
    shell.unlock = userShellUnlock;
    shellInit(&shell, shellBuffer, 512);
    if (xTaskCreate(shellTask, "shell", 256, &shell, 5, NULL) != pdPASS)
    {
        logError("shell task creat failed");
    }
}
CEVENT_EXPORT(EVENT_INIT_STAGE2, userShellInit);

