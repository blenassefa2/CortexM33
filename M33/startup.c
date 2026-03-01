extern int main(void);
extern unsigned long _estack;

void Reset_Handler(void) {
    main();
    while (1);
}

__attribute__((section(".vectors")))
void *vector_table[] = {
    (void *)&_estack,
    Reset_Handler,
};