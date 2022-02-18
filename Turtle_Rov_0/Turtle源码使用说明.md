# Turtle源码使用说明

版本特性：本程序主要是

Note：本文档作为Turtle系列ROV不完全使用手册，尽可能地保证其可靠性及准确性，由于仍是Alpha版本，所以可能没有版本号对应，后期可能实现，可以通过二进制文件中的编译日期对应版本。由于程序初期设计问题，虽然实现了模块化地设计，但是配置文件仍存在比较混乱的问题，望后期维护的同学可以改进。目前的配置文件，主要在`turtle_g.h`文件中，后期的改进尽量放置到`core`目录下的功能头文件中，仍不完善，望谅解，后期应该都做成`#define`写到`cfg`文件中。

## 1. 目录说明

- `Application/MDK-ARM`     STM32 启动文件及相关编译结果，不建议修改
- `Application/User/Core`     STM32CubeMX生成的初始化文件，除 `main.c` 文件外，不建议修改
- `Rov_Turtle/Common`     ROV公共驱动文件，如全局设置，中断及CRC16等相关公用库文件
- `Rov_Turtle/Core`     ROV核心功能上层文件，包含自动控制、调试、定时器中断处理(考虑到便捷性，部分在 `turtle_it` 中)
- `Rov_Turtle/Function`    ROV功能性驱动文件，包含推进器、云台等的驱动
- `Rov_Turtle/ThirtyPart`     ROV移植的小型第三方库
- `Rov_Turtle/Shell`     移植第三方SHELL相关文件，其中 `Usr` 为用户命令定义文件及BSP文件，其余为原生文件除 `cfg` 文件外 不建议修改
- `Drivers/STM32F1xx_HAL_Driver`     STM32 HAL库文件
- `Drivers/CMSlS`     ARM驱动文件

## 2.



