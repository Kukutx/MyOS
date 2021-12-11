#include "common/types.h"
#include "gdt.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/pci.h"
#include "drivers/driver.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"
#include "drivers/vga.h"
#include "gui/desktop.h"
#include "gui/window.h"
#include "multitasking.h"
#include "memorymanagement.h"
#include "drivers/amd_am79c973.h"
#include "net/etherframe.h"
#include "net/arp.h"
#include "net/ipv4.h"
#include "net/icmp.h"
#include "net/udp.h"
#include "net/tcp.h"

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;
using namespace myos::net;
// 获取显示器的物理地址，打印命令行
void printf(const char* str) {
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;// 获取窗口屏幕坐标
    static uint8_t x = 0, y = 0;                      //定义窗口屏幕的坐标位
    // 取出低16位，高位不取，print打印支持换行
    for (int i = 0; str[i]; i++) {
        switch(str[i]) {
        case '\n':
            y++;  // 换行
            x = 0;
            break;
        default:
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
            x++;
            break;
        }
        // 显示器的像素为 80*25(因为命令行是一行一行实现的，每行最多是80字符，最多25行，而命令行偏移是类似数组的是连续的)
        if (x >= 80) { // 写满一行了
            x = 0;
            y++;
        }

        if (y >= 25) { // 写满屏了，清屏操作
            for (y = 0; y < 25; y++) { // 遍历每一行
                for (x = 0; x < 80; x++) { // 遍历每一列
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';// 和 ' '字符或一下即可清空
                }
            }
            x = 0, y = 0;
        }
    }
}

void printfHex(uint8_t key) {
    char* foo = (char*)"00";
    const char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0x0f];
    foo[1] = hex[key & 0x0f];
    printf((const char*)foo);
}

class PrintKeyboardEventHandler : public KeyboardEventHandler {
public:
    void OnKeyDown(char c) {
        char* foo = (char*)" ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler {
public:
    MouseToConsole()
        : x(40),y(12) {
    }

    void OnActivate() {
        uint16_t* VideoMemory = (uint16_t*)0xb8000; // 获取内存地址
        // 往这个内存地址做一些操作，这样可以获取到鼠标对应的位置
        VideoMemory[y * 80 + x] = ((VideoMemory[y * 80 + x] & 0xf000) >> 4) |
                                ((VideoMemory[y * 80 + x] & 0x0f00) << 4) |
                                (VideoMemory[y * 80 + x] & 0x00ff);
    }
    // 重载方法
    void OnMouseMove(int8_t nx, int8_t ny) override {
        uint16_t* VideoMemory = (uint16_t*)0xb8000; // 获取内存地址
        // 往这个内存地址做一些操作，这样可以获取到鼠标对应的位置
        VideoMemory[y * 80 + x] = ((VideoMemory[y * 80 + x] & 0xf000) >> 4) |
                                ((VideoMemory[y * 80 + x] & 0x0f00) << 4) |
                                (VideoMemory[y * 80 + x] & 0x00ff);
        // 鼠标往右移动：x 增大；鼠标往下移动：y 增大，但是 buffer[2] 是相反的，取一下反
        x += nx;
        if (x < 0) x = 0;
        else if (x >= 80) x = 79;

        y += ny;
        if (y < 0) y = 0;
        else if (y >= 25) y = 24;

        VideoMemory[y * 80 + x] = ((VideoMemory[y * 80 + x] & 0xf000) >> 4) |
                                ((VideoMemory[y * 80 + x] & 0x0f00) << 4) |
                                (VideoMemory[y * 80 + x] & 0x00ff);
    }
    
private:
    int8_t x, y;
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
// 调用 constructor 函数
extern "C" void callConstructors() {
    for (constructor* i = &start_ctors; i != &end_ctors; i++) {
        (*i)();
    }
}
void taskA() {
    while (true) {
        printf("A");
    }
}

void taskB() {
    while (true) {
        printf("B");
    }
}
class PrintfUDPHandler : public UserDatagramProtocolHandler {
public:
    void HandleUserDatagramProtocolMessage(UserDatagramProtocolSocket* socket, uint8_t* data, uint16_t size) {
        char* foo = "";
        for (int i = 0; i < size; i++) {
            foo[0] = data[i];
            printf(foo);
        }
    }
};
class PrintfTCPPHandler : public TransmissionControlProtocolHandler {
public:
    bool HandleUserDatagramProtocolMessage(TransmissionControlProtocolHandler* socket, uint8_t* data, uint16_t size) {
        char* foo = "";
        for (int i = 0; i < size; i++) {
            foo[0] = data[i];
            printf(foo);
        }
        return true;
    }
};
//内核主函数
extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber) {
    printf("hello world!\n");
    printf("cpp\n");
    GlobalDescriptorTable gdt;
    size_t heap = 10 * 1024 * 1024;
    uint32_t* memupper = (uint32_t*)((size_t)multiboot_structure + 8);
    printfHex(((*memupper) >> 24) & 0xff);
    printfHex(((*memupper) >> 16) & 0xff);
    printfHex(((*memupper) >> 8) & 0xff);
    printfHex(((*memupper) >> 0) & 0xff);

    MemoryManager memoryManager(heap, (*memupper) * 1024 - heap - 10 * 1024);

    printf("\n heap: 0x");
    printfHex((heap >> 24) & 0xff);
    printfHex((heap >> 16) & 0xff);
    printfHex((heap >> 8) & 0xff);
    printfHex((heap >> 0) & 0xff);

    void* allocated = memoryManager.malloc(1024);
    printf("\n allocated: 0x");
    printfHex(((size_t)allocated >> 24) & 0xff);
    printfHex(((size_t)allocated >> 16) & 0xff);
    printfHex(((size_t)allocated >> 8) & 0xff);
    printfHex(((size_t)allocated >> 0) & 0xff);

    TaskManger taskManger;
    // Task task1(&gdt, taskA);
    // Task task2(&gdt, taskB);
    // taskManger.AddTask(&task1);
    // taskManger.AddTask(&task2);
    InterruptManager interrupts(0x20, &gdt, &taskManger);
// #define GRAPHICMODE
#ifdef GRAPHICMODE 
    Desktop desktop(320, 200, 0x00, 0x00, 0xa8);
#endif
    DriverManager drvManager;
    
#ifdef GRAPHICMODE 
    KeyBoardDriver keyboard(&interrupts, &desktop);
#else
    PrintKeyboardEventHandler kbhandler;              // keyboard handler
    KeyBoardDriver keyboard(&interrupts, &kbhandler); // 中断开启前，keyboard 进行初始化 
#endif
    drvManager.AddDriver(&keyboard);                  // 把 keyboard 放到 drvManager 里面

#ifdef GRAPHICMODE 
    MouseDriver mouse(&interrupts, &desktop);
#else
    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
#endif
    drvManager.AddDriver(&mouse);                      // 把 mouse 放到 drvManager 里面

    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);
    VideoGraphicsArray vga;

