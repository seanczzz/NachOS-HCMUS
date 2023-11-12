#include "syscall.h"
#include "copyright.h"

#define MAX_LENGTH 200

int main()
{
  char fileName[MAX_LENGTH + 1];
  int id, rm;

  Write("Nhap file can xoa: ", MAX_LENGTH, 1);
  Read(fileName, MAX_LENGTH, 0);

  // Remove
  id = Open(fileName, 0);
  if (id != -1)
  {
    rm = Remove(fileName);
    if (rm == 0)
    {
      Write("Thanh cong\n", MAX_LENGTH, 1);
    }
    else
    {
      Write("That bai\n", MAX_LENGTH, 1);
    }
  }
  else
  {
    Write("That bai\n", MAX_LENGTH, 1);
  }
  Halt();
}