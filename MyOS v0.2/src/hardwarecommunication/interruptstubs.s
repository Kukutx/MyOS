.set IRQ_BASE, 0x20
.section .text
.extern __ZN4myos21hardwarecommunication16InterruptManager15HandleInterruptEhj

.macro HandleInterruptRequest num
.global __ZN4myos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev  #声明该函数   HandleInterruptRequest函数系列
__ZN4myos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:         #函数定义
    movb $\num + IRQ_BASE, (interruptnumber)
    pushl $0
    jmp int_bottom
.endm

.macro HandleException num
.global __ZN4myos21hardwarecommunication16InterruptManager19HandleException\num\()Ev       #HandleException函数系列
__ZN4myos21hardwarecommunication16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x02
HandleInterruptRequest 0x03
HandleInterruptRequest 0x04
HandleInterruptRequest 0x05
HandleInterruptRequest 0x06
HandleInterruptRequest 0x07
HandleInterruptRequest 0x08
HandleInterruptRequest 0x09
HandleInterruptRequest 0x0A
HandleInterruptRequest 0x0B
HandleInterruptRequest 0x0C
HandleInterruptRequest 0x0D
HandleInterruptRequest 0x0E
HandleInterruptRequest 0x0F
HandleInterruptRequest 0x31

HandleException 0x00
HandleException 0x01
HandleException 0x02
HandleException 0x03
HandleException 0x04
HandleException 0x05
HandleException 0x06
HandleException 0x07
HandleException 0x08
HandleException 0x09
HandleException 0x0A
HandleException 0x0B
HandleException 0x0C
HandleException 0x0D
HandleException 0x0E
HandleException 0x0F
HandleException 0x10
HandleException 0x11
HandleException 0x12
HandleException 0x13


int_bottom:
    pushl %ebp                       #pushl就相当于intel汇编中的push dword
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx 
    pushl %ebx 
    pushl %eax 

    pushl %esp
    push (interruptnumber)
    call __ZN4myos21hardwarecommunication16InterruptManager15HandleInterruptEhj

    movl %eax, %esp
    
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx 

    popl %esi
    popl %edi
    popl %ebp

    add $4, %esp

.global __ZN4myos21hardwarecommunication16InterruptManager15InterruptIgnoreEv
__ZN4myos21hardwarecommunication16InterruptManager15InterruptIgnoreEv:

    iret

.data
    interruptnumber: .byte 0
