#include <stdlib.h>

__attribute__((noinline))
void leak(void)
{
    volatile char *p = malloc(1024);
    p[0] = 42;
}

int main(void)
{
    leak();
    return 0;
}
