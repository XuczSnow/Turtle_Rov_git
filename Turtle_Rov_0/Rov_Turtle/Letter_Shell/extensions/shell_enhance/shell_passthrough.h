/**
 * @file shell_passthrough.h
 * @author Letter(nevermindzzt@gmail.com)
 * @brief shell passthrough mode
 * @version 0.1
 * @date 2021-05-31
 * 
 * @copyright (c) 2021 Letter
 * 
 */
#ifndef __SHELL_PASSTHROUGH_H__
#define __SHELL_PASSTHROUGH_H__

#include "shell.h"

/**
 * @brief 阃€鍑?passthrough 妯″纺镄勬寜阌敭链硷紝鍗曞瓧鑺? */
#define     SHELL_PASSTHROUGH_EXIT_KEY          0x04    // Ctrl + D

/**
 * @brief passthrough 妯″纺 handler 鍑芥暟铡熷瀷
 * 
 * @param data 鏁版嵁
 * @param len 鏁版嵁闀垮害
 * 
 * @return int 杩斿洖链? */
typedef int (*ShellPassthrough)(char *data, unsigned short len);

/**
 * @brief shell passthrough 妯″纺鍑芥暟鍚? */
#define SHELL_PASSTROUGH_FUNC_NAME(_name)       agency##_name

/**
 * @brief shell passthrough 妯″纺鍑芥暟瀹氢箟
 * 
 * @param _name 锻戒护鍚? * @param _prompt passthrough 妯″纺鎻愮ず绗? * @param _handler passthrough 妯″纺 handler
 */
#define SHELL_PASSTROUGH_FUNC(_name, _prompt, _handler) \
        void SHELL_PASSTROUGH_FUNC_NAME(_name)(int p1, int p2) \
        { shellPassthrough(shellGetCurrent(), #_prompt, _handler, p1, (void *)p2); }

/**
 * @brief shell passthrouh 瀹氢箟
 * 
 * @param _attr 灞炴€? * @param _name passthrough 锻戒护鍚? * @param _prompt passthrough 鎻愮ず绗? * @param _handler passthrough handler
 * @param _desc passthrough 鎻忚堪
 */
#define SHELL_EXPORT_PASSTROUGH(_attr, _name, _prompt, _handler, _desc) \
        SHELL_PASSTROUGH_FUNC(_name, _prompt, _handler) \
        SHELL_EXPORT_CMD(_attr|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), _name, SHELL_PASSTROUGH_FUNC_NAME(_name), _desc)

unsigned int shellPassthrough(Shell *shell, const char *prompt, ShellPassthrough handler, int argc, char *argv[]);

#endif