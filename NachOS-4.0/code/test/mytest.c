#include "syscall.h"
#include "copyright.h"

#define maxlen 32

int main()
{
  // int newProc1, exitCode1;
  // newProc1 = Exec("cat"); // Project 01

  // exitCode1 = Join(newProc1);

  // Exit(exitCode1);

  int sem, wait, signal;
  sem = CreateSemaphore("semaphore", 1);
  PrintNum(sem);
  PrintString("\n");

  // wait = Wait("semaphoree");
  // PrintNum(wait);
  // PrintString("\n");

  signal = Signal("abc");
  PrintNum(signal);
  PrintString("\n");

  Halt();
}