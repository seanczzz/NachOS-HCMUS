/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"
#include "synchconsole.h"
#include <stdlib.h>

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

char SysReadChar() { return kernel->synchConsoleIn->GetChar(); }

int SysReadString(char *buffer, int length)
{
  for (int i = 0; i < length; i++)
  {
    buffer[i] = SysReadChar();
    if (buffer[i] == '\n')
    {
      buffer[i] = '\0';
      return i;
    }
  }
  buffer[length] = '\0';

  return length;
}

int SysPrintString(char *buffer, int length)
{
  for (int i = 0; i < length; i++)
  {
    if (buffer[i] == '\0')
    {
      return i;
    }
    kernel->synchConsoleOut->PutChar(buffer[i]);
  }
  return length;
}

#define MAX_NUM_LENGTH 11
#define INT32_MIN (-2147483647 - 1)

void SysPrintNum(int num)
{
  char _numberBuffer[MAX_NUM_LENGTH + 2];
  if (num == 0)
    return kernel->synchConsoleOut->PutChar('0');

  if (num == INT32_MIN)
  {
    kernel->synchConsoleOut->PutChar('-');
    for (int i = 0; i < 10; ++i)
      kernel->synchConsoleOut->PutChar("2147483648"[i]);
    return;
  }

  if (num < 0)
  {
    kernel->synchConsoleOut->PutChar('-');
    num = -num;
  }
  int n = 0;
  while (num)
  {
    _numberBuffer[n++] = num % 10;
    num /= 10;
  }
  for (int i = n - 1; i >= 0; --i)
    kernel->synchConsoleOut->PutChar(_numberBuffer[i] + '0');
}

// Input: - User space address (int)
// - Limit of buffer (int)
// Output:- Buffer (char*)
// Purpose: Copy buffer from User memory space to System memory space
char *User2System(int virtAddr, int limit)
{
  int i; // index
  int oneChar;
  char *kernelBuf = NULL;
  kernelBuf = new char[limit + 1]; // need for terminal string
  if (kernelBuf == NULL)
    return kernelBuf;
  memset(kernelBuf, 0, limit + 1);
  // printf("\n Filename u2s:");
  for (i = 0; i < limit; i++)
  {
    kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
    kernelBuf[i] = (char)oneChar;
    // printf("%c",kernelBuf[i]);
    if (oneChar == 0)
      break;
  }
  return kernelBuf;
}

// Input: - User space address (int)
// - Limit of buffer (int)
// - Buffer (char[])
// Output:- Number of bytes copied (int)
// Purpose: Copy buffer from System memory space to User memory space
int System2User(int virtAddr, int len, char *buffer)
{
  if (len < 0)
    return -1;
  if (len == 0)
    return len;
  int i = 0;
  int oneChar = 0;
  do
  {
    oneChar = (int)buffer[i];
    kernel->machine->WriteMem(virtAddr + i, 1, buffer[i]);
    i++;
  } while (i < len && oneChar != 0);
  return i;
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
