#include "syscall.h"
#include "copyright.h"

int main(int argc, char *argv[])
{
  // file input: Hello.txt, file output: output.txt
  int inputFileId, outputFileId, socketId;
  char buffer[50];

  // PrintNum(argc);
  // PrintString("\n");
  // PrintString(argv[1]);
  // PrintString(argv[2]);

  inputFileId = Open("Hello.txt", 0);
  Read(buffer, 49, inputFileId);

  socketId = SocketTCP();

  Connect(socketId, "127.0.0.1", 1234);

  Send(socketId, buffer, 49);

  Receive(socketId, buffer, 49);

  Create("output.txt");
  outputFileId = Open("output.txt", 0);
  Write(buffer, 49, outputFileId);

  Halt();
}