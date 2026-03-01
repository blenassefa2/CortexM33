int main(void) {
    volatile int x = 42;
    volatile int y = x + 1;
    (void)y;
    while (1);
}