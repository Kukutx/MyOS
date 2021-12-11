.set MAGIC, 0x1badb002                      #GRUB魔术块
.set FLAGS, (1<<0 | 1<<1)                   #GRUB标志块 
.set CHECKSUM, -(MAGIC + FLAGS)             #校验块

.section .multiboot                         #multiboot规范  
  .long MAGIC
  .long FLAGS
  .long CHECKSUM

.section .text                              #.text段保存代码，是只读和可执行的，后面那些指令都属于.text段。
.extern _kernelMain                         #导入外部函数,导入_kernelMain
.extern _callConstructors                   #导入系统构造函数
.global loader                              #让链接器能看见loader

loader:
  mov $kernel_stack, %esp
  call _callConstructors                   #调用系统构造函数
  push %eax
  push %ebx
  call _kernelMain                         #调用内核入口函数

_stop:
  cli
  hlt                                       #停机指令
  jmp _stop                                 #结束

.section .bss
.space 2*1024*1024
kernel_stack: