/**
  ******************************************************************************
  * @file    de_usr_conf.h
  * @author  XuczSnow, OUC/Fab U+
  * @brief   经典差分进化算法用户配置文件
  *
  @verbatim

  @endverbatim
  ******************************************************************************
  @attention

  MIT License

  Copyright (c) 2022 XuczSnow

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ******************************************************************************
  */
#ifndef DE_USR_CONF_H
#define DE_USR_CONF_H

#include "de_type.h"

#define USR_MODE MODE_DMRDT

#define   NVAR                2u        /*计算维度*/
#define   NPOP                20u       /*种群数量*/
#define   ITER                1000u      /*迭代次数*/

#define   OUT_DEMENSION       8u        /*输出维度*/
#define   IN_DEMENSION        6u        /*输入维度*/

#define   FITNESS_Q           0.1f      /*比重系数*/

#endif
