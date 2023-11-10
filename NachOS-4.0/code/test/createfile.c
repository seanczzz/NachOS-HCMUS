#include "syscall.h"
#include "copyright.h"

#define maxlen 32

int main()
{
  int a, b, c, d, rm,
      socketId1, connect1, numBytesSent1, numBytesRecv1;

  char buffer[50];
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

  // Create socket
  socketId1 = SocketTCP();
  PrintNum(socketId1);
  PrintString("\n");

  // Connect to server
  connect1 = Connect(socketId1, "127.0.0.1", 1234);
  PrintNum(connect1);
  PrintString("\n");

  // Send data to server
  numBytesSent1 = Send(socketId1, "Le Huynh Cuong", 16);
  PrintNum(numBytesSent1);
  PrintString("\n");

  // Receive data from server
  numBytesRecv1 = Receive(socketId1, buffer, 25);
  PrintNum(numBytesRecv1);
  PrintString("\n Receiving from server: ");
  PrintString(buffer);
  PrintString("\n");

  Halt();
}