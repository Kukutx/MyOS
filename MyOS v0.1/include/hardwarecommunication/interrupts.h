#ifndef __MYOS__HARDWARECOMMUNICATION__INTERRUPTS_H
#define __MYOS__HARDWARECOMMUNICATION__INTERRUPTS_H
/**********实现中断**********/
#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "gdt.h"

namespace myos
{
    namespace hardwarecommunication
    {   //中断管理类
        class InterruptManager;
        // 定义一个中断相关的基类，中断句柄类
        class InterruptHandler {
        public:
            // 处理中断的方法
            virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);   // 改成虚函数，这样可以使用泛型
        protected:
            // 构造函数放在 protected 里面，即不让外界控制该构造
            InterruptHandler(myos::common::uint8_t interruptNumber, InterruptManager* interruptManager);
            ~InterruptHandler();

            myos::common::uint8_t interruptNumber;   //中断数量
            InterruptManager* interruptManager;      //中断句柄
        };

         //中断管理类
        class InterruptManager {
            friend class InterruptHandler;    // InterruptManager 类访问 InterruptHandler 的成员，需要设置友元类
        public:
            InterruptManager(myos::common::uint16_t hardwareInterruptOffset, GlobalDescriptorTable* gdt);
            ~InterruptManager();

            myos::common::uint16_t HardwareInterruptOffset(); // 获取hardwareInterruptOffset(硬件中断偏移量)的值
            // 写一个方法，让 CPU 开启中断
            void Activate();   // 启动，实现放在构造函数下面
            void Deactivate(); // 停用，如果 InterruptManager 指针已经被初始化，调用该方法

        protected:
            static InterruptManager* ActiveInterruptManager;     //启动中断管理器
            InterruptHandler* handlers[256];
            // 中断门描述符
            struct GateDescriptor {
                myos::common::uint16_t handlerAddressLowBits;    //中断处理程序地址低位
                myos::common::uint16_t gdt_codeSegmentSelector;  //gdt 代码段选择器
                myos::common::uint8_t reserved;  // 保留
                myos::common::uint8_t access;    // 访问控制
                myos::common::uint16_t handlerAddressHighBits;   //中断处理程序地址高位
            } __attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];  //中断描述符表 IDT
            // https://cdn.jsdelivr.net/gh/isanthree/blog-gallery/pic/20210610010504.png
            // 实现 IDRT 寄存器，用来定位 IDT 表的位置
            struct InterruptDescriptorTablePointer {
                myos::common::uint16_t size;
                myos::common::uint32_t base;
            } __attribute__((packed));
             // 定义中断的入口地址
            static void SetInterruptDescriptorTableEntry(
                myos::common::uint8_t interruptNumber,             // 中断编号
                myos::common::uint16_t codeSegmentSelectorOffset,  //代码段选择器偏移
                void (*handler)(),                                 //函数指针
                myos::common::uint8_t DescriptorPrivilegelLevel,   //描述符特权级别
                myos::common::uint8_t DescriptorType               //描述符类型
            );

            myos::common::uint16_t hardwareInterruptOffset;        //硬件中断偏移

            static void InterruptIgnore();                         //中断忽略

            static myos::common::uint32_t HandleInterrupt(myos::common::uint8_t interruptNumber, myos::common::uint32_t esp); // 定义一个静态函数，使其处理一些中断的请求
            myos::common::uint32_t DoHandleInterrupt(myos::common::uint8_t interruptNumber, myos::common::uint32_t esp);      // 定义一个非静态方法
            // 处理编号为 0xXX 的中断，需要实现对应的汇编
            static void HandleInterruptRequest0x00();    // 时间中断
            static void HandleInterruptRequest0x01();
            static void HandleInterruptRequest0x02();
            static void HandleInterruptRequest0x03();
            static void HandleInterruptRequest0x04();
            static void HandleInterruptRequest0x05();
            static void HandleInterruptRequest0x06();
            static void HandleInterruptRequest0x07();
            static void HandleInterruptRequest0x08();
            static void HandleInterruptRequest0x09();
            static void HandleInterruptRequest0x0A();
            static void HandleInterruptRequest0x0B();
            static void HandleInterruptRequest0x0C();
            static void HandleInterruptRequest0x0D();
            static void HandleInterruptRequest0x0E();
            static void HandleInterruptRequest0x0F();
            static void HandleInterruptRequest0x31();

            // 定义异常函数
            static void HandleException0x00();
            static void HandleException0x01();
            static void HandleException0x02();
            static void HandleException0x03();
            static void HandleException0x04();
            static void HandleException0x05();
            static void HandleException0x06();
            static void HandleException0x07();
            static void HandleException0x08();
            static void HandleException0x09();
            static void HandleException0x0A();
            static void HandleException0x0B();
            static void HandleException0x0C();
            static void HandleException0x0D();
            static void HandleException0x0E();
            static void HandleException0x0F();
            static void HandleException0x10();
            static void HandleException0x11();
            static void HandleException0x12();
            static void HandleException0x13();

            Port8BitSlow picMasterCommand;
            Port8BitSlow picMasterData;
            Port8BitSlow picSlaveCommand;
            Port8BitSlow picSlaveData;
        };
    }
}

#endif 