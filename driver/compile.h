#ifndef _compile_h
#define	_compile_h



//功能选择，只能其中一个为1，不能同时开
#define FOUR_AXIS_UAV 	0//四轴空心杯无人机
#define FIXED_WING_AIRCRAFT		1//固定翼手抛机
#define BRUSHLESS_FOUR_AXIS_UAV		0//无刷四轴无人机

#if FOUR_AXIS_UAV

#elif FIXED_WING_AIRCRAFT

#elif BRUSHLESS_FOUR_AXIS_UAV

#endif

#endif

