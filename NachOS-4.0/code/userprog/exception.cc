// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define CONSOLE_IN 0
#define CONSOLE_OUT 1
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

#define MAX_PRINT_LENGTH 200
void HandleSysCall_PrintString()
{
	int memPtr = kernel->machine->ReadRegister(4); // read address of C-string
	char *buffer = User2System(memPtr, MAX_PRINT_LENGTH);
	SysPrintString(buffer, strlen(buffer));
	delete[] buffer;
}

void HandleSysCall_PrintNum()
{
	int num = kernel->machine->ReadRegister(4);
	SysPrintNum(num);
}

void updateProgramCounter()
{
	/* set previous programm counter (debugging only)*/
	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

	/* set next programm counter for brach execution */
	kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}

void HandleSysCall_Create()
{
	int virtAddr;
	char *filename;
	DEBUG(dbgAddr, "\n SC_Create call ...");
	DEBUG(dbgAddr, "\n Reading virtual address of filename");
	// Lấy tham số tên tập tin từ thanh ghi r4
	virtAddr = kernel->machine->ReadRegister(4);
	DEBUG(dbgAddr, "\n Reading filename.");
	// MaxFileLength là = 32
	filename = User2System(virtAddr, MaxFileLength + 1);
	if (filename == NULL)
	{
		printf("\n Not enough memory in system");
		DEBUG(dbgAddr, "\n Not enough memory in system");
		kernel->machine->WriteRegister(2, -1); // trả về lỗi cho chương
		// trình người dùng
		delete filename;
		return;
	}
	DEBUG(dbgAddr, "\n Finish reading filename.");
	// DEBUG(dbgAddr,"\n File name : '"<<filename<<"'");
	//  Create file with size = 0
	//  Dùng đối tượng fileSystem của lớp OpenFile để tạo file,
	//  việc tạo file này là sử dụng các thủ tục tạo file của hệ điều
	//  hành Linux, chúng ta không quản ly trực tiếp các block trên
	//  đĩa cứng cấp phát cho file, việc quản ly các block của file
	//  trên ổ đĩa là một đồ án khác
	if (!kernel->fileSystem->Create(filename, 0))
	{
		printf("\n Error create file '%s'", filename);
		kernel->machine->WriteRegister(2, -1);
		delete filename;
		return;
	}
	kernel->machine->WriteRegister(2, 0); // trả về cho chương trình
	// người dùng thành công
	delete filename;
}

void HandleSysCall_Open()
{
	DEBUG(dbgSys, "Opening files.....\n");

	int virtAddr = kernel->machine->ReadRegister(4);
	DEBUG(dbgAddr, "\n Reading filename.");
	// MaxFileLength là = 32
	char *filename = User2System(virtAddr, MaxFileLength + 1);

	int openMode = kernel->machine->ReadRegister(5);

	int openResult = kernel->fileSystem->Open(filename, openMode);
	if (openResult == -1)
	{
		DEBUG(dbgSys, "Fail to opening files huhuh \n");
	}
	else
	{
		DEBUG(dbgSys, "Succeed in opening file with id= ");
		DEBUG(dbgSys, openResult);
		DEBUG(dbgSys, "\n");
	}
	kernel->machine->WriteRegister(2, openResult);
	delete filename;
}

void HandleSysCall_Close()
{
	DEBUG(dbgSys, "Closing files.....\n");

	int closeFileId = kernel->machine->ReadRegister(4);
	int closeResult = kernel->fileSystem->Close(closeFileId);

	if (closeResult == -1)
	{
		DEBUG(dbgSys, "Fail to close files huhuh \n");
	}
	else // closeResult == 0
	{
		DEBUG(dbgSys, "Succeed in closing file\n");
	}

	kernel->machine->WriteRegister(2, closeResult);
}

