EXPERIMENT ?= ex_4_

# Toolchain
CC      = arm-none-eabi-gcc
GDB     = gdb
QEMU    = qemu-system-arm
MACHINE = mps2-an521

OPENOCD = openocd
OPENOCD_IF = interface/cmsis-dap.cfg
OPENOCD_TGT = target/rp2040.cfg
OPENOCD_SPEED = 5000


# Directories
INCDIRS := -I./include -I./include/cmsis -I./include/fault -I./include/multicore \
           -I./include/platform -I./include/timing -I./include/mpu

# CPU selection (RP2040 experiment needs Cortex-M0+)
CPU     = cortex-m0plus
ifeq ($(EXPERIMENT),ex_3_prime_probe)
CPU = cortex-m0plus
endif
ifeq ($(EXPERIMENT),ex_4_time)
CPU = cortex-m0plus
endif

# Compiler flags
CFLAGS  = -mcpu=$(CPU) -mthumb -g -O0  -nostdlib \
          -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables \
          -nostartfiles -mfloat-abi=soft $(INCDIRS)

# Linker (use experiment-local linker script when present)
EXP_LINKER := src/$(EXPERIMENT)/link.ld
ifeq ($(wildcard $(EXP_LINKER)),)
LDFLAGS = -T link.ld
else
LDFLAGS = -T $(EXP_LINKER)
endif
TARGET  = firmware.elf

MPU_IMPL ?= armv8m
MPU_VARIANT_SRCS := $(wildcard src/common/mpu/mpu_*.c)

# Source files (all common + experiments recursively)
# Common sources
COMMON_SRCS := $(filter-out $(MPU_VARIANT_SRCS),$(wildcard src/common/**/*.c) $(wildcard src/common/*.c) $(wildcard src/common/**/*.s)) src/common/mpu/mpu_$(MPU_IMPL).c

# Experiment-specific sources
EXP_SRCS := $(wildcard src/$(EXPERIMENT)/*.c) $(wildcard src/$(EXPERIMENT)/*.s)

SRCS := $(COMMON_SRCS) $(EXP_SRCS)
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

# QEMU Targets
qemu: $(TARGET)
	$(QEMU) -machine $(MACHINE) -nographic -semihosting-config enable=on,target=native \
	-device loader,file=$(TARGET),cpu-num=0

qemu_debug: $(TARGET)
	$(QEMU) -machine $(MACHINE) -nographic -semihosting-config enable=on,target=native \
	-device loader,file=$(TARGET),cpu-num=0 -S -s

qemu2cpu: $(TARGET)
	$(QEMU) -machine $(MACHINE) -nographic -s \
	-semihosting-config enable=on,target=native \
	-device loader,file=$(TARGET),cpu-num=0 \
	-device loader,file=$(TARGET),cpu-num=1

# Load individual CPUs
load_cpu0:
	$(QEMU) -machine $(MACHINE) -nographic \
	-device loader,file=$(TARGET),cpu-num=0 -S -s

load_cpu1:
	$(QEMU) -machine $(MACHINE) -nographic \
	-device loader,file=$(TARGET),cpu-num=1 -S -gdb tcp::1235

# GDB targets
gdb:
	$(GDB) $(TARGET) -ex "target remote:1234"

gdb1:
	$(GDB) $(TARGET) -ex "target remote:1235"

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

gdb-ocd:
	$(GDB) $(TARGET) \
	-ex "set pagination off" \
	-ex "target extended-remote :3333" \
	-ex "monitor targets rp2040.core0" \
	-ex "monitor arm semihosting enable" \
	-ex "monitor arm semihosting_fileio enable" \
	-ex "monitor targets rp2040.core1" \
	-ex "monitor arm semihosting enable" \
	-ex "monitor arm semihosting_fileio enable" \
	-ex "monitor targets rp2040.core0" \
	-ex "monitor reset halt" \
	-ex "monitor reg msp 0x20042000" \
	-ex "monitor reg pc 0x10000015" \
	-ex "continue"

dump-spy:
	$(GDB) -q $(TARGET) \
	-ex "set pagination off" \
	-ex "set confirm off" \
	-ex "target extended-remote :3333" \
	-ex "monitor halt" \
	-ex "set logging file result_analysis/data_output/spy_results.txt" \
	-ex "set logging overwrite on" \
	-ex "set logging enabled on" \
	-ex "p spy_results" \
	-ex "set logging enabled off" \
	-ex "dump binary memory result_analysis/data_output/spy_results.bin spy_results spy_results+10" \
	-ex "quit"

dump-spy-exp2:
	$(GDB) -q $(TARGET) \
	-ex "set pagination off" \
	-ex "set confirm off" \
	-ex "target remote :1234" \
	-ex "monitor halt" \
	-ex "set logging file result_analysis/data_output/spy_results.txt" \
	-ex "set logging overwrite on" \
	-ex "set logging enabled on" \
	-ex "p spy_results" \
	-ex "set logging enabled off" \
	-ex "dump binary memory result_analysis/data_output/spy_results.bin spy_results spy_results+2800" \
	-ex "quit"

# dump-spy-exp4:
#     $(GDB) -q $(TARGET) \
#     -ex "set pagination off" \
#     -ex "set confirm off" 
#     # -ex "target extended-remote :3333" \
#     # -ex "monitor halt" \
#     # -ex "set logging file result_analysis/data_output/spy_results.txt" \
#     # -ex "set logging overwrite on" \
#     # -ex "set logging enabled on" \
#     # -ex "p spy_results" \
#     # -ex "set logging enabled off" \
#     # -ex "dump binary memory result_analysis/data_output/spy_results.bin spy_results spy_results+$(shell echo $$((ITERATIONS*4)))" \
#     # -ex "quit"


clean:
	rm -f $(OBJS) $(TARGET) 

.PHONY: all clean qemu qemu_debug qemu2cpu load_cpu0 load_cpu1 gdb gdb1 ocd flash gdb-ocd dump-spy
