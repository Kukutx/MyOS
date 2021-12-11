/* coding: utf-8 */

#include "hardwarecommunication/port.h"

using namespace myos::common;
using namespace myos::hardwarecommunication;

Port::Port(uint16_t portnumber)
    : portnumber(portnumber) {}

Port::~Port() {}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Port8Bit ...
// Port8Bit类构造函数，调用基类的构造函数
Port8Bit::Port8Bit(uint16_t portnumber)
    : Port(portnumber) {}

Port8Bit::~Port8Bit() {}

// 汇编进行操纵端口进行写操作
void Port8Bit::Write(uint8_t data){
    // 向端口 portnumber 写入一个数据（data）
    Write8(portnumber, data);
}

uint8_t Port8Bit::Read(){
    return Read8(portnumber);
}
// ... Port8Bit >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Port8BitSlow ...
// Port8BitSlow类构造函数，调用基类的构造函数
Port8BitSlow::Port8BitSlow(uint16_t portnumber)
    : Port8Bit(portnumber) {}

Port8BitSlow::~Port8BitSlow() {}

// 汇编进行操纵端口进行写操作
void Port8BitSlow::Write(uint8_t data){
    Write8Slow(portnumber, data);
}
// ... Port8BitSlow >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Port16Bit ...
// Port16Bit类构造函数，调用基类的构造函数
Port16Bit::Port16Bit(uint16_t portnumber)
    : Port(portnumber) {}

Port16Bit::~Port16Bit() {}

// 汇编进行操纵端口进行写操作
void Port16Bit::Write(uint16_t data){
    Write16(portnumber, data);
}

uint16_t Port16Bit::Read(){
    return Read16(portnumber);
}
// ... Port16Bit >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Port32Bit ...
// Port32Bit类构造函数，调用基类的构造函数
Port32Bit::Port32Bit(uint16_t portnumber)
    : Port(portnumber) {}

Port32Bit::~Port32Bit() {}

// 汇编进行操纵端口进行写操作
void Port32Bit::Write(uint32_t data){
    Write32(portnumber, data);
}

uint32_t Port32Bit::Read(){
    return Read32(portnumber);
}
// ... Port32Bit >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
