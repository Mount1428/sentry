#ifndef _BUZZER_H
#define _BUZZER_H

#include "bsp_pwm.h"

#define DoFreq 523
#define ReFreq 587
#define MiFreq 659
#define FaFreq 698
#define SoFreq 784
#define LaFreq 880
#define SiFreq 988

typedef enum
{
    OCTAVE_0 = 0,
    OCTAVE_1,
    OCTAVE_2,
    OCTAVE_3,
    OCTAVE_4,
    OCTAVE_5,
    OCTAVE_6,
    OCTAVE_7,
    OCTAVE_8,
} Octave_e;

typedef enum
{
    BUZZER_OFF = 0,
    BUZZER_ON,
    BUZZER_BUSY
} BuzzerState_e;

typedef enum
{
    Buzzer_Once = 0,
    Buzzer_Circle
} Buzzer_Mode_e;

typedef struct
{
    float loudness;
    uint16_t delay;
} Buzzer_config_s;

typedef struct
{
    PWMInstance *pwm;

    float loudness;
    Octave_e *octave;
    BuzzerState_e buzzer_state;
    Buzzer_Mode_e mode;
    uint8_t octave_len;
    uint16_t delay;
} BuzzerInstance;

/// @brief Buzzer任务函数
/// @param argument 
void BuzzerTask(void *argument);

/// @brief Buzzer注册函数
/// @param config 
/// @return 
BuzzerInstance *BuzzerRegister(Buzzer_config_s *config);

/// @brief 设置Buzzer工作状态
/// @param buzzer 
/// @param state 
void BuzzerSetStatus(BuzzerInstance *buzzer, BuzzerState_e state);

/// @brief 设置Buzzer运行模式（单次/循环）
/// @param buzzer 
/// @param mode 
void BuzzerSetMode(BuzzerInstance *buzzer, Buzzer_Mode_e mode);

/// @brief 设置Buzzer播放音调
/// @param buzzer 
/// @param octave 音调数组
/// @param octave_len 数组长度
void BuzzerSetOctave(BuzzerInstance *buzzer, Octave_e *octave, uint8_t octave_len);

#endif // _BUZZER_H
