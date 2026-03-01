# CortexM MPU programming experiment

So I am trying to understand MPU configuration and security of embedded IoT devices and this repository shows the learning process and working implementations of configurations, cpu emulation and debugging.

I am trying to follow [Arm's developer tutorial](https://developer.arm.com/documentation/ka005771/latest/) on MPU programming but with open source resources

Part 1:  (ARM CortexM M3 CPU on lm3s6965evb machine(board))
- create a very simple firmware
- emulate it on qemu
- debug with gdb
    - have breakpoint on main and make the debugger reach this main without any fault

__issues faced__
- I started this part with Cortex M33 CPU and mps2-an505 machine(board) but qemu didn't understand proper way of emulating this machine
- Instead of setting up the stack pointer properly based on the vector table it kept using the vector table as a kernel code to run and it hardfaults even before beginning anything
- **Trials to solve this issue**:  I thought CortexM33 was maybe too complex so I used CortexM3 Cpu and the simple good board for this CPU was lm3s6965evb so I switched the board as well. But before this decision 
    - I tried changing only the board with other ARM boards or tried to update the way vector table is configured or fault handling was done and none of them were correct
- **Conclusion** The board(machine) is not well supported by qemu

__Folder__
[M3](/M3/)

Part 2: (ARM CortexM M33 CPU on lm3s6965evb machine(board))
- Do the same as part 1 but with the M33 cpu
- Identify the MPU_TYPE configuration
- complete Exercise 1 - Find out how many MPU Regions are present in your Device [Arm's developer tutorial](https://developer.arm.com/documentation/ka005771/latest/)
- Apparently the lm3s6965evb machine does work with M33 CPU so I only had to change the cpu flag on qemu for it to correctly boot
- For identifying MPU registers I need to check the MPU register memory location from ARM since these registers are Memory Mapped registers
    - Hardware registers are placed inside the same address space as RAM — so you access hardware by reading/writing memory addresses.
    - You can't simply see their value using ```info registers``` command on gdb
    - instead you use the examine gdb command
    ```gdb
    x / x  w 0xE000ED90 
    │  │   │		│ 
    │  │   │		└── address to read from 
    │  │   └───── size of each unit 
    │  └──────── format to display in (x implies hexadecimal format)
    └────────────── examine command

    ```
__Folder__
[M33](/M33/)