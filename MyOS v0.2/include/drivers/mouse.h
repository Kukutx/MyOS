#ifndef __MYOS__DRIVERS__MOUSE_H
#define __MYOS__DRIVERS__MOUSE_H
/**********鼠标驱动事件**********/
#include "common/types.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"

namespace myos 
{
    namespace drivers 
    {   //鼠标事件句柄类
        class MouseEventHandler {
        public:
            MouseEventHandler();              
            ~MouseEventHandler();
            // 定义为 virtual，因为是通用的接口
            virtual void OnActivate();                                      // 激活函数，用于激活 mouse
            virtual void OnMouseDown(myos::common::uint8_t button);         //鼠标按下事件接口
            virtual void OnMouseUp(myos::common::uint8_t button);           //鼠标松开事件接口
            virtual void OnMouseMove(myos::common::int8_t x, myos::common::int8_t y);        //鼠标移动事件接口
        };
        //鼠标驱动类
        class MouseDriver : public myos::hardwarecommunication::InterruptHandler, public Driver {
        public:
            MouseDriver(myos::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);   //鼠标驱动构造函数
            ~MouseDriver();
            virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);    //句柄中断接口
            virtual void Activate();  // 定义抽象的激活函数
        private:
            myos::hardwarecommunication::Port8Bit dataport;           //数据端口
            myos::hardwarecommunication::Port8Bit commandport;        //命令端口
            /*********鼠标按键状态*********/
            myos::common::uint8_t buffer[3];                          //缓冲区
            myos::common::uint8_t offset;                             // 表示读取的是哪一个字节，初始值为 0
            myos::common::uint8_t buttons;                            // 标志按键的状态：按下状态 or 释放状态
            /*********鼠标坐标和句柄*********/
            myos::common::int8_t x, y;      // 坐标表示鼠标的位置，使用 int8_t 类型
            MouseEventHandler* handler;     //鼠标句柄
        };
    }
}

#endif 