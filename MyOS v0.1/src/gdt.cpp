#include "gdt.h"

using namespace myos;
using namespace myos::common;

GlobalDescriptorTable::GlobalDescriptorTable()
    : nullSegmentDescriptor(0, 0, 0),
    unusedSegmentDescriptor(0, 0, 0),
    codeSegmentDescriptor(0, 64 * 1024 * 1024, 0x9a),
    dataSegmentDescriptor(0, 64 * 1024 * 1024, 0x92) {
    uint32_t i[2];   // 定义存放 GDT 描述符(GDT descriptor)的数组
    i[1] = (uint32_t)this;
    i[0] = sizeof(GlobalDescriptorTable) << 16;
    asm volatile("lgdt (%0)": :"p" (((uint8_t *)i) + 2));  //C++调用ASM 内联汇编
}

GlobalDescriptorTable::~GlobalDescriptorTable() {}
// 定义函数获取数据段
uint16_t GlobalDescriptorTable::DataSegmentSelector() {
    return ((uint8_t*)&dataSegmentDescriptor - (uint8_t*)this) >> 3;  // 返回段内偏移
}
// 定义函数获取代码段
uint16_t GlobalDescriptorTable::CodeSegmentSelector() {
    return ((uint8_t*)&codeSegmentDescriptor - (uint8_t*)this) >> 3;  // 返回段内偏移
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type) {
    uint8_t* target = (uint8_t*)this;
    if (limit < 1048576) {  // 2^20=1048576
        target[6] = 0x40;   // 22号位D/B 设置为 1
    } else {
        if ((limit & 0xfff) != 0xfff) {
            limit = (limit >> 12) - 1;
        } else {
            limit = limit >> 12;
        }
        // 32位 保护模式
        target[6] = 0xC0;   // 23号位和22号位 设置为 1
    }

    target[0] = limit & 0xff;
    target[1] = (limit >> 8) & 0xff;
    target[6] |= (limit >> 16) & 0xf;

    target[2] = base & 0xff;
    target[3] = (base >> 8) & 0xff;
    target[4] = (base >> 16) & 0xff;
    target[7] = (base >> 24) & 0xff;

    target[5] = type;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base() {
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];
    return result;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit() {
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[6] & 0xf;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if ((target[6] & 0xC0) == 0xC0)
        result = (result << 12) | 0xfff;
    return result;
}