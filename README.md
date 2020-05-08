# [RTX](https://github.com/OS-Q/RTX) 

[![sites](http://182.61.61.133/link/resources/OSQ.png)](http://www.OS-Q.com)

#### 归属实时系统：[RTQ](https://github.com/OS-Q/RTQ)

### [RTX简介](https://github.com/OS-Q/RTX/wiki)

[RTX](https://github.com/OS-Q/RTX) 使用标准的C结构编写，运用RealView编译器进行编译。RTX.H 头文件定义了RTX函数以及宏，可以让轻松地声明任务并达到RTOS所有特性。

RTX的基本功能就是开始和停止任务（进程），除此之外还支持进程通信，例如任务的同步、共享资源（外设或内存）的管理、任务之间消息的传递。

开发者可以使用基本函数去开启实时运行器，去开始和终结任务，以及去传递任务间的控制（轮转调度）。开发者可以赋予任务优先级。

RTX是Keil MDK软件自带的一个小型的操作系统,是免版税的确定性实时操作系统,适用于 ARM 和 Cortex-M 设备。


#### RTX时序规格 (单位：us)

* 初始化系统(os_sys_init), 启动进程： 36.3
* 创建定义的进程，没有进程切换： 12.9
* 创建定义的进程，切换进程： 14.6 
* 撤消进程(通过os_tsk_delete)： 5.9 
* 进程切换(通过os_tsk_delete_self)： 8.8 
* 进程切换(通过os_tsk_pass)： 4.6 
* 进程切换(upon set event)： 7.3 
* 进程切换(upon sent semaphore)： 5.5 
* 进程切换(upon sent message)： 6.1 
* 设置时间(没有进程切换)： 2.5 
* 发送信号量 (没有进程切换)： 1.9 
* 发送消息(没有进程切换)： 2.8 
* 获得进程标识符 (os_tsk_self)： 1.0 
* IRQ中断服务子程序的中断响应时间： 0.4 
* IRQ中断服务子程序的最大等待时间(lockout)： 2.2 
* IRQ中断服务子程序的最大中断延迟 (response + lockout)： 2.6


### [收录资源](https://github.com/OS-Q) 

* [文档](docs/)
* [资源](src/)

### [OS-Q = Open Solutions | Open Source |  Operating System ](http://www.OS-Q.com)