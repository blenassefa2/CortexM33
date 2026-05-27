# Cortex-M MPU & Side-Channel Experiments

This repository explores memory protection and side channels on ARM Cortex-M systems using bare-metal firmware and emulation.

## Goals

- Understand **Memory Protection Unit (MPU)** behavior on Cortex-M
- Explore **multi-core interactions** (Cortex-M33 dual-core)
- Investigate **timing side channels** using hardware features (e.g., DWT, SysTick)
- Build reproducible experiments using open-source tooling


## Getting Started

Clone the repository:

```bash
git clone  https://github.com/blenassefa2/CortexM_MPU_practice
cd CortexM_MPU_practice
```

---

### Selecting an Experiment

Choose the experiment by editing the `EXPERIMENT` variable inside the `Makefile`.

Example:

```make
EXPERIMENT ?= experiment_0
```

Or by specifying the experiment in each target command

```bash
make EXPERIMENT=ex_4_time
```

Available experiments:

#### QEMU Experiments

These run fully in QEMU:

```text
experiment_0
experiment_1
experiment_2
```

#### RP2040 Hardware Experiments

These require a Raspberry Pi Pico (RP2040) and OpenOCD:

```text
ex_3_prime_probe
ex_4_time
ex_5_flush
ex_6_flush_reload
```

---

### QEMU Workflow

Build the firmware:

```bash
cd experiments/multicore
make
```

Run in QEMU:

```bash
make qemu
```

Run in QEMU with GDB support:

```bash
make qemu_debug
```

Connect GDB:

```bash
make gdb
```

Run dual-core QEMU:

```bash
make qemu2cpu
```

---

### RP2040 Hardware Workflow

These experiments use:

- Raspberry Pi Pico (RP2040)
- CMSIS-DAP debug probe
- OpenOCD
- GDB

Build the firmware:

```bash
make EXPERIMENT=ex_4_time
```

Flash firmware:

```bash
make flash
```

Start OpenOCD:

```bash
make ocd
```

Start GDB debugging session on another terminal:

```bash
make gdb-ocd
```

Dump spy results:

```bash
make dump-spy
```

Output files are stored in:

```text
result_analysis/data_output/
```

---

### Cleaning

```bash
make clean
```

thorough cleaning

```bash
make EXPERIMENT=ex_4_... clean
```

##  Key Topics

- MPU configuration and region isolation
- TrustZone basics (secure vs non-secure execution)
- Dual-core synchronization and shared memory
- Timing measurements using:
  - SysTick
  - DWT cycle counter
- Side-channel primitives:
  - Shared memory contention
  - Cross-core timing leakage


## Tooling

- QEMU (ARM Cortex-M emulation)
- GDB (low-level debugging)
- CMSIS (ARM device headers)
- Custom bare-metal runtime


## Repository Structure

```
.
├── Readme.md  
├── setup/                  # QEMU setup and board experiments (Part 1: Understanding Microcontrollers)
|
└── Two_Cores/              # Mostly two core configurations with multiple side channel attack experiments (Part 2: MCU setup and Side channel attack implementations)
    |
    ├── python/             
    │   ├── plot_timing.py
    │   └── requirements.txt
    |
    ├──  README.md
    ├──  Makefile           # Includes targets for each experiment
    ├──  link.ld            # default linker script when not specified in experiment_*/ folders
    ├── include/
    │   ├── cmsis/          # some arm libraries
    │   ├── fault/          # hardfault and memfault handling algorithms
    │   ├── multicore/      # multicore initiation logics
    │   ├── platform/       # specification of boards or cores ... (Raspbpico, an521, cortexm33, cortexm0+)
    │   ├── timing/         # different timing options (for example: systick , dwt)
    │   ├── mpu/            # different mpu set up for different architectures (for example: armv6, armv8)
    │   ├── print.h
    │   └── shared.h        # global variables for testing core concurrency ...
    │
    └── src/
        ├──  common/        # implementations of the included libraries
        |    ├── fault/
        |    ├── multicore/
        |    ├── platform/
        |    └── timing/
        ├── experiment_1/
        |   ├── Notes.md
        |   ├── 0boot.s
        │   ├── main.c
        │   ├── user.c
        │   └── user_caller.s
        │
        ├── experiment_2/
        |   ├── Notes.md
        |   ├── link.ld     # Custom linker script
        |   ├── 0boot.s
        │   ├── cpu0.c
        │   └── cpu1.c      # Notes, diagrams, and explanations
        ...
        └── experiment_n/




```

## Example Experiments

[MPU Basics](Setups/README.md)
- Detect number of MPU regions
- Configure memory regions
- Trigger and debug faults

Dual-Core Execution
- Boot secondary core
- Synchronize cores using shared memory

Side Channels
- Measure timing differences across cores
- Explore cache and memory contention effects
- Investigate MPU-related timing leakage


# References
- ARM MPU Programming Guide (to be linked)
- CMSIS Documentation (to be linked)
- QEMU ARM System Emulator (to be linked)
- Research Paper1  (to be linked)
- Research Paper2 (to be linked)
- Research Paper3 (to be linked)
- Research Paper4 (to be linked)