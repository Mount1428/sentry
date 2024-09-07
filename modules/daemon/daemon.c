/**
 * @file daemon.c
 * @author Zhou Mingfeng
 * @brief
 * @version 0.1
 * @date 2024-09-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "daemon.h"

#include "mem/mem.h"
#include "bsp_dwt.h"
#include "led.h"
#include "buzzer.h"
#include "main.h"

static DaemonInstance *instance[MAX_DAEMON_COUNT] = {nullptr}; // Daemon所有实例指针数组
static uint8_t idx = 0;                                        // Daemon模块全局索引

static LEDInstance *led_ins = nullptr;
static BuzzerInstance *buzzer_ins = nullptr;

static Octave_e error_buzz[2] = {OCTAVE_2, OCTAVE_0};
static Octave_e warning_buzz[5] = {OCTAVE_3, OCTAVE_0, OCTAVE_3, OCTAVE_0, OCTAVE_0};
static Octave_e start_buzz[4] = {OCTAVE_3, OCTAVE_4, OCTAVE_5, OCTAVE_0};

void DaemonInit(void)
{
    LED_Config_s led_config = {
        .red_gpio_config = {.GPIOx = LED_R_GPIO_Port, .GPIO_Pin = LED_R_Pin},
        .blue_gpio_config = {.GPIOx = LED_B_GPIO_Port, .GPIO_Pin = LED_B_Pin},
        .green_gpio_config = {.GPIOx = LED_G_GPIO_Port, .GPIO_Pin = LED_G_Pin}};
    led_ins = LEDInit(&led_config);

    Buzzer_config_s buzz_config = {
        .delay = 200,
        .loudness = 0.01};
    buzzer_ins = BuzzerRegister(&buzz_config);

    BuzzerSetStatus(buzzer_ins, BUZZER_ON);
    BuzzerSetOctave(buzzer_ins, start_buzz, 4);
}

DaemonInstance *DaemonRegister(Daemon_Init_Config_s *config)
{
    DaemonInstance *ins = (DaemonInstance *)mem_Calloc(GET_TYPE_SIZE(DaemonInstance));
    if (!ins)
        return nullptr;

    ins->reload_count = config->reload_count ? config->reload_count : 100;
    ins->temp_count = config->init_count ? config->reload_count : 100;
    ins->offline_status = config->daemon_status;
    ins->offline_callback = config->callback;
    ins->id = config->owner_id;

    ins->temp_count += ins->reload_count;

    instance[idx++] = ins;
    return ins;
}

void DaemonReload(DaemonInstance *ins)
{
    ins->temp_count = ins->reload_count;
}

uint8_t DaemonIsOnline(DaemonInstance *ins)
{
    return ins->daemon_status == DAEMON_NONE;
}

void DaemonTask(void)
{
    DaemonInstance *ins; // 降低访问内存开销
    Daemon_Status_e temp_status = {0};
    for (uint8_t i = 0; i < idx; i++)
    {
        ins = instance[i];
        if (ins->temp_count > 0)
        {
            ins->daemon_status = DAEMON_NONE;
            ins->temp_count -= 1 * 10; // 任务调用间隔为100Hz
        }
        else
        {
            ins->daemon_status = ins->offline_status;
            temp_status |= ins->offline_status;
            if (ins->offline_callback)
            {
                ins->offline_callback(ins);
            }
        }
    }

    // 离线报警(处理LED和Buzzer)
    if (temp_status & DAEMON_ERROR)
    {
        LED_SetColor(led_ins, LED_RED);
        BuzzerSetStatus(buzzer_ins, BUZZER_ON);
        BuzzerSetOctave(buzzer_ins, error_buzz, 2);
    }
    else if (temp_status & DAEMON_WARNING)
    {
        LED_SetColor(led_ins, LED_GREEN | LED_BLUE);
        BuzzerSetStatus(buzzer_ins, BUZZER_ON);
        BuzzerSetOctave(buzzer_ins, warning_buzz, 5);
    }
    else
    {
        LED_SetColor(led_ins, LED_GREEN);
        BuzzerSetStatus(buzzer_ins, BUZZER_OFF);
    }
}
