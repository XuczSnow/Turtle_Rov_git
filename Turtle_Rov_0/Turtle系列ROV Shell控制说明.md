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

  自动控制模式设置。

  参数|取值
  ---|:---
  flag|自动控制目标设置 (AUTO_DEPTH AUTO_HEADING)
  en|自动控制使能 (1 0)
  sp|设定值
  mode|控制方式设定(MODE_PID MODE_FOPID MODE_CCPID)
  out|自动控制输出方式 (OUT_ORG OUT_DC)

- t_auto_param [flag] [Kp] [Ki] [Kd] [lam] [u]

  自动控制参数设置。

  参数|取值
  ---|:---
  flag |自动控制目标设置 (AUTO_DEPTH AUTO_HEADING AUTO_PUTCH CC_PID_IN CC_PID_OUT)
  Kp|比例增益
  Ki|积分增益
  Kd|微分增益
  lam|积分阶次
  u|微分阶次

- t_prop [flag] [ppm]

  单一推进器控制量设置(ppm)

  参数|取值
  ---|:---
  flag|推进器序号(PROP1 ……)
  ppm|推进器控制量(ppm值)

- t_autoAttitude_en [en]

  是否使能姿态控制

  参数|取值
  ---|:---
  en|是否使能(1 0)

- t_autoSetDc_value [value]

  设置浮力补偿值

  参数|取值
  ---|:---
  value| 实际补偿值（0-125）

- t_autoSet_cycle [value]

  设置控制周期

  参数|取值
  ---|:---
  value| 实际设置控制周期为(value*10)ms

- t_pressSet_lptk [gain]

  设置低通滤波增益，用于垂向运动速度求解

  参数|取值
  ---|:---
  gain|浮点数(0-1)
