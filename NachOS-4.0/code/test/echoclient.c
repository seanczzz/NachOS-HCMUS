#include "syscall.h"
#include "copyright.h"

int main()
{
  int socketId1, connect1, numBytesSent1, numBytesRecv1, close1;

  int numClient = 4, count = 0;
  char *messages[4] = {"Le Huynh Cuong", "Nguyen Tan Hung", "Tran Minh Quang", "do an NachOS he dieu hanh"};
  char buffer[50];

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