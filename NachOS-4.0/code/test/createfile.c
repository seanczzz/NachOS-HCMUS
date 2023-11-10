#include "syscall.h"
#include "copyright.h"

#define maxlen 32

int main()
{
  int a, b, c, d, rm, socket;
  /*Open a file*/
  a = Open("LeHuynhCuong.txt", 0);
  // b = Close(2);

  // Write to console
  // char buffer[] = "I am working on NachOS project\n";
  // c = Write(buffer, 50, 1);

  // Read from console
  // char buffer[] = "I am working on NachOS project\n";
  // c = Read(buffer, 100, 0);

  // Seek
  // d = Seek(-1, a);
  // PrintNum(d);

  // Remove
  // PrintNum(a);
  // PrintString("\n");
  // rm = Remove("LeHuynhCuong.txt");
  // PrintNum(rm);
  // PrintString("\n");

  PrintNum(a);
  PrintString("\n");
  socket = SocketTCP();
  PrintNum(socket);
  PrintString("\n");

  socket = SocketTCP();
  PrintNum(socket);
  PrintString("\n");

  Halt();
}