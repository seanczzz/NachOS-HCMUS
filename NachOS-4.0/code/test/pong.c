#include "syscall.h"
int main()
{
    int i;
    int j;
    int c;
    // CreateSemaphore("pong", 1);
    for (i = 0; i < 1000; i++)
    {
        // Wait("pong");
        // Write("B", 1, 1);

        Write("B ", 2, 1);
        // Write(" ", 1, 1);
        // Signal("pong");
    }
}