/**
 * @file led.c
 * @author Zhou Mingfeng
 * @brief
 * @version 0.1
 * @date 2024-09-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "led.h"

#include "mm/mem.h"

LEDInstance *LEDInit(LED_Config_s *config)
{
    LEDInstance *ins = (LEDInstance *)mem_Calloc(GET_TYPE_SIZE(LEDInstance));

    ins->red_instance = GPIORegister(&config->red_gpio_config);
    ins->green_instance = GPIORegister(&config->green_gpio_config);
    ins->blue_instance = GPIORegister(&config->blue_gpio_config);

    return ins;
}

void LED_SetColor(LEDInstance *ins, LED_Color_e color)
{
    if (color == LED_BLACK)
    {
        GPIOSet(ins->red_instance);
        GPIOSet(ins->green_instance);
        GPIOSet(ins->blue_instance);
    }
    else
    {
        if (color & LED_RED)
        {
            GPIOReset(ins->red_instance);
        }
        else
        {
            GPIOSet(ins->red_instance);
        }

        if (color & LED_GREEN)
        {
            GPIOReset(ins->green_instance);
        }
        else
        {
            GPIOSet(ins->green_instance);
        }

        if (color & LED_BLUE)
        {
            GPIOReset(ins->blue_instance);
        }
        else
        {
            GPIOSet(ins->blue_instance);
        }
    }
}