void HandleSysCall_Read()
{
	int virtAddr;
	char *buffer;
	DEBUG(dbgAddr, "\n SC_Read call ...");
	DEBUG(dbgAddr, "\n Reading virtual address of buffer");
	virtAddr = kernel->machine->ReadRegister(4);
	int size = kernel->machine->ReadRegister(5);
	buffer = User2System(virtAddr, size + 1);
	if (buffer == NULL)
	{
		printf("\n Not enough memory in system");
		DEBUG(dbgAddr, "\n Not enough memory in system");
		kernel->machine->WriteRegister(2, -1); // trả về lỗi cho chương
		// trình người dùng
		delete buffer;
		return;
	}
	int id = kernel->machine->ReadRegister(6);

	if (id == CONSOLE_IN)
	{
		int sl = SysReadString(buffer, size);
		System2User(virtAddr, size + 1, buffer);
		kernel->machine->WriteRegister(2, sl);
		return;
	}

	int readResult = kernel->fileSystem->Read(buffer, size, id);

	if (readResult == -1)
	{
		DEBUG(dbgSys, "Fail to read files huhuh \n");
	}
	else // readResult == 0
	{
		DEBUG(dbgSys, "Succeed in reading file\n");
	}
	System2User(virtAddr, strlen(buffer), buffer);
	kernel->machine->WriteRegister(2, readResult);
	delete buffer;
}

void HandleSysCall_Write()
{
	int virtAddr;
	char *buffer;
	DEBUG(dbgAddr, "\n SC_Write call ...");
	DEBUG(dbgAddr, "\n Writing virtual address of buffer");
	virtAddr = kernel->machine->ReadRegister(4);
	int size = kernel->machine->ReadRegister(5);
	buffer = User2System(virtAddr, size + 1);
	if (buffer == NULL)
	{
		printf("\n Not enough memory in system");
		DEBUG(dbgAddr, "\n Not enough memory in system");
		kernel->machine->WriteRegister(2, -1); // trả về lỗi cho chương
		// trình người dùng
		delete buffer;
		return;
	}
	int id = kernel->machine->ReadRegister(6);

	if (id == CONSOLE_OUT)
	{
		int sl = SysPrintString(buffer, size);
		kernel->machine->WriteRegister(2, sl);
		return;
	}

	int writeResult = kernel->fileSystem->Write(buffer, size, id);

	if (writeResult == -1)
	{
		DEBUG(dbgSys, "Fail to write files huhuh \n");
	}
	else // writeResult == 0
	{
		DEBUG(dbgSys, "Succeed in writing file\n");
	}
	kernel->machine->WriteRegister(2, writeResult);
	delete buffer;
}

void HandleSysCall_Seek()
{
	int pos = kernel->machine->ReadRegister(4);
	int fileId = kernel->machine->ReadRegister(5);

	int seekResult = kernel->fileSystem->Seek(pos, fileId);
	kernel->machine->WriteRegister(2, seekResult);
}

void HandleSysCall_Remove()
{
	int virtAddr = kernel->machine->ReadRegister(4);
	char *fileName = User2System(virtAddr, MaxFileLength + 1);
	if (fileName == NULL)
	{
		printf("\n Not enough memory in system");
		DEBUG(dbgAddr, "\n Not enough memory in system");
		kernel->machine->WriteRegister(2, -1); // trả về lỗi cho chương
		// trình người dùng
		delete fileName;
		return;
	}

	int removeResult = kernel->fileSystem->Remove(fileName);
	kernel->machine->WriteRegister(2, removeResult);
}

void HandleSysCall_SocketTCP()
{
	int result = kernel->fileSystem->SocketTCP();
	kernel->machine->WriteRegister(2, result);
}

int CanConnect(int socketid, char *ip, int port)
{
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_port = htons(port);
	int len = sizeof(server);

	return connect(socketid, (struct sockaddr *)&server, len);
}

void HandleSysCall_Connect()
{
	int fileId = kernel->machine->ReadRegister(4);
	if (fileId < 2 || fileId >= FILE_MAX)
	{
		kernel->machine->WriteRegister(2, -1);
		return;
	}

	int socketid = kernel->fileSystem->fileTable[kernel->currentThread->processID]->getSocketIdAt(fileId);
	if (socketid == 0)
	{
		kernel->machine->WriteRegister(2, -1);
		return;
	}

	int virtAddr = kernel->machine->ReadRegister(5);

	// Dia chi IP (vd : 127.127.127.127) maxlen = 3*4 + 3 = 15, them 1 null character => 16
	char *ip = User2System(virtAddr, 16);
	if (ip == NULL)
	{
		printf("\n Not enough memory in system");
		DEBUG(dbgAddr, "\n Not enough memory in system");
		kernel->machine->WriteRegister(2, -1); // trả về lỗi cho chương
		// trình người dùng
		delete ip;
		return;
	}

	int port = kernel->machine->ReadRegister(6);

	int result = CanConnect(socketid, ip, port);

	if (result < 0)
	{
		kernel->machine->WriteRegister(2, -1);
	}
	else
	{
		kernel->machine->WriteRegister(2, 0);
	}
	delete ip;
}

