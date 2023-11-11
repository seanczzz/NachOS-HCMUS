#ifndef FILETABLE_H
#define FILETABLE_H
#include "openfile.h"
#include "sysdep.h"
#include "filetable.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define FILE_MAX 20
#define CONSOLE_IN 0
#define CONSOLE_OUT 1
#define MODE_READWRITE 0
#define MODE_READ 1
#define MODE_WRITE 2

struct FileItem
{
  OpenFile *openFile;
  char *nameOpenFile;
  int socketId;
  int fileOpenMode;
};

class FileTable
{
private:
  FileItem fileItem[FILE_MAX];

public:
  FileTable()
  {
    fileItem[CONSOLE_IN].fileOpenMode = MODE_READ;
    fileItem[CONSOLE_OUT].fileOpenMode = MODE_WRITE;

    for (int i = 2; i < FILE_MAX; i++)
    {
      fileItem[i].socketId = 0;
      fileItem[i].openFile = NULL;
    }
  }

  int getSocketIdAt(int fileId)
  {
    return fileItem[fileId].socketId;
  }

  int Insert(char *fileName, int openMode)
  {
    int freeIndex = -1;
    int fileDescriptor = -1;
    for (int i = 2; i < FILE_MAX; i++)
    {
      if (fileItem[i].openFile == NULL)
      {
        freeIndex = i;
        break;
      }
    }

    if (freeIndex == -1)
    {
      return -1;
    }

    if (openMode == MODE_READWRITE)
      fileDescriptor = OpenForReadWrite(fileName, FALSE);
    if (openMode == MODE_READ)
      fileDescriptor = OpenForRead(fileName, FALSE);

    if (fileDescriptor == -1)
      return -1;

    fileItem[freeIndex].openFile = new OpenFile(fileDescriptor);
    fileItem[freeIndex].fileOpenMode = openMode;
    fileItem[freeIndex].nameOpenFile = strdup(fileName);

    return freeIndex;
  }

  int Remove(int index)
  {
    if (index < 2 || index >= FILE_MAX)
      return -1;
    if (fileItem[index].openFile)
    {
      delete fileItem[index].openFile;
      delete fileItem[index].nameOpenFile;

      fileItem[index].openFile = NULL;
      fileItem[index].nameOpenFile = NULL;
      return 0;
    }
    return -1;
  }

  int Remove(char *name)
  {
    for (int i = 2; i < FILE_MAX; i++)
    {
      if (fileItem[i].openFile && fileItem[i].nameOpenFile && strcmp(name, fileItem[i].nameOpenFile) == 0)
      {
        delete fileItem[i].nameOpenFile;
        delete fileItem[i].openFile;
        fileItem[i].nameOpenFile = NULL;
        fileItem[i].openFile = NULL;
        return 0;
      }
    }
    return -1;
  }

  int Read(char *buffer, int charCount, int index)
  {
    if (index >= FILE_MAX)
      return -1;
    if (fileItem[index].openFile == NULL)
      return -1;
    int result = fileItem[index].openFile->Read(buffer, charCount);
    // if we cannot read enough bytes, we should return -2
    // if (result != charCount)
    //   return -2;
    return result;
  }

  int Write(char *buffer, int charCount, int index)
  {
    if (index >= FILE_MAX)
      return -1;
    if (fileItem[index].openFile == NULL || fileItem[index].fileOpenMode == MODE_READ)
      return -1;
    return fileItem[index].openFile->Write(buffer, charCount);
  }

  int Seek(int pos, int index)
  {
    if (index <= 1 || index >= FILE_MAX)
      return -1;
    if (fileItem[index].openFile == NULL)
      return -1;
    // use seek(-1) to move to the end of file
    if (pos == -1)
      pos = fileItem[index].openFile->Length();
    if (pos < 0 || pos > fileItem[index].openFile->Length())
      return -1;
    return fileItem[index].openFile->Seek(pos);
  }

  int CreateSocket()
  {
    // int sockId = OpenSocket();
    int sockId = socket(AF_INET, SOCK_STREAM, 0);
    if (sockId < 0)
      return -1;

    int freeIndex = -1;

    for (int i = 2; i < FILE_MAX; i++)
    {
      if (fileItem[i].openFile == NULL && fileItem[i].socketId == 0)
      {
        freeIndex = i;
        break;
      }
    }

    if (freeIndex == -1)
    {
      return -1;
    }

    fileItem[freeIndex].socketId = sockId;
    return freeIndex;
  }

  ~FileTable()
  {
    for (int i = 0; i < FILE_MAX; i++)
    {
      if (fileItem[i].openFile)
        delete fileItem[i].openFile;

      if (fileItem[i].nameOpenFile)
        delete fileItem[i].nameOpenFile;
    }
  }
};

#endif