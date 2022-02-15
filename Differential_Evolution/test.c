#include <time.h>
#include <stdio.h>
#include <string.h>

#include "de_fitness.h"
#include "de_type.h"
#include "de_usr_conf.h"
#include "./Adaptive_De/adaptive_de.h"
//#include "./Classical_De/classical_de.h"

float FitnessFunc(){
  return 0;
}

float EndlineFunc(){
  return 0;
}

float input[] = {0,0,0,0,0,50};

/*调用示例*/
int main(){
  FILE *fp = NULL;
  errno_t err = 0;
  char path[100] = { 0 };
  char text[100] = { 0 };
  float min[NVAR] = {-120, -120};
  float max[NVAR] = { 120,  120};
  //float min[NVAR] = {-120, -120, -120, -120, -120, -120, -120, -120};
  //float max[NVAR] = { 120,  120,  120,  120,  120,  120,  120,  120};
  printf("开始测试！\r\n");

  for (int i = 0; i < 100; i++) {
    clock_t start,finish = 0;
    DeAlgorithm_Type De;
    DeInit(&De, 0.8, 0.4, min, max, De_Fitness, EndlineFunc);
    start = clock();
    DeRun(&De, input);
    finish = clock();

    float total_time = (float)(finish-start)/CLOCKS_PER_SEC;
    printf(" 运行时间为：%.3f\n", total_time);
    printf(" 迭代结束，全局适应度为 %f \n 计算结果为：", De.global_fitness);
    for (int i=0;i<OUT_DEMENSION;i++) printf("%f,", vec_temp[i]);
    printf("\b\n");
    printf(" 结果存储中...\n");
    sprintf_s(path, 100, "cls_de_%d.txt", i);
    err = fopen_s(&fp,path,"w+");
    sprintf_s(text,100,"迭代次数: %d, 运行时间: %f\n",De.log_iter,total_time);
    fputs(text, fp);
    for (int i=0;i<ITER;i++){
      sprintf_s(text,100,"%d,%f\n",i,De.log_fitness[i]);
      fputs(text, fp);
    }
    for (int i = 0; i < OUT_DEMENSION; i++) {
      sprintf_s(text, 100, "%f,", vec_temp[i]);
      fputs(text, fp);
    }
    err = fclose(fp);
    printf(" 结果存储完成！\n");
  }
  return 0;
}
