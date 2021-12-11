#include "hardwarecommunication/interrupts.h"

using namespace myos;
using namespace myos::common;
using namespace myos::hardwarecommunication;

void printf(const char*);
void printfHex(uint8_t);

InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager) {
    this->interruptNumber = interruptNumber;
    this->interruptManager = interruptManager;
    interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler() {
    if (interruptManager->handlers[interruptNumber] == this) {
        interruptManager->handlers[interruptNumber] = 0;
    }
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp) {
    return esp;
}

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];// 存了256个中断
InterruptManager* InterruptManager::ActiveInterruptManager = 0;     // 初值为 0

void InterruptManager::SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber,
    uint16_t codeSegmentSelectorOffset,
    void (*handler)(),
    uint8_t DescriptorPrivilegelLevel,
    uint8_t DescriptorType) {
    const uint8_t IDT_DESC_PRESENT = 0x80;
    // handler 是一个指针
    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xffff;
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = ((uint32_t)handler >> 16) & 0xffff;
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;                                  // 段选择子，即段的偏移
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | ((DescriptorPrivilegelLevel & 3) << 5) | DescriptorType;  // 设置访问权限
    interruptDescriptorTable[interruptNumber].reserved = 0;                                                                         // 保留默认为 0
}
    InterruptManager::InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* gdt,TaskManger* taskManger) 
    : picMasterCommand(0x20), picMasterData(0x21), picSlaveCommand(0xA0), picSlaveData(0xA1){  // 8295A 芯片端口初始化
    this->taskManger = taskManger;
    this->hardwareInterruptOffset = hardwareInterruptOffset;
    uint16_t CodeSegment = (gdt->CodeSegmentSelector()) << 3; 
    // uint16_t CodeSegment = (gdt->CodeSegmentSelector()) >> 3;  //2 偏移 3位，修复 bug，否则无法实现中断

    const uint8_t IDT_INTERRUPT_GATE = 0xe;  // 中断门是 1110b
    for (uint16_t i = 0; i < 256; i++) {
        handlers[i] = 0;
        SetInterruptDescriptorTableEntry(i, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);  // 初始化
    }

    // 初始化处理异常  ，HandleException函数系列
    SetInterruptDescriptorTableEntry(0x00, CodeSegment, &HandleException0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x01, CodeSegment, &HandleException0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x02, CodeSegment, &HandleException0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x03, CodeSegment, &HandleException0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x04, CodeSegment, &HandleException0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x05, CodeSegment, &HandleException0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x06, CodeSegment, &HandleException0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x07, CodeSegment, &HandleException0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x08, CodeSegment, &HandleException0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x09, CodeSegment, &HandleException0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0A, CodeSegment, &HandleException0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0B, CodeSegment, &HandleException0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0C, CodeSegment, &HandleException0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0D, CodeSegment, &HandleException0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0E, CodeSegment, &HandleException0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0F, CodeSegment, &HandleException0x0F, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x10, CodeSegment, &HandleException0x10, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x11, CodeSegment, &HandleException0x11, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x12, CodeSegment, &HandleException0x12, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x13, CodeSegment, &HandleException0x13, 0, IDT_INTERRUPT_GATE);

    //初始化处理中断请求 ，//HandleInterruptRequest函数系列
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x00, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x01, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x02, CodeSegment, &HandleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x03, CodeSegment, &HandleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x04, CodeSegment, &HandleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x05, CodeSegment, &HandleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x06, CodeSegment, &HandleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x07, CodeSegment, &HandleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x08, CodeSegment, &HandleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x09, CodeSegment, &HandleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0A, CodeSegment, &HandleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0B, CodeSegment, &HandleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0D, CodeSegment, &HandleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0E, CodeSegment, &HandleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0F, CodeSegment, &HandleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x31, CodeSegment, &HandleInterruptRequest0x31, 0, IDT_INTERRUPT_GATE);
    
    // 往 8295A芯片（中断控制器）端口写东西
    // ICW1
    picMasterCommand.Write(0x11);  // 该语句是惯用写法
    picSlaveCommand.Write(0x11);
    // ICW2
    picMasterData.Write(hardwareInterruptOffset);
    picSlaveData.Write(hardwareInterruptOffset + 8);  // 8位 之后继续寻找 8位
    // ICW3
    picMasterData.Write(0x04);
    picSlaveData.Write(0x02);
    // ICW4
    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);
    // 触发中断，即不屏蔽中断请求
    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);

    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;

    asm volatile("lidt %0": :"m" (idt)); // "m"表示内存的意思，从内存读取到 lidt，将base和limit加载到idtr(register)中
}

InterruptManager::~InterruptManager() {} // 析构函数

uint16_t InterruptManager::HardwareInterruptOffset() {
    return hardwareInterruptOffset;
}
// 使 CPU 开启中断
void InterruptManager::Activate() {
    if (ActiveInterruptManager != 0) {  // 不等于 0，说明被初始化过了
        ActiveInterruptManager->Deactivate();
    }
    // 等于 0，则：
    ActiveInterruptManager = this;
    asm("sti");  // 开启所有中断
}
// 使 CPU 关闭中断
void InterruptManager::Deactivate() {
    // 等于 this，重新赋值为 0，调用汇编 "cli" 关闭中断
    if (ActiveInterruptManager == this) {
        ActiveInterruptManager = 0;
        asm("cli"); //关闭所有中断
    }
}

uint32_t InterruptManager::HandleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    if (ActiveInterruptManager != 0) {
        return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
    }
    // printf("\n按下键盘的结果：中断被触发.\n");
    return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    if (handlers[interruptNumber] != 0) {  //如果存活的话
        esp = handlers[interruptNumber]->HandleInterrupt(esp);  // 相当于恢复现场的操作 PS:中断(4)25:10
    } else if (interruptNumber != hardwareInterruptOffset) {    // 如果不是时钟中断（时钟中断是从0开始的）
        printf("UNHANDLED INTERRUPT 0X");
        printfHex(interruptNumber);
    }
    if (interruptNumber == hardwareInterruptOffset) {
        esp = (uint32_t)taskManger->Schedule((CPUState*)esp);
    }
    // 判断是否为硬件中断，如果是的话：
    if (hardwareInterruptOffset <= interruptNumber && interruptNumber < hardwareInterruptOffset + 16) {
        picMasterCommand.Write(0x20); // EOI 对应的 OCW2 的值为 0x20
        if (hardwareInterruptOffset + 8 <= interruptNumber) {
            picSlaveCommand.Write(0x20); // 修复鼠标无法移动的 bug
        }
    }
    return esp;
}

// 实现中断处理函数，注释掉不需要，因为是在汇编里面实现
// 这里是为了得到汇编命名，用完注释
// void InterruptManager::HandleInterruptRequest0x00() {}

// 实现异常函数，注释掉不需要，因为是在汇编里面实现了
// 这里是为了得到汇编命名，用完注释
// void InterruptManager::HandleException0x00() {}

// 注释掉不需要，因为是在汇编里面实现了
// 这里是为了得到汇编命名，用完注释
// void InterruptManager::InterruptIgnore() {}
