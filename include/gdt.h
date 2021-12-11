#ifndef __GDT_H
#define __GDT_H
/**********GDT 全局描述符表**********/
#include "common/types.h"

namespace myos
{
    // 定义 GDT，全局描述符表
    class GlobalDescriptorTable
    {
    public:
        // 定义段描述符
        class SegmentDescriptor
        {
        public:
            SegmentDescriptor(myos::common::uint32_t base, myos::common::uint32_t limit, myos::common::uint8_t type);
            myos::common::uint32_t Base();  // return base address
            myos::common::uint32_t Limit(); // return limit

        private:
            myos::common::uint16_t limit_lo;      // Segment Limit 15..0，两个字节构成
            myos::common::uint16_t base_lo;       // Base Address 15..0，两个字节构成
            myos::common::uint8_t base_hi;        // Base 23..16，一个字节构成
            myos::common::uint8_t type;           // TYPE
            myos::common::uint8_t flags_limit_hi; // 图4-13 的 23..16
            myos::common::uint8_t base_vhi;       // Base 31..24，一个字节构成
        } __attribute__((packed));                // __attribute__((packed))：编译器不要进行内存对齐

        SegmentDescriptor nullSegmentDescriptor;   // 定义空的段描述符
        SegmentDescriptor unusedSegmentDescriptor; // 定义未使用的段描述符
        SegmentDescriptor codeSegmentDescriptor;   // 定义代码段
        SegmentDescriptor dataSegmentDescriptor;   // 定义数段

    public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();

        // 定义函数获取代码段
        myos::common::uint16_t CodeSegmentSelector(); 
        // 定义函数获取数据段
        myos::common::uint16_t DataSegmentSelector();
    };
}

#endif