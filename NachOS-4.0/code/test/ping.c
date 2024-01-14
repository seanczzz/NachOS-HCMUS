#include "syscall.h"
int main()
{
    int i;
    int j;
    // CreateSemaphore("ping", 1);
    for (i = 0; i < 1000; i++)
    {
        // Wait("ping");
        // Write("A", 1, 1);

        Write("A ", 2, 1);
        // Write(" ", 1, 1);
        // Signal("ping");
    }
}