#include "syscall.h"
#define MAX_LENGTH 200
int main(int argc, char *argv[])
{
  char name[MAX_LENGTH + 1];
  int result;
  Write("Nhap ten file can tao: ", MAX_LENGTH, 1);
  Read(name, MAX_LENGTH, 0);
  result = Create(name);
  if (result == 0)
  {
    Write("Tao file thanh cong\n", MAX_LENGTH, 1);
  }
  else
  {
    Write("Tai file that bai\n", MAX_LENGTH, 1);
  }
  Halt();
}