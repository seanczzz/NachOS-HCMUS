#include "syscall.h"
#include "copyright.h"

#define maxlen 32

int main()
{
  int rm;
  /*Open a file*/
  int a = Open("LeHuynhCuong.txt", 0);
  // int b = Close(2);

  // Write to console
  // char buffer[] = "I am working on NachOS project\n";
  // int c = Write(buffer, 50, 1);

  // Read from console
  // char buffer[] = "I am working on NachOS project\n";
  // int c = Read(buffer, 100, 0);

  // Seek
  // int d = Seek(-1, a);
  // PrintNum(d);

  // Remove
  PrintNum(a);
  PrintString("\n");
  rm = Remove("cuong.txt");
  PrintNum(rm);

  // Segmentation fault
  Halt();
}