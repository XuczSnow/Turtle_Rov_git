/**
  ******************************************************************************
  * @file    de_fitness.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   差分进化算法适应度函数实现
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

#include "de_fitness.h"

#include "stdint.h"

const float c[6]      ={0,0,0,0,0,0};

const float M1[6][2]  ={{-0.034, -0.2},
                        { 0.094, -0.5},
                        {-0.358,  0  },
                        {-0.94 ,  0  },
                        {-0.34 ,  0  },
                        { 0,      1  }};

const float M2[8][6]  ={{ 0.0},  /*占位，为了计算方便*/
                        { 0,      0,      1.3966,  0,       1.4706,  0  },
                        { 0,      0,      1.3966,  0.5319,  0,       0  },
                        { 0,      0,      0,       0.5319,  1.4706,  0  },
                        { 0.0},
                        { 0,     -1,      0,      -0.1,     0,       0.5},
                        {-2.5,    0,      0,       0,       0.25,    0.5},
                        { 2.5,    1,      0,       0.1,    -0.25,    0  }};

const float M[6][8]   ={{-0.034,  0.034, -0.034,   0.034,  -0.2,     0.2,   -0.2,    0.2},
                        { 0.094,  0.094, -0.094,  -0.094,  -0.5,    -0.5,    0.5,    0.5},
                        {-0.358,  0.358,  0.358,  -0.358,   0,       0,      0,      0  },
                        {-0.94,  -0.94,   0.94,    0.94,    0,       0,      0,      0  },
                        {-0.34,   0.34,  -0.34,    0.34,    0,       0,      0,      0  },
                        { 0,      0,      0,       0,       1,       1,      1,      1  }};

const float w2 = 0;
const float M_Norm = 2;
static float ex_vector[OUT_DEMENSION] = { 0.0 };

float vec_temp[OUT_DEMENSION] = { 0.0 };

float De_Fitness(float *vector, float *input, DeMode_Type mode){
  float fit = 0.0;
  float min_temp[IN_DEMENSION]  = {0.0};

  for (int i = 0; i < OUT_DEMENSION; i++) vec_temp[i] = 0;
  /*降维后转化为输出向量*/
  if ((mode&MODE_DMRDT) == MODE_DMRDT){
    /*f1=M2(c-M1·f2)*/
    float temp[IN_DEMENSION] = {0.0};
    for (uint8_t i=0;i<IN_DEMENSION;i++){
      temp[i] = M1[i][0]*vector[0]+M1[i][1]*vector[1];
    }
    for (uint8_t i=0;i<IN_DEMENSION;i++){
      temp[i] = input[i]-temp[i];
    }
    vec_temp[0] = vector[0];
    vec_temp[4] = vector[1];
    for (uint8_t i=0;i<OUT_DEMENSION;i++){
      if (i==0 || i==4) continue;
      for (uint8_t j=0;j<IN_DEMENSION;j++){
        vec_temp[i] += M2[i][j]*temp[j];
      }
    }
  }else{
    for (uint8_t i=0;i<OUT_DEMENSION;i++) vec_temp[i] = vector[i];
  }

  /*适应度计算*/
  for (uint8_t i=0;i<OUT_DEMENSION;i++){ 
    fit += vec_temp[i]*vec_temp[i];
    ex_vector[i] = vec_temp[i];
  }
  fit *= FITNESS_Q;
  for (uint8_t i=0;i<IN_DEMENSION;i++){
    float temp = 0.0;
    for (uint8_t j=0;j<OUT_DEMENSION;j++) temp += M[i][j]*vec_temp[j];
    min_temp[i] = input[i] - temp;
    fit += min_temp[i]*min_temp[i];
  }
  return fit;
}

float De_Endline(float *input){
  float ep = 0.0;
  for (uint8_t i=0;i<IN_DEMENSION;i++){
    ep += input[i]*input[i];
  }
  ep = FITNESS_Q/M_Norm*ep+w2;
  return ep;
}
