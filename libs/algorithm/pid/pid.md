# pid

> 绝对式pid计算

## 外部接口

```c

PIDInstance *PIDRegister(PID_Init_Config_s *config);
float PIDCalculate(PIDInstance *ctl, float target, float feedback);

```

初始化PID控制器时向`PIDRegister`传入PID_Init_Config_s结构体指针，返回`PIDInstance`结构体指针。

在需要调用PID运算时，调用`PIDCalculate`函数，输入`PIDInstance`结构体指针、目标值、反馈值，将会以`float`格式返回输出值
