#include "syscall.h"
#include "copyright.h"

#define maxlen 32

int main()
{
  int createRes, a, b, c, d, rm,
      socketId1, connect1, numBytesSent1, numBytesRecv1, close1;

  int numClient = 4, count = 0;
  char *messages[4] = {"Le Huynh Cuong", "Nguyen Tan Hung", "Tran Minh Quang", "do an NachOS he dieu hanh"};
  char buffer[50];
  // char buffer[] = "I am working on NachOS project\n"; // only for write to console or file

  // /*Create and Open a file*/
  // createRes = Create("HuynhCuong.txt");
  // a = Open("Hello.txt", 0);
  // b = Close(a);
  // PrintNum(b);
  // PrintString("\n");

  // // Write to console
  // c = Write(buffer, 50, 1);

  // // Write to file
  // c = Write(buffer, 50, a);

  // // Read from console
  // c = Read(buffer, 49, 0);
  // PrintString("Chuoi doc duoc tu console: ");
  // PrintString(buffer);

  // // Read from file
  // c = Read(buffer, 49, a);
  // PrintString("Chuoi doc duoc tu file: ");
  // PrintString(buffer);
  // PrintString("\n");

  // // Seek
  // d = Seek(-1, a);
  // PrintNum(d);

  // Remove
  // a = Open("a.txt", 0);
  // rm = Remove("a.txt");
  // PrintNum(rm);
  // PrintString("\n");

  /////////////////////////////////////////////////////
  //--------------SOCKET TEST 1--------------------------
  for (count = 0; count < numClient; count++)
  {
    // Create socket
    socketId1 = SocketTCP();
    // PrintNum(socketId1);
    // PrintString("\n");

    // Connect to server
    connect1 = Connect(socketId1, "127.0.0.1", 1234);
    // PrintNum(connect1);
    // PrintString("\n");

    // Send data to server
    numBytesSent1 = Send(socketId1, messages[count], 27);
    PrintNum(numBytesSent1);
    PrintString("\n");

    // Receive data from server
    numBytesRecv1 = Receive(socketId1, buffer, 25);
    PrintNum(numBytesRecv1);
    PrintString("\n Receiving from server: ");
    PrintString(buffer);
    PrintString("\n");

    // Close connection
    close1 = Close(socketId1);
    PrintString("close: ");
    PrintNum(close1);
    PrintString("\n");

    PrintString("-------------------------\n");
  }
  /////////////////////////////////////////////////////

  Halt();
}