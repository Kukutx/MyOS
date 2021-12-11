#include "drivers/mouse.h"

using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

MouseEventHandler::MouseEventHandler() {}
MouseEventHandler::~MouseEventHandler() {}
void MouseEventHandler::OnActivate() {}
void MouseEventHandler::OnMouseDown(uint8_t button) {}
void MouseEventHandler::OnMouseUp(uint8_t button) {}
void MouseEventHandler::OnMouseMove(int8_t x, int8_t y) {}

MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler) 
    : InterruptHandler(0x0C + manager->HardwareInterruptOffset(), manager),  // 0x0C：鼠标中断是 12 号
    dataport(0x60),             // 端口仍是 0x60，因为鼠标和键盘使用的是相同的控制器
    commandport(0x64),
    offset(0),
    buttons(0),
    handler(handler) {
    
}

MouseDriver::~MouseDriver() {}

void printf(const char*);

void MouseDriver::Activate() {
    if (handler != nullptr) 
        handler->OnActivate();

    commandport.Write(0xa8);
    commandport.Write(0x20);
    uint8_t status = (dataport.Read() | 2) & ~0x20;
    commandport.Write(0x60);
    dataport.Write(status);

    commandport.Write(0xd4);
    dataport.Write(0xf4);
    dataport.Read();
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp) {
    uint8_t status = commandport.Read();    // 首先从 commandport 里面读取鼠标的状态
    // 如果不是鼠标或者 handler 为空，则直接返回；0x20 对应着上面 0x20 相关状态值的操作
    if (!(status & 0x20) || handler == nullptr) return esp;

    buffer[offset] = dataport.Read();  // 从 buffer 里面读取数据
    offset = (offset + 1) % 3;         // 然后 offset 此时需要往后偏移一位

    if (offset == 0) {                 // 如果 offset==0，说明 x,y 被操纵过了
        handler->OnMouseMove(buffer[1], -buffer[2]);
        // 实现按键按下变白的功能
        for (uint8_t i = 0; i < 3; i++) {
            // buttons 描述的是上一个按键的状态，buffer[0] 是当前按键的状态
            if ((buffer[0] & (1 << i)) != (buttons & (1 << i))) {
                if (buttons & (1 << i)) {        //上一个按键的状态是 1 的话，当前按键的状态必然是 0，即 mouse 被抬起了
                    handler->OnMouseUp(i + 1);   // 还没函数实现，随便给个值
                } else {
                    handler->OnMouseDown(i + 1); // 还没函数实现，随便给个值
                }
            }
        }
        buttons = buffer[0];
    }
    
    
    return esp;
}