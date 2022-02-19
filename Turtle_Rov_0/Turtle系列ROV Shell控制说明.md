# Turtle系列ROV Shell控制说明

注：[arg] 必选参数、\<arg> 可选参数

## Letter Shell 自带指令

- help
  
  命令帮助

## Turtle系列ROV 系统控制指令

- t_hello
  
  输出欢迎页面

- t_reset
  
  重启机器人

- t_download
  
  机器人固件下载

## Turtle系列ROV 自动控制指令

- t_auto [flag]  [en]  [sp] \<mode> \<out>

  自动控制模式设置：

  参数|取值
  ---|:---
  flag|自动控制目标设置 (AUTO_DEPTH AUTO_HEADING)
  en|自动控制使能 (1 0)
  sp|设定值
  mode|控制方式设定(MODE_PID MODE_FOPID MODE_CCPID)
  out|自动控制输出方式 (OUT_ORG OUT_DC)

- t_si [flag] [en] \<mode> (**暂无**)

  系统辨识模式设置：

  参数|取值
  ---|:---
  flag|系统辨识目标设置 (SI_DEEP SI_HEADING SI_PITCH SI_ROLL)
  en|系统辨识使能 (1 0)
  mode|系统辨识方式设定(SI_SIN)

- t_prop [flag] [ppm]

  单一推进器控制量设置(ppm)

  参数|取值
  ---|:---
  flag|推进器序号(PROP1 ……)
  ppm|推进器控制量(ppm值)

- t_autoDeepSqrt_en [en] (**暂无**)

  是否使能深度控制外环平方根

  参数|取值
  ---|:---
  en|是否使能(1 0)

- t_autoAttitude_en [en]

  是否使能姿态控制

  参数|取值
  ---|:---
  en|是否使能(1 0)

- t_autoDe_en [en]

  是否使能差分进化算法

  参数|取值
  ---|:---
  en|是否使能(1 0)
