# 编译参数
GCCPARAMS = -m16 -march=i386 -masm=intel -nostdlib -ffreestanding -mpreferred-stack-boundary=2 -lgcc -shared -fno-pic
ASPARAMS = -f elf32
LDPARAMS = -m elf_i386 -N -Ttext 0x8000 --oformat binary

# 创建目录
createdir:
	mkdir src
	mkdir temp
	mkdir obj

# 生成内核二进制文件
osKernel.bin:
	nasm ${ASPARAMS} osKernel.asm -o ./obj/osKernel.o
	nasm ${ASPARAMS} liba.asm -o ./obj/liba.o
	gcc -c ${GCCPARAMS} libc.c -o ./obj/libc.o
	ld ${LDPARAMS} ./obj/osKernel.o ./obj/liba.o ./obj/libc.o -o ./temp/osKernel.bin
	
#二进制文件生成
bootBin:
	nasm loader.asm -o ./temp/loader.bin
	nasm mbr.asm -o ./temp/mbr.bin
	nasm pm.asm -o ./temp/pm.bin
	make osKernel.bin

# 清除缓存
clean:
	rm -rf src
	rm -rf temp
	rm -rf obj

#写入数据
add:
	dd if=./temp/mbr.bin of=./src/MyTinyOS.vhd bs=512 count=1 conv=notrunc,noerror
	dd if=./temp/loader.bin of=./src/MyTinyOS.vhd bs=512 count=1 seek=2 conv=notrunc,noerror
	dd if=./temp/osKernel.bin of=./src/MyTinyOS.vhd bs=512 count=100 seek=9 conv=notrunc,noerror
	dd if=./temp/pm.bin of=./src/MyTinyOS.vhd bs=512 count=100 seek=20 conv=notrunc,noerror

# 复制空的样本
backup:
	cp -r ./Backup/MyTinyOS\(empty\).vhd ./src/MyTinyOS.vhd

# 生成内核
all:	
	make clean
	make createdir
	make backup
	make bootBin
	make add
	@echo "\n\n\n\nhave Dome!"
