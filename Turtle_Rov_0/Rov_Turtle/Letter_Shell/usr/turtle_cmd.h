/**
  ******************************************************************************
  * @file    turtle_cmd.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列机器人shell指令及函数头文件
  *
  ******************************************************************************
  * @attention
  *	
  * 
  ******************************************************************************
  */
	
#ifndef TURTLE_CMD_H
#define TURTLE_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

typedef enum
{
  NO_ERR = 0,
	ARG_ERR
}Shell_ErrDef;


/********VT100终端指令相关定义********/

//VT100 颜色定义
#define NONE          "\033[m"
#define RED           "\033[0;32;31m"
#define LIGHT_RED     "\033[1;31m"
#define GREEN         "\033[0;32;32m"
#define LIGHT_GREEN   "\033[1;32m"
#define BLUE          "\033[0;32;34m"
#define LIGHT_BLUE    "\033[1;34m"
#define DARK_GRAY     "\033[1;30m"
#define CYAN          "\033[0;36m"
#define LIGHT_CYAN    "\033[1;36m"
#define PURPLE        "\033[0;35m"
#define LIGHT_PURPLE  "\033[1;35m"
#define BROWN         "\033[0;33m"
#define YELLOW        "\033[1;33m"
#define LIGHT_GRAY    "\033[0;37m"
#define WHITE         "\033[1;37m"

//VT100 控制指令定义
#define CLEAR_ALL			"\033[0m" 			//关闭所有属性
#define SET_HIGHLIGHT	"\033[1m"				//设置高亮度
#define UNDER_LINE    "\033[4m"				//下划线
#define TWINKLE				"\033[5m"				//闪烁
#define INVERSE       "\033[7m"				//反显
#define VANISH				"\033[8m"				//消隐
				//设置前景色
#define FG_BLACK			"\033[30m"
#define FG_RED				"\033[31m"
#define FG_GREEN			"\033[32m"
#define FG_YELLOW			"\033[33m"
#define FG_BLUE				"\033[34m"
#define FG_PURPLE			"\033[35m"
#define FG_BGREEN			"\033[36m"
#define FG_WHITE			"\033[37m"
				//设置背景色
#define BG_BLACK			"\033[40m"
#define BG_RED				"\033[41m"
#define BG_GREEN			"\033[42m"
#define BG_YELLOW			"\033[43m"
#define BG_BLUE				"\033[44m"
#define BG_PURPLE			"\033[45m"
#define BG_DGREEN			"\033[46m"
#define BG_WHITE			"\033[47m"
				//鼠标移动
				/*
				\033[nA       光标上移 n 行
				\033[nB       光标下移 n 行
				\033[nC       光标右移 n 行
				\033[nD       光标左移 n 行
				\033[y;xH     设置光标位置(x,y)
				*/
#define MOUSE_MOVE		"\033["					//鼠标移动前缀
#define MOUSE_UP		  "A"
#define MOUSE_DOWN		"B"
#define MOUSE_RIGHT		"C"
#define MOUSE_LEFT		"D"
#define MOUSE_POS			"H"

#define DISPLAY_CLR		"\033[2J"				//清屏
#define DISPLAY_LCLR  "\033[K"				//清除从光标到行尾的内容
#define MOUSE_SPOS		"\033[s"				//保存光标位置
#define MOUSE_RPOS		"\033[u"				//恢复光标位置
#define MOUSE_HIDE		"\033[?25l"			//隐藏光标
#define MOUSE_DISPLAY	"\033[?25h"			//显示光标

void Shell_PutStr(char* data, uint16_t len);

#endif
