CC = /opt/cross/bin/i686-elf-gcc
AS = nasm
CFLAGS = -ffreestanding -Wall -Wextra -g -O2
LDFLAGS = -m elf_i386 -T linker.ld
ASFLAGS = -f elf32
LD = /opt/cross/bin/i686-elf-ld
REM = *.o */*.o */*/*.o */*/*/*.o

SRC_C = $(wildcard src/*.c src/*/*.c src/*/*/*.c)
SRC_S = $(wildcard src/*.s src/*/*.s)

OBJS_C = $(SRC_C:.c=.o)
OBJS_S = $(SRC_S:.s=.s.o)
OBJS = $(OBJS_C) $(OBJS_S)

all: clean srcs image

clean:
	rm -rf $(REM)

srcs: $(OBJS)

%.s.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

image:
	$(LD) $(LDFLAGS) -o kernel $(OBJS)
	mv kernel GOS/boot/kernel
	grub-mkrescue -o GOS.iso GOS/
	rm -rf $(REM)