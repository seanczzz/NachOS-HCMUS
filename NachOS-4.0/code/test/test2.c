#include "syscall.h"
#define MAX_LENGTH 200
int main()
{
  int pingPID, pongPID;
  CreateSemaphore("catcop", 1);
  // Write("Ping pong test starting...\n\n", 200, 1);
  pingPID = Exec("cat");
  pongPID = Exec("copy");

  Join(pingPID);
  Join(pongPID);
}