/**
  ******************************************************************************
  * @file    xucz_de.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   差分进化算法实现
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

#include "xucz_de.h"

#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#include "xucz_de_type.h"

static DeAlgorithm_Type De;

void DeInit(DeAlgorithm_Type *De, float f, float cr, float *min, float *max, DeFitFunc_Ptr FitFunc){
  De->de_f = f;
  De->de_cr = cr;
  for (int i=0;i<NVAR;i++){
    De->pop_min[i] = min[i];
    De->pop_max[i] = max[i];
  }
  De->DeFitFunc = FitFunc;
}

static Individual_Type Mutate[NPOP];
static Individual_Type Cross[NPOP];

void DeRun(struct DeAlgorithm *De){
  /*种群初始化*/
  for (int i=0;i<NPOP;i++){
    for (int j=0;j<NVAR;j++){
      float randx = (float)rand()/RAND_MAX;
      De->pop[i].vector[j] = (1-randx)*(De->pop_min[j]) + randx*(De->pop_max[j]);
    }
    De->pop[i].fitness = De->DeFitFunc(De->pop[i].vector);
  }
  for(int i=0;i<NVAR;i++){
    De->global_solution[i] = De->pop[0].vector[i];
  }
  De->global_fitness = De->pop[0].fitness;

  /*开始迭代计算*/
  for (int it=1;it<=ITER;it++){

    /*变异操作*/
    for (int i=0;i<NPOP;i++){
      int rand1 = (int)rand()%NPOP;
      int rand2 = (int)rand()%NPOP;
      int rand3 = (int)rand()%NPOP;
      for (int j=0;j<NVAR;j++){
        Mutate[i].vector[j] = De->pop[rand1].vector[j] + De->de_f *\
                              (De->pop[rand2].vector[j] - De->pop[rand3].vector[j]);
        if (Mutate[i].vector[j] > De->pop_max[j]) Mutate[i].vector[j] = De->pop_max[j];
        if (Mutate[i].vector[j] < De->pop_min[j]) Mutate[i].vector[j] = De->pop_min[j];
      }
    }

    /*交叉操作*/
    for(int i=0;i<NPOP;i++){
      int randc = (int)rand()%NVAR;
      for (int j=0;j<NVAR;j++){
        float rand_cr = (float)rand()/RAND_MAX;
        if (rand_cr < De->de_cr)
          Cross[i].vector[j] = Mutate[i].vector[j];
        else
          Cross[i].vector[j] = De->pop[i].vector[j];
      }
      Cross[i].fitness = De->DeFitFunc(Cross[i].vector);
    }

    /*选择操作*/
    for (int i=0;i<NPOP;i++){
      if (Cross[i].fitness < De->pop[i].fitness){
        De->pop[i].fitness = Cross[i].fitness;
        for (int j=0;j<NVAR;j++) De->pop[i].vector[j] = Cross[i].vector[j];
        if (De->pop[i].fitness < De->global_fitness){
          De->global_fitness = De->pop[i].fitness;
          for (int j=0;j<NVAR;j++) De->global_solution[j] = De->pop[i].vector[j];
        }
      }
    }

    /*一次迭代结束*/
    printf("第 %d 轮迭代，全局适应性为 %f \r\n", it, De->global_fitness);
  }
  printf("迭代结束，全局适应度为 %f \r\n 计算结果为：", De->global_fitness);
  for (int i=0;i<NVAR;i++) printf("%f,", De->global_solution[i]);
}

float FitnessFunc(){
  return 0;
}

int main(){
  float min[NVAR] = {120};
  float max[NVAR] = {120};
  printf("开始测试！\r\n");

  clock_t start,finish = 0;
  DeAlgorithm_Type De;
  DeInit(&De, 0.4, 0.8, min, max, FitnessFunc);
  start = clock();
  DeRun(&De);
  finish = clock();

  float total_time = (float)(finish-start)/CLOCKS_PER_SEC;
  printf("运行时间为：%.3f", total_time);

  return 0;
}
