
#define   NVAR    8       /*计算维度*/
#define   NPOP    20      /*种群数量*/
#define   ITER    100     /*迭代次数*/

typedef struct  Individual    Individual_Type;
typedef struct  DeAlgorithm   DeAlgorithm_Type;
typedef float   (*DeFitFunc_Ptr)(float *vector);

struct Individual{
  float vector[NVAR];
  float fitness;
};

struct DeAlgorithm{
  Individual_Type pop[NPOP];
  DeFitFunc_Ptr   DeFitFunc;
  float de_f;
  float de_cr;
  float global_fitness;
  float global_solution[NVAR];
  float pop_min[NPOP];
  float pop_max[NPOP];
};
