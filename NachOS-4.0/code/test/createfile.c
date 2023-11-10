#include "syscall.h"
#include "copyright.h"

#define maxlen 32

int main()
{
  int a, b, c, d, rm, socketId, connect1;
  /*Open a file*/
  // a = Open("LeHuynhCuong.txt", 0);
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

  socketId = SocketTCP();
  PrintNum(socketId);
  PrintString("\n");

  connect1 = Connect(socketId, "127.0.0.1", 1234);
  PrintNum(connect1);
  PrintString("\n");
  Halt();
}