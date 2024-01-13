#include "syscall.h"
#define MAX_LENGTH 200
int main()
{
  int pingPID, pongPID;
  // Write("Ping pong test starting...\n\n", 200, 1);
  pingPID = Exec("ping");
  pongPID = Exec("pong");

  Join(pingPID);
  Join(pongPID);
}