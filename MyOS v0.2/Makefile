GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -fleading-underscore -fno-exceptions -fno-builtin -nostdlib -fno-rtti -fno-pie

ASPARAMS = --32
LDPARAMS = -melf_i386 -no-pie

objects = obj/loader.o \
          obj/gdt.o \
		  obj/memorymanagement.o \
          obj/drivers/driver.o \
          obj/hardwarecommunication/port.o \
          obj/hardwarecommunication/interruptstubs.o \
          obj/hardwarecommunication/interrupts.o \
		  obj/hardwarecommunication/pci.o \
		  obj/multitasking.o \
		  obj/drivers/amd_am79c973.o \
          obj/drivers/keyboard.o \
          obj/drivers/mouse.o \
		  obj/drivers/vga.o \
		  obj/gui/widget.o \
		  obj/gui/window.o \
		  obj/gui/desktop.o \
		  obj/net/etherframe.o \
		  obj/net/arp.o \
		  obj/net/ipv4.o \
		  obj/net/icmp.o \
		  obj/net/udp.o \
		  obj/net/tcp.o \
          obj/kernel.o
#编译cpp文件
obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ ${GPPPARAMS} -o $@ -c $<
#编译asm文件
obj/%.o: src/%.s
	mkdir -p $(@D)	
	as ${ASPARAMS} -o $@ $<
#链接.o文件到.bin二进制文件
mykernel.bin: linker.ld ${objects}
	ld ${LDPARAMS} -T $< -o $@ ${objects}
#下载文件到目录下
install: mykernel.bin
	sudo cp $< /boot/mykernel.bin
#编译二进制文件为iso格式
mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "myos" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso
#开启ubuntu内的虚拟机并开启myjbos系统
run: mykernel.iso
	(killall virtualboxvm && sleep 1) || true
	virtualboxvm --startvm "myos" &
#清除缓存文件
.PHONY: clean
clean:
	rm -rf mykernel.bin mykernel.iso obj