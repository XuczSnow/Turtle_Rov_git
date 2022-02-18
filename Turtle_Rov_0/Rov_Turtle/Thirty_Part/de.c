/**
  ******************************************************************************
  * @file    de.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 差分进化算法实现
  *
  ******************************************************************************
  * @attention
  * 移植自CSDN,原版权信息如下
  *
  ******************************************************************************
  */
	
/***************Differential Evolution Algorithm*************************/
/*
* @Author: Gong Xu
* (c) Copyright 2018 ，DSP Laboratory of Lanzhou University of Technology, All rights reserved.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "turtle_g.h"

#include "de.h"

struct DE de;

struct individual mutate[nPop];
struct individual cross[nPop];

void runde(float F, float CR, float x_low_bound, float x_up_bound, float(*function_fitness)(float*))
{
	de.F = F;
	de.CR = CR;
	de.x_low_bound = x_low_bound;
	de.x_up_bound = x_up_bound;
	de.function = function_fitness;
	
	EventStartA(0);
	// inital de for iteration program
	for (int i = 0; i < nPop; i++)
	{
		for (int j = 0; j < nVar; j++)
		{
			float randx = (float)rand() / (float)RAND_MAX;
			de.ptcle[i].position[j] = de.x_low_bound + (de.x_up_bound - de.x_low_bound) * randx;
		}
		de.ptcle[i].fitness = de.function(de.ptcle[i].position);
	}

	memcpy(&de.global_solution, &de.ptcle[0].position, sizeof(float) * nVar);
	de.global_fitness = de.ptcle[0].fitness;

	// End of initialization 
	for (int it = 1; it <= Iter; it++)
	{
		// mutate 
		for (int i = 0; i < nPop; i++)
		{

			int rand1 = (int)rand() % nPop;

			int rand2 = (int)rand() % nPop;

			int rand3 = (int)rand() % nPop;
			if (rand1 == i || rand2 == i || rand3 == i || rand1 == rand2 || rand2 == rand3)
			{

				rand1 = (int)rand() % nPop;

				rand2 = (int)rand() % nPop;

				rand3 = (int)rand() % nPop;
			}
			for (int j = 0; j < nVar; j++)
			{
				mutate[i].position[j] = de.ptcle[rand1].position[j] + F * (de.ptcle[rand2].position[j] - de.ptcle[rand3].position[j]);
				if (mutate[i].position[j] > de.x_up_bound)
				{
					mutate[i].position[j] = de.x_up_bound;
				}
				if (mutate[i].position[j] < de.x_low_bound)
				{
					mutate[i].position[j] = de.x_low_bound;
				}
			}
		}

		//crossover
		for (int i = 0; i < nPop; i++)
		{

			int randc = (int)rand() % nVar;
			for (int j = 0; j < nVar; j++)
			{
				float rand_cr = (float)rand() / (float)RAND_MAX;
				if ((j == randc) || (rand_cr < de.CR))
				{
					cross[i].position[j] = mutate[i].position[j];
				}
				else
				{
					cross[i].position[j] = de.ptcle[i].position[j];
				}
				// limit of variables of bound
				if (cross[i].position[j] > de.x_up_bound)
				{
					cross[i].position[j] = de.x_up_bound;
				}
				if (cross[i].position[j] < de.x_low_bound)
				{
					cross[i].position[j] = de.x_low_bound;
				}
			}

			cross[i].fitness = de.function(cross[i].position);
		}
		// select
		for (int i = 0; i < nPop; i++)
		{
			if (cross[i].fitness < de.ptcle[i].fitness)
			{
				de.ptcle[i].fitness = cross[i].fitness;
				memcpy(&de.ptcle[i].position, &cross[i].position, sizeof(float) * nVar);

				if (de.ptcle[i].fitness < de.global_fitness)
				{
					de.global_fitness = de.ptcle[i].fitness;
					memcpy(&de.global_solution, &de.ptcle[i].position, sizeof(float) * nVar);
				}
			}
		}

		// 一次迭代结束
		// printf("This is %d iteration, globalfitness is %f\r\n ", it, de.global_fitness);

		// 加入终止条件，以提高快速性
		if (de.global_fitness < u[2]+1)
			break;
	}
	EventStopA(0);
	
//	printf("The iteration is end, show the soultion and fitness!\r\n");
//	printf("Global_fitness :%f ", de.global_fitness);
//	for (uint8_t i = 0; i < nVar; i++)
//	{
//		printf(",%.2f ", de.global_solution[i]);
//	}
//	printf("\r\n\r\n");
}
