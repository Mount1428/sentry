#ifndef _LIBS_PID_H
#define _LIBS_PID_H

#include <stdint.h>

typedef enum
{
    PID_Optimitation_None = 0b00000000,
    PID_Integral_Limit = 0b00000001,
    PID_Derivative_On_Measurement = 0b00000010,
    PID_Trapezoid_Intergral = 0b00000100,
    PID_Proportional_On_Measurement = 0b00001000,
    PID_OutputFilter = 0b00010000,
    PID_ChangingIntegrationRate = 0b00100000,
    PID_DerivativeFilter = 0b01000000,
    PID_ErrorHandle = 0b10000000,
} PID_Optimization_e;

/// @brief 电机堵转检测
typedef enum errorType_e
{
    PID_ERROR_NONE = 0x00U,
    PID_MOTOR_BLOCKED_ERROR = 0x01U
} ErrorType_e;

typedef struct
{
    uint64_t ERRORCount;
    ErrorType_e ERRORType;
} PID_ErrorHandler_t;

/* PID结构体 */
typedef struct _
{
    //---------------------------------- init config block
    // config parameter
    float Kp;
    float Ki;
    float Kd;
    float OutputLimit;
    float DeadBand;

    // improve parameter
    PID_Optimization_e Optimization;
    float IntegralLimit;     // 积分限幅
    float CoefA;             // 变速积分 For Changing Integral
    float CoefB;             // 变速积分 ITerm = Err*((A-abs(err)+B)/A)  when B<|err|<A+B
    float Output_LPF_RC;     // 输出滤波器 RC = 1/omegac
    float Derivative_LPF_RC; // 微分滤波器系数

    //-----------------------------------
    // for calculating
    float Measure;
    float Last_Measure;
    float Err;
    float Last_Err;
    float Last_ITerm;

    float Pout;
    float Iout;
    float Dout;
    float ITerm;

    float Output;
    float Last_Output;
    float Last_Dout;

    float Ref;

    uint32_t DWT_CNT;
    float dt;

    //-----------------------------------
    // for blocked_error callback
    void (*error_callback)(PIDInstance *pid);

    PID_ErrorHandler_t ERRORHandler; // 电机堵转错误回调函数
} PIDInstance;

/// @brief 电机堵转回调函数
typedef void (*ErrorCallback)(PIDInstance *pid);

/// @brief PID初始化结构体
typedef struct
{
    float Kp;
    float Ki;
    float Kd;
    float OutputLimit; // 输出限幅
    float DeadBand;    // 积分死区

    PID_Optimization_e Optimization;
    float IntegralLimit;     // 积分限幅
    float CoefA;             // AB为变速积分参数,变速积分实际上就引入了积分分离
    float CoefB;             // ITerm = Err*((A-abs(err)+B)/A)  when B<|err|<A+B
    float Output_LPF_RC;     // RC = 1/omegac
    float Derivative_LPF_RC; // 积分限幅

    ErrorCallback error_callback; // 电机堵转错误回调函数
} PID_Init_Config_s;

/**
 * @brief 初始化PID实例
 * @param config PID初始化配置
 * @return PIDInstance*  PID结构体
 */
PIDInstance *PIDRegister(const PID_Init_Config_s *config);

/**
 * @brief 计算PID输出
 *
 * @param pid     PID实例指针
 * @param measure 反馈值
 * @param ref     设定值
 * @return float  PID计算输出
 */
float PIDCalculate(PIDInstance *pid, float measure, float ref);

#endif // !_LIBS_PID_H
