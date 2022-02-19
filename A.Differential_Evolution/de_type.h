/**
  ******************************************************************************
  * @file    de_type.h
  * @author  XuczSnow, OUC/Fab U+
  * @brief   经典差分进化算法数据定义
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
#ifndef CLASSICAL_DE_TYPE_H
#define CLASSICAL_DE_TYPE_H

#include "de_usr_conf.h"

typedef enum    DeMode        DeMode_Type;
typedef struct  Individual    Individual_Type;
typedef struct  DeAlgorithm   DeAlgorithm_Type;
typedef float   (*DeFitFunc_Ptr)(float *vector, float *input, DeMode_Type mode);
typedef float   (*DeEndFunc_Ptr)(float *input);

enum DeMode{
  MODE_CLSDE    = 0x10030000,
  MODE_ADTDE    = 0x10030001,
  MODE_DMRDT    = 0x10030002,
};

struct Individual{
  float vector[NVAR];
  float fitness;
};

struct DeAlgorithm{
  Individual_Type pop[NPOP];
  DeFitFunc_Ptr   DeFitFunc;
  DeEndFunc_Ptr   DeEndFunc;
  float de_f;
  float de_cr;
  float global_fitness;
  float global_solution[NVAR];
  float pop_min[NPOP];
  float pop_max[NPOP];
  /*一些需要记录的缓存*/
  float log_fitness[ITER];
  int   log_iter;
};

#endif
