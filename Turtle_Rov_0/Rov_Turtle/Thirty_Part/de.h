/**
  ******************************************************************************
  * @file    de.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 差分进化算法头文件
  *
  ******************************************************************************
  * @attention
  * 
  *
  ******************************************************************************
  */
	
#ifndef TURTLE_DE_H
#define TURTLE_DE_H

#define nVar 4  // Number of variables
#define nPop 10   // Number of Population
#define Iter 100  // Maximum number of iterations

struct individual
{
	float position[nVar];
	float fitness;

};

struct DE
{
	struct individual ptcle[nPop];
	float F;
	float CR;
	float(*function)(float*);  // Function pointer definition
	float global_fitness;
	float global_solution[nVar];
	float x_low_bound;
	float x_up_bound;
};

extern struct DE de;

void runde(float F, float CR, float x_low_bound, float x_up_bound, float(*function_fitness)(float*));

#endif
