#include "syscall.h"
int main()
{
    int i;
    CreateSemaphore("ping", 1);
    for (i = 0; i < 100; i++)
    {
        Wait("ping");
        Write("A", 1, 1);
        Signal("ping");
    }
}