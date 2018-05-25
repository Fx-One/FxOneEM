// 平台定义(指定的值只用于唯一区别，无特定要求)
#ifndef STM32F10x
	#define STM32F10x 110
#endif

#ifndef STM32F405x
	#define STM32F405x 1405
#endif

// 平台切换
#ifndef PLATFORM
	#define PLATFORM STM32F10x
#endif

