#include "syscall.h"
#include "copyright.h"

#define maxlen 32

int main()
{
  int len;

  /*Create a file*/
  int a = Open("LeHuynhCuong.txt", 0);
  // int b = Close(2);

  char buffer[] = "dsddadassad";
  int c = Write(buffer, 50, a);

  Halt();
}