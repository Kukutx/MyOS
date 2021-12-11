## MyOS

我参考了网上的教程实现的简易命令框操作系统，请在**Linux**平台下使用

**参考连接：**

- http://wyoos.org/index.php
-  https://www.youtube.com/watch?v=1rnA6wpF0o4&list=PLHh55M_Kq4OApWScZyPl5HhgsTJS9MZ6M

#### 如何操作？

```makefile
#1.下载该项目       git clone https://github.com/Kukutx/MyOS.git
#2.进入根目录       cd MyOS
#3.编译源文件       make
#4.生成.iso虚拟盘   make mykernel.iso
#5.打开VM or virtualbox新建一个虚拟机,用类型other 版本Dos 然后启动虚拟机
```

#### 编译时可用的命令：

```
> make                 # 只编译源码
> make mykernel.bin    # 编译二进制文件
> make mykernel.iso    # 生成.iso虚拟盘
> make install         # 下载文件到boot目录下             (没啥用的)
> make run             # 编译并运行，默认使用virtualbox虚拟机运行
> make clean           # 清除编译产生的对象文件以及可执行文件
```
