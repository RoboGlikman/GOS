CC = /opt/cross/bin/i686-elf-gcc
AS = nasm
CFLAGS = -ffreestanding -Wall -Wextra -g -O2
LDFLAGS = -m elf_i386 -T linker.ld
ASFLAGS = -f elf32
LD = /opt/cross/bin/i686-elf-ld
REM = *.o */*.o */*/*.o Gos/boot/kernel

SRC_C = src/kernel.c src/vga/vga.c src/gdt/gdt.c src/util/util.c src/idt/idt.c src/timer/timer.c src/stdlib/stdio.c src/keyboard/keyboard.c src/memory/memory.c src/kmalloc/kmalloc.c src/stdlib/string.c
SRC_S = src/boot.s src/gdt/gdt.s src/idt/idt.s

OBJS_C = $(SRC_C:.c=.o)
OBJS_S = $(SRC_S:.s=.s.o)
OBJS = $(OBJS_C) $(OBJS_S)

all: clean kernel image

clean:
	rm -rf $(REM)

kernel: $(OBJS)

%.s.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

image:
	$(LD) $(LDFLAGS) -o kernel $(OBJS)
	mv kernel Gos/boot/kernel
	grub-mkrescue -o Gos.iso Gos/
	rm -rf $(REM)