    drvManager.ActivateAll();    // 激活所有驱动

#ifdef GRAPHICMODE
    vga.SetMode(320, 200, 8);
    Window w1(&desktop, 10, 10, 20, 20, 0xa8, 0x00, 0x00);
    desktop.AddChild(&w1);
    Window w2(&desktop, 40, 15, 30, 30, 0x00, 0xa8, 0x00);
    desktop.AddChild(&w2);
#endif

    uint8_t ip1 = 10, ip2 = 0, ip3 = 2, ip4 = 15;
    uint32_t ip_be = ((uint32_t)ip4 << 24) 
                    |((uint32_t)ip3 << 16)
                    |((uint32_t)ip2 << 8)
                    | (uint32_t)ip1;

    uint8_t gip1 = 10, gip2 = 0, gip3 = 2, gip4 = 2;
    uint32_t gip_be = ((uint32_t)gip4 << 24) 
                    |((uint32_t)gip3 << 16)
                    |((uint32_t)gip2 << 8)
                    | (uint32_t)gip1;

    amd_am79c973* eth0 = (amd_am79c973*)(drvManager.drivers[2]);

    eth0->SetIPAddress(ip_be);
    EtherFrameProvider etherframe(eth0);

    AddressResolutionProtocol arp(&etherframe);

    uint8_t subnet1 = 255, subnet2 = 255, subnet3 = 255, subnet4 = 0;
    uint32_t subnet_be = ((uint32_t)subnet4 << 24) 
                    |((uint32_t)subnet3 << 16)
                    |((uint32_t)subnet2 << 8)
                    | (uint32_t)subnet1;

    InternetProtocolProvider ipv4(&etherframe, &arp, gip_be, subnet_be);
    InternetControlMessageProtocol icmp(&ipv4);
    UserDatagramProtocolProvider udp(&ipv4);
    TransmissionControlProtocolProvider tcp(&ipv4);
    // etherframe.Send(0xffffffffffff, 0x608, (uint8_t*)"Hello Network", 13);

    interrupts.Activate();    // 开启中断

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    // arp.Resolve(gip_be);
    // ipv4.Send(gip_be, 0x0008, (uint8_t*)"Hello Network", 13);
    arp.BroadcastMACAddress(gip_be);
    // icmp.RequestEchoReply(gip_be);

    // PrintfUDPHandler udphandler;
    // UserDatagramProtocolSocket* socket = udp.Listen(1234);
    // udp.Bind(socket, &udphandler);

    tcp.Connect(gip_be, 1234);
    PrintfTCPPHandler tcphandler;
    TransmissionControlProtocolSocket* tcpsocket = tcp.Connect(gip_be, 1234);
    tcp.Bind(tcpsocket, &tcphandler);
    tcpsocket->Send((uint8_t*)"Hello World!", 12);
    
    while(1) {
#ifdef GRAPHICMODE
        desktop.Draw(&vga);
#endif
    }
}