#include "syscall.h"
#define MAX_LENGTH 200
int main()
{
  char name[MAX_LENGTH + 1];
  char nd[MAX_LENGTH];
  int sizefile;
  int id;
  Write("Nhap ten file: ", MAX_LENGTH, 1);
  Read(name, MAX_LENGTH, 0);
  id = Open(name, 1);
  if (id != -1)
  {
    sizefile = Seek(-1, id);
    Seek(0, id);
    Write("Noi dung cua file: ", MAX_LENGTH, 1);
    Read(nd, sizefile, id);
    Write(nd, sizefile, 1);
    Write("\n", 1, 1);
    Close(id);
  }
  else
  {
    Write("Mo file that bai\n", MAX_LENGTH, 1);
  }
  Halt();
}