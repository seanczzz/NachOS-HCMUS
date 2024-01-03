#include "syscall.h"
#include "copyright.h"

#define maxlen 32

int main()
{
  int newProc1;
  newProc1 = Exec("cat"); // Project 01

  PrintNum(newProc1);
  Halt();
}