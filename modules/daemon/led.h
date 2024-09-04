#ifndef _LED_H
#define _LED_H

#include "bsp_pwm.h"
#include "bsp_gpio.h"

typedef enum 
{
    LED_BLACK = 0B0000,
    LED_RED = 0b0001,
    LED_GREEN = 0b0010,
    LED_BLUE = 0b0100
} LED_Color_e;

typedef struct _led
{
    GPIOInstance *red_instance;
    GPIOInstance *green_instance;
    GPIOInstance *blue_instance;

    // @todo 支持设定LED颜色变化数组
} LEDInstance;

typedef struct 
{
    GPIO_Init_Config_s red_gpio_config;
    GPIO_Init_Config_s green_gpio_config;
    GPIO_Init_Config_s blue_gpio_config;
} LED_Config_s;

/// @brief LED灯初始化函数
/// @param config 
/// @return 
LEDInstance *LEDInit(LED_Config_s *config);

/// @brief 设定led灯颜色
/// @param ins 
/// @param color 
void LED_SetColor(LEDInstance *ins, LED_Color_e color);

// @todo 支持LED灯颜色按照颜色变化数组变化
// void LEDTask(void)；

#endif // !_LED_H

