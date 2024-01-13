#include "syscall.h"
int main()
{
    int i;
    CreateSemaphore("pong", 1);
    for (i = 0; i < 100; i++)
    {
        Wait("pong");
        Write("B", 1, 1);
        Signal("pong");
    }
}