#ifndef __MYOS__HARDWARECOMMUNICATION__PORT_H
#define __MYOS__HARDWARECOMMUNICATION__PORT_H
/**********I/O读写端口**********/
#include "common/types.h"

namespace myos
{
    namespace hardwarecommunication
    {   // 这是一个虚拟类，不希望被实例化，使用 protected
        class Port {
        protected:
            myos::common::uint16_t portnumber;   // io 操作就是要往一个端口读或写
            Port(myos::common::uint16_t portnumber);
            ~Port();
        };
         // 8位 读取，继承 Port 类
        class Port8Bit : public Port {
        public:
            Port8Bit(myos::common::uint16_t portnumber);
            ~Port8Bit();
            // 定义写操作，使用 virtual，希望能被 overwrite。
            // 父类的指针可以使用子类的方法
            virtual void Write(myos::common::uint8_t data);
            virtual myos::common::uint8_t Read();   // 定义读操作
        protected:
            static inline myos::common::uint8_t Read8(myos::common::uint16_t _port) {
                myos::common::uint8_t result;
                __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (_port));           //内联汇编
                return result;
            }
            static inline void Write8(myos::common::uint16_t _port, myos::common::uint8_t _data) {
                __asm__ volatile("outb %0, %1" : : "a" (_data), "Nd" (_port));
            }
        };
         // 8位 读，继承 Port8Bit 类，慢操作
        class Port8BitSlow : public Port8Bit {
        public:
            Port8BitSlow(myos::common::uint16_t portnumber);
            ~Port8BitSlow();
            // 定义写操作，使用 virtual，希望能被 overwrite。
            // 父类的指针可以使用子类的方法
            virtual void Write(myos::common::uint8_t data);
        protected:
            static inline void Write8Slow(myos::common::uint16_t _port, myos::common::uint8_t _data) {
                __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (_data), "Nd" (_port));
            }
        };
         // 16位 读取，继承 Port 类
        class Port16Bit : public Port {
        public:
            Port16Bit(myos::common::uint16_t portnumber);
            ~Port16Bit();
            // 定义写操作，使用 virtual，希望能被 overwrite
            virtual void Write(myos::common::uint16_t data);
            virtual myos::common::uint16_t Read();  // 定义读操作
        protected:
            static inline myos::common::uint16_t Read16(myos::common::uint16_t _port) {
                myos::common::uint16_t result;
                __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (_port));
                return result;
            }

            static inline void Write16(myos::common::uint16_t _port, myos::common::uint16_t _data) {
                __asm__ volatile("outw %0, %1" : : "a" (_data), "Nd" (_port));
            }
        };
        // 32位 读取，继承 Port 类
        class Port32Bit : public Port {
        public:
            Port32Bit(myos::common::uint16_t portnumber);
            ~Port32Bit();
            // 定义写操作，使用 virtual，希望能被 overwrite
            virtual void Write(myos::common::uint32_t data);
            virtual myos::common::uint32_t Read();  // 定义读操作
        protected:
            static inline myos::common::uint32_t Read32(myos::common::uint16_t _port) {
                myos::common::uint32_t result;
                __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (_port));
                return result;
            }

            static inline void Write32(myos::common::uint16_t _port, myos::common::uint32_t _data) {
                __asm__ volatile("outl %0, %1" : : "a"(_data), "Nd" (_port));
            }
        };

        // 64位 读取，继承 Port 类
        class Port64Bit : public Port
        {
        public:
            Port64Bit(myos::common::uint16_t portnumber);
            ~Port64Bit();
            // 定义写操作，使用 virtual，希望能被 overwrite
            virtual void Write(myos::common::uint64_t data);
            virtual myos::common::uint64_t Read(); // 定义读操作
        };
    }
}

#endif