void HandleSysCall_Send()
{
	int fileId = kernel->machine->ReadRegister(4);
	if (fileId < 2 || fileId >= FILE_MAX)
	{
		kernel->machine->WriteRegister(2, -1);
		return;
	}

	int socketid = kernel->fileSystem->fileTable[kernel->currentThread->processID]->getSocketIdAt(fileId);
	if (socketid == 0)
	{
		kernel->machine->WriteRegister(2, -1);
		return;
	}

	int len = kernel->machine->ReadRegister(6);

	int virtAddr = kernel->machine->ReadRegister(5);
	char *buffer = User2System(virtAddr, len);
	if (buffer == NULL)
	{
		printf("\n Not enough memory in system");
		DEBUG(dbgAddr, "\n Not enough memory in system");
		kernel->machine->WriteRegister(2, -1); // trả về lỗi cho chương
		// trình người dùng
		delete buffer;
		return;
	}

	int writeResult = write(socketid, buffer, strlen(buffer));

	if (writeResult < 0)
	{
		if (errno == ECONNRESET)
		{
			kernel->machine->WriteRegister(2, 0);
		}
		else
		{
			kernel->machine->WriteRegister(2, -1);
		}
	}
	else
	{
		kernel->machine->WriteRegister(2, writeResult);
	}
	delete buffer;
}

void HandleSysCall_Receive()
{
	int fileId = kernel->machine->ReadRegister(4);
	if (fileId < 2 || fileId >= FILE_MAX)
	{
		kernel->machine->WriteRegister(2, -1);
		return;
	}

	int socketid = kernel->fileSystem->fileTable[kernel->currentThread->processID]->getSocketIdAt(fileId);
	if (socketid == 0)
	{
		kernel->machine->WriteRegister(2, -1);
		return;
	}

	int len = kernel->machine->ReadRegister(6);

	int virtAddr = kernel->machine->ReadRegister(5);
	char *buffer = User2System(virtAddr, len);
	if (buffer == NULL)
	{
		printf("\n Not enough memory in system");
		DEBUG(dbgAddr, "\n Not enough memory in system");
		kernel->machine->WriteRegister(2, -1); // trả về lỗi cho chương
		// trình người dùng
		delete buffer;
		return;
	}

	int readResult = read(socketid, buffer, len);

	if (readResult < 0)
	{
		if (errno == ECONNRESET)
		{
			kernel->machine->WriteRegister(2, 0);
		}
		else
		{
			kernel->machine->WriteRegister(2, -1);
		}
	}
	else
	{
		System2User(virtAddr, len, buffer);
		kernel->machine->WriteRegister(2, readResult);
		// kernel->machine->WriteRegister(2, readResult);
	}
	// delete buffer;
}

void HandleSysCall_Exec()
{
	int virtAddr = kernel->machine->ReadRegister(4);			 // doc dia chi ten chuong trinh tu thanh ghi r4
	char *name = User2System(virtAddr, MaxFileLength + 1); // Lay ten chuong trinh, nap vao kernel
	if (name == NULL)
	{
		DEBUG(dbgSys, "\n Not enough memory in System");
		ASSERT(false);
		kernel->machine->WriteRegister(2, -1);
		return;
	}
	kernel->machine->WriteRegister(2, SysExec(name));
	// DO NOT DELETE NAME, THE THEARD WILL DELETE IT LATER
	// delete[] name;

	return;
}

void HandleSysCall_Join()
{
	int id = kernel->machine->ReadRegister(4);
	kernel->machine->WriteRegister(2, SysJoin(id));
}

void HandleSysCall_Exit()
{
	int id = kernel->machine->ReadRegister(4);
	kernel->machine->WriteRegister(2, SysExit(id));
}

