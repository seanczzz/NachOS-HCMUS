#include "syscall.h"
#define MAX_LENGTH 200
int main(int argc, char *argv[])
{
  char file1[MAX_LENGTH + 1];
  char file2[MAX_LENGTH + 1];
  char nd1[MAX_LENGTH + 1];
  char nd2[MAX_LENGTH + 1];
  int size1;
  int size2;
  int id1;
  int id2;
  int idOut;
  int kq = 1;
  Write("Nhap file1: ", MAX_LENGTH, 1);
  Read(file1, MAX_LENGTH, 0);
  Write("Nhap file2: ", MAX_LENGTH, 1);
  Read(file2, MAX_LENGTH, 0);

  Create("concatOut.txt");
  idOut = Open("concatOut.txt", 0);

  id1 = Open(file1, 1);
  if (id1 != -1)
  {
    id2 = Open(file2, 0);
    if (id2 != -1)
    {
      size1 = Seek(-1, id1);
      size2 = Seek(-1, id2);

      // Read file1's content
      Seek(0, id1);
      Read(nd1, size1, id1);

      // Read file2's content
      Seek(0, id2);
      Read(nd2, size2, id2);

      // Write to output file
      Seek(0, idOut);
      Write(nd1, size1, idOut);

      Seek(-1, idOut);
      Write(nd2, size2, idOut);
      //

      Close(idOut);
      Close(id2);
    }
    else
    {
      kq = 0;
    }
    Close(id1);
  }
  else
  {
    kq = 0;
  }
  if (!kq)
  {
    Write("That bai\n", MAX_LENGTH, 1);
  }
  else
  {
    Write("Thanh cong\n", MAX_LENGTH, 1);
  }
  Halt();
}