

# Toolchain
CC      = arm-none-eabi-gcc
GDB     = gdb-multiarch
QEMU    = qemu-system-arm
MACHINE = mps2-an521
OPENOCD = openocd
OPENOCD_IF = interface/cmsis-dap.cfg
OPENOCD_TGT = target/rp2040.cfg
OPENOCD_SPEED = 5000


# no includes for now


# CPU selection (RP2040 experiment needs Cortex-M0+)
CPU     = cortex-m0plus


# Compiler flags
CFLAGS  = -mcpu=$(CPU) -mthumb -g -O0 -nostdlib \
          -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables \
          -nostartfiles -mfloat-abi=soft

# Linker
LDFLAGS = -T link.ld
TARGET  = firmware.elf


# Source files
EXP_SRCS := blink.c 0boot.s

SRCS := $(EXP_SRCS)
OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)

# Build rules
all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@


# GDB targets
gdb:
	$(GDB) $(TARGET) -ex "target remote:3333"



# Probe Pico (CMSIS-DAP) + target Pico (RP2040)
ocd:
	$(OPENOCD) -f $(OPENOCD_IF) -f $(OPENOCD_TGT) -c "adapter speed $(OPENOCD_SPEED)"

flash: $(TARGET)
	$(OPENOCD) -f $(OPENOCD_IF) -f $(OPENOCD_TGT) \
	-c "adapter speed $(OPENOCD_SPEED)" \
	-c "program $(TARGET) verify" \
	-c "reset halt" \
	-c "reg sp 0x20042000" \
	-c "reg pc 0x10000015" \
	-c "resume" \
	-c "shutdown"

# Clean
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean qemu qemu_debug qemu2cpu load_cpu0 load_cpu1 gdb gdb1 ocd flash