void HandleSysCall_CreateSemaphore()
{
	int virtAddr = kernel->machine->ReadRegister(4);
	int semval = kernel->machine->ReadRegister(5);

	char *name = User2System(virtAddr, MaxFileLength + 1);
	if (name == NULL)
	{
		DEBUG(dbgSys, "\n Not enough memory in System");
		ASSERT(false);
		kernel->machine->WriteRegister(2, -1);
		delete[] name;
		return;
	}

	kernel->machine->WriteRegister(2, SysCreateSemaphore(name, semval));
	delete[] name;
}

void HandleSysCall_Wait()
{
	int virtAddr = kernel->machine->ReadRegister(4);

	char *name = User2System(virtAddr, MaxFileLength + 1);
	if (name == NULL)
	{
		DEBUG(dbgSys, "\n Not enough memory in System");
		ASSERT(false);
		kernel->machine->WriteRegister(2, -1);
		delete[] name;
		return;
	}

	kernel->machine->WriteRegister(2, SysWait(name));
	delete[] name;
	return;
}

void HandleSysCall_Signal()
{
	int virtAddr = kernel->machine->ReadRegister(4);

	char *name = User2System(virtAddr, MaxFileLength + 1);
	if (name == NULL)
	{
		DEBUG(dbgSys, "\n Not enough memory in System");
		ASSERT(false);
		kernel->machine->WriteRegister(2, -1);
		delete[] name;
		return;
	}

	kernel->machine->WriteRegister(2, SysSignal(name));
	delete[] name;
	return;
}

void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);

	// DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	case NoException: // return control to kernel
		kernel->interrupt->setStatus(SystemMode);
		DEBUG(dbgSys, "Switch to system mode\n");
		break;
	case PageFaultException:
	case ReadOnlyException:
	case BusErrorException:
	case AddressErrorException:
	case OverflowException:
	case IllegalInstrException:
	case NumExceptionTypes:
		cerr << "Error " << which << " occurs\n";
		SysHalt();
		ASSERTNOTREACHED();

	case SyscallException:
		switch (type)
		{
		case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();
			// ASSERTNOTREACHED();
			return;

		case SC_PrintNum:
			HandleSysCall_PrintNum();
			updateProgramCounter();
			return;

		case SC_PrintString:
			HandleSysCall_PrintString();
			updateProgramCounter();
			return;

		case SC_Create:
			HandleSysCall_Create();
			updateProgramCounter();
			return;

		case SC_Open:
			HandleSysCall_Open();
			updateProgramCounter();
			return;

		case SC_Close:
			HandleSysCall_Close();
			updateProgramCounter();
			return;

		case SC_Write:
			HandleSysCall_Write();
			updateProgramCounter();
			return;

		case SC_Read:
			HandleSysCall_Read();
			updateProgramCounter();
			return;

		case SC_Seek:
			HandleSysCall_Seek();
			updateProgramCounter();
			return;

		case SC_Remove:
			HandleSysCall_Remove();
			updateProgramCounter();
			return;

		case SC_SocketTCP:
			HandleSysCall_SocketTCP();
			updateProgramCounter();
			return;

		case SC_Connect:
			HandleSysCall_Connect();
			updateProgramCounter();
			return;

		case SC_Send:
			HandleSysCall_Send();
			updateProgramCounter();
			return;

		case SC_Receive:
			HandleSysCall_Receive();
			updateProgramCounter();
			return;

		case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
											/* int op2 */ (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			/* Modify return point */
			updateProgramCounter();

			return;

		case SC_Exec:
			HandleSysCall_Exec();
			updateProgramCounter();
			return;

		case SC_Join:
			HandleSysCall_Join();
			updateProgramCounter();
			return;
		case SC_Exit:
			HandleSysCall_Exit();
			updateProgramCounter();
			return;
		case SC_CreateSemaphore:
			HandleSysCall_CreateSemaphore();
			updateProgramCounter();
			return;

		case SC_Wait:
			HandleSysCall_Wait();
			updateProgramCounter();
			return;
		case SC_Signal:
			HandleSysCall_Signal();
			updateProgramCounter();
			return;

			// ASSERTNOTREACHED();

		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}
	ASSERTNOTREACHED();
}
