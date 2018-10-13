# FxOne
FxOne是基于FreeRTOS开发的嵌入式开发框架。  
开发语言: C++  
IDE：eclipse + CDT + GNU ARM  
编译器：GNU Tools for ARM Embedded Processors (参考: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm)

# 支持硬件说明
FxOne的开发目标是希望能够支持大部分常用的MCU。  
由于硬件平台资源有限，因此暂时基于STM32F1系列硬件进行封装。

# 进度说明
1. Core模块已经稳定  
2. Thread模块已经稳定  
3. IO模块还不稳定（建议不要使用)  

# 待解决的问题
1. 基于C++11实现信号与槽  
