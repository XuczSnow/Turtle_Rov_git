﻿/**
 * @file shell_ext.h
 * @author Letter (NevermindZZT@gmail.com)
 * @brief shell extensions
 * @version 3.0.0
 * @date 2019-12-31
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#ifndef __SHELL_EXT_H__
#define __SHELL_EXT_H__

#include "shell.h"

/**
 * @brief 鏁板瓧绫诲瀷
 * 
 */
typedef enum
{
    NUM_TYPE_DEC,                                           /**< 鍗佽繘鍒舵暣鍨?*/
    NUM_TYPE_BIN,                                           /**< 浜岃繘鍒舵暣鍨?*/
    NUM_TYPE_OCT,                                           /**< 鍏繘鍒舵暣鍨?*/
    NUM_TYPE_HEX,                                           /**< 鍗佸叚杩涘埗鏁村瀷 */
    NUM_TYPE_FLOAT                                          /**< 娴偣鍨?*/
} ShellNumType;

unsigned int shellExtParsePara(Shell *shell, char *string);
int shellExtRun(Shell *shell, ShellCommand *command, int argc, char *argv[]);

#endif
