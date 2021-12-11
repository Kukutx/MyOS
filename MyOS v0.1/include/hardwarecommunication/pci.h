#ifndef __MYOS__HARDWARECOMMUNICATION__PCI_H
#define __MYOS__HARDWARECOMMUNICATION__PCI_H
/**********PCI总线**********/
#include "hardwarecommunication/port.h"
#include "common/types.h"
#include "interrupts.h"
#include "drivers/driver.h"


namespace myos
{
    namespace hardwarecommunication
    {   //基址寄存器类型
        enum BaseAddressRegisterType {
            MemoryMapping = 0,
            InputOutput = 1
        }; 
        //基址寄存器
        class BaseAddressRegister {
        public:
            bool prefetchable;                        //预取
            myos::common::uint8_t* address;           //地址
            myos::common::uint32_t size;              //大小
            BaseAddressRegisterType type;             //类型
        };
        //外围组件互连设备描述符
        class PeripheralComponentInterconnectDeviceDescriptor {
        public:
            PeripheralComponentInterconnectDeviceDescriptor();
            ~PeripheralComponentInterconnectDeviceDescriptor();

            myos::common::uint32_t portBase;  // 基址
            myos::common::uint32_t interrupt; // 中断

            myos::common::uint8_t bus;
            myos::common::uint8_t device;     //设备
            myos::common::uint8_t function;   //功能

            myos::common::uint16_t device_id; // 设备号；0 和 0xffff 都是无效的
            myos::common::uint16_t vendor_id; // 供应商号

            myos::common::uint8_t class_id;
            myos::common::uint8_t subclass_id;
            myos::common::uint8_t interface_id; //接口号
            myos::common::uint8_t revision;     //修订
        };

        class PeripheralComponentInterconnectController {
        public:
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();
            //读取
            myos::common::uint32_t Read(
                myos::common::uint8_t bus, 
                myos::common::uint8_t device,          //设备
                myos::common::uint8_t function,        //功能
                myos::common::uint8_t registeroffset); // 寄存器偏移
            //写入
            void write(myos::common::uint8_t bus, 
                myos::common::uint8_t device,          //设备
                myos::common::uint8_t function,        //功能
                myos::common::uint8_t registeroffset,  //寄存器偏移
                myos::common::uint32_t value);
            //设备具有功能
            bool DeviceHasFunctions(myos::common::uint8_t bus, myos::common::uint8_t device); // 0000000
            //选择驱动程序
            void SelectDrivers(myos::drivers::DriverManager *driverManager);
            // 获取 设备描述符
            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(myos::common::uint8_t bus, 
                myos::common::uint8_t device,          //设备
                myos::common::uint8_t function);       //功能
            BaseAddressRegister GetBaseAddressRegister(myos::common::uint8_t bus, 
                myos::common::uint8_t device,          //设备
                myos::common::uint8_t function,        //功能
                myos::common::uint8_t bar);
        private:
            Port32Bit dataPort;        //数据端口
            Port32Bit commandPort;     //命令端口
        };
    }
}

#endif