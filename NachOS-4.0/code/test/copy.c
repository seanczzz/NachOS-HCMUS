#include "syscall.h"
#define MAX_LENGTH 200
int main(int argc, char *argv[])
{
  char file1[MAX_LENGTH + 1];
  char file2[MAX_LENGTH + 1];
  char nd1[MAX_LENGTH + 1];
  char nd2[MAX_LENGTH + 1];
  int size1, siz2, id1, id2;
  int kq = 0;
  Write("Nhap file1: ", MAX_LENGTH, 1);
  Read(file1, MAX_LENGTH, 0);
  Write("Nhap file2: ", MAX_LENGTH, 1);
  Read(file2, MAX_LENGTH, 0);
  id1 = Open(file1, 1);
  if (id1 != -1)
  {
    kq = Create(file2);
    if (kq != -1)
    {
      id2 = Open(file2, 0);
      if (id2 != -1)
      {
        size1 = Seek(-1, id1);
        Seek(0, id1);
        Seek(0, id2);
        Read(nd1, size1, id1);
        Write(nd1, size1, id2);
        Close(id2);
      }
      else
      {
        kq = -1;
      }
    }
    Close(id1);
  }
  else
  {
    kq = -1;
  }
  if (kq == -1)
  {
    Write("That bai\n", MAX_LENGTH, 1);
  }
  else
  {
    Write("Thanh cong\n", MAX_LENGTH, 1);
  }
  Halt();
}