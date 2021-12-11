#ifndef __MYOS__DRIVERS__KEYBOARD_H
#define __MYOS__DRIVERS__KEYBOARD_H
/**********键盘驱动事件**********/
#include "common/types.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"

namespace myos 
{
    namespace drivers 
    {   //键盘事件句柄类
        class KeyboardEventHandler {           
        public:
            KeyboardEventHandler();             
            ~KeyboardEventHandler();
            // 定义为 virtual，因为是通用的接口
            virtual void OnKeyDown(char);       //键盘按下事件接口
            virtual void OnKeyUp(char);         //键盘抬起事件接口
        };
        // 定义一个类继承一些中断的处理函数，键盘驱动构造类
        class KeyBoardDriver : public myos::hardwarecommunication::InterruptHandler, public Driver {
        public:
            // 传入一个 InterruptManager，通过这个管理者，去处理一些中断程序
            KeyBoardDriver(myos::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler* handler);   //键盘驱动构造函数
            ~KeyBoardDriver();
            virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);   //句柄中断接口
            virtual void Activate();            // 定义抽象的激活函数
        private:
            myos::hardwarecommunication::Port8Bit dataport;           //数据端口
            myos::hardwarecommunication::Port8Bit commandport;        //命令端口
            KeyboardEventHandler* handler;      //键盘句柄
        };
    }
}


#endif 