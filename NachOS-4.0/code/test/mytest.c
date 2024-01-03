#include "syscall.h"
#include "copyright.h"

#define maxlen 32

int main()
{
  int newProc1, exitCode1;
  newProc1 = Exec("cat"); // Project 01

  exitCode1 = Join(newProc1);

  Exit(exitCode1);
  Halt();
}