/**
  ******************************************************************************
  * @file    shell_port.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Letter SHELL 官方方例程中 shell_port.h 文件移植
  *
  ******************************************************************************
  * @attention
  *	使用HAL库
  * 
  ******************************************************************************
  */


/**
 * @file shell_port.h
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#ifndef __SHELL_PORT_H__
#define __SHELL_PORT_H__

#include "turtle_g.h"
#include "shell.h"

extern Shell shell;

void userShellInit(void);

#endif
