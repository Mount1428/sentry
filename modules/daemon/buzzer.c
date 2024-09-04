//
// Created by 晚风 on 2024/4/18.
//

#include "buzzer.h"

#include "time/time.h"
#include "bsp_pwm.h"
#include "cmsis_os2.h"
#include "mm/mem.h"

static BuzzerInstance *_buzzer = nullptr;
static uint8_t idx = 0;

osThreadId_t BuzzerTaskHandle;
const osThreadAttr_t BuzzerTask_attributes = {
    .name = "BuzzerTask",
    .stack_size = 64 * 1,
    .priority = (osPriority_t)osPriorityHigh,
};

/* ---------------- static functions ---------------- */
static void Buzzer_Init(BuzzerInstance *buzzer)
{
    PWM_Init_Config_s buzzer_config = {
        .htim = &htim4,
        .channel = TIM_CHANNEL_3,
        .dutyratio = 0,
        .period = 0.001};

    buzzer->pwm = PWMRegister(&buzzer_config);
    PWMSetDutyRatio(buzzer->pwm, 0);
}

/* ---------------- extern functions ---------------- */
BuzzerInstance *BuzzerRegister(Buzzer_config_s *config)
{
    if (!idx)
    {
        BuzzerTaskHandle = osThreadNew(BuzzerTask, NULL, &BuzzerTask_attributes);
        idx++;
    }
    else
    {
        while (1)
            ;
    }

    BuzzerInstance *ins = (BuzzerInstance *)mem_Calloc(sizeof(BuzzerInstance));
    if (ins == nullptr)
        return nullptr;

    ins->loudness = config->loudness;
    ins->buzzer_state = BUZZER_OFF;
    ins->delay = config->delay;
    ins->mode = Buzzer_Once;
    Buzzer_Init(ins);

    return ins;
}

void BuzzerSetStatus(BuzzerInstance *buzzer, BuzzerState_e state)
{
    // while (buzzer->buzzer_state == BUZZER_BUSY)
    // ;

    buzzer->buzzer_state = state;
}

void BuzzerSetMode(BuzzerInstance *buzzer, Buzzer_Mode_e mode)
{
    // while (buzzer->buzzer_state == BUZZER_BUSY)
    // ;
    BuzzerState_e temp = buzzer->buzzer_state;
    buzzer->buzzer_state = BUZZER_OFF;

    buzzer->mode = mode;
    buzzer->buzzer_state = temp;
}

void BuzzerSetOctave(BuzzerInstance *buzzer, Octave_e *octave, uint8_t octave_len)
{
    // while (buzzer->buzzer_state == BUZZER_BUSY)
    // ;

    BuzzerState_e temp = buzzer->buzzer_state;
    buzzer->buzzer_state = BUZZER_OFF;

    buzzer->octave = octave;
    buzzer->octave_len = octave_len;

    buzzer->buzzer_state = temp;
}

void BuzzerTask(void *argument)
{
    UNUSED(argument);

    for (;;)
    {

        if (!_buzzer)
            continue;

        if (_buzzer->buzzer_state == BUZZER_OFF)
        {
            PWMSetDutyRatio(_buzzer->pwm, 0);
        }
        else
        {
            BuzzerState_e temp = _buzzer->buzzer_state;
            _buzzer->buzzer_state = BUZZER_BUSY;

            for (uint8_t j = 0; j < _buzzer->octave_len; j++)
            {
                PWMSetDutyRatio(_buzzer->pwm, _buzzer->loudness);
                switch (_buzzer->octave[j])
                {
                case OCTAVE_0:
                {
                    PWMSetDutyRatio(_buzzer->pwm, 0);
                    break;
                }
                case OCTAVE_1:
                {
                    PWMSetPeriod(_buzzer->pwm, (float)1 / DoFreq);
                    break;
                }
                case OCTAVE_2:
                {
                    PWMSetPeriod(_buzzer->pwm, (float)1 / ReFreq);
                    break;
                }
                case OCTAVE_3:
                {
                    PWMSetPeriod(_buzzer->pwm, (float)1 / MiFreq);
                    break;
                }
                case OCTAVE_4:
                {
                    PWMSetPeriod(_buzzer->pwm, (float)1 / FaFreq);
                    break;
                }
                case OCTAVE_5:
                {
                    PWMSetPeriod(_buzzer->pwm, (float)1 / SoFreq);
                    break;
                }
                case OCTAVE_6:
                {
                    PWMSetPeriod(_buzzer->pwm, (float)1 / LaFreq);
                    break;
                }
                case OCTAVE_7:
                {
                    PWMSetPeriod(_buzzer->pwm, (float)1 / SiFreq);
                    break;
                }
                default:
                    break;
                }
                osDelay(_buzzer->delay);
            }
            _buzzer->buzzer_state = temp;

            if (_buzzer->mode == Buzzer_Once)
            {
                _buzzer->buzzer_state = BUZZER_OFF;
            }
        }

        osDelay(100);
    }
}
