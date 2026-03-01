CC      = arm-none-eabi-gcc
CFLAGS  = -mcpu=cortex-m33 -mthumb -g -O0 -nostdlib -nostartfiles -mfloat-abi=soft
TARGET  = firmware.elf

all: $(TARGET)

$(TARGET): main.c startup.c link.ld
	$(CC) $(CFLAGS) -T link.ld main.c startup.c -o $(TARGET)

qemu: $(TARGET)
	qemu-system-arm -machine lm3s6965evb -cpu cortex-m33 -nographic \
	  -kernel $(TARGET) -S -s

gdb:
	gdb-multiarch -ex "target remote :1234" \
	              -ex "file firmware.elf" \
	              -ex "break main" \
	              -ex "continue" \
	              firmware.elf

clean:
	rm -f $(TARGET)