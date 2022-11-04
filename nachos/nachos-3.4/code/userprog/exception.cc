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
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

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
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

char* User2System(int virtAddr,int limit) 
{ 
 int i;// index 
 int oneChar; 
 char* kernelBuf = NULL; 
 
 kernelBuf = new char[limit +1];//need for terminal string 
 if (kernelBuf == NULL) 
 return kernelBuf; 
 memset(kernelBuf,0,limit+1); 
 
 //printf("\n Filename u2s:"); 
 for (i = 0 ; i < limit ;i++) 
 { 
 machine->ReadMem(virtAddr+i,1,&oneChar); 
 kernelBuf[i] = (char)oneChar; 
 //printf("%c",kernelBuf[i]); 
 if (oneChar == 0) 
 break; 
 } 
 return kernelBuf; 
}


int System2User(int virtAddr,int len,char* buffer) 
{
if (len < 0) return -1; 
 if (len == 0)return len; 
 int i = 0; 
 int oneChar = 0 ; 
 do{ 
 oneChar= (int) buffer[i]; 
 machine->WriteMem(virtAddr+i,1,oneChar); 
 i ++; 
 }while(i < len && oneChar != 0); 
 return i; 
}


void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    switch (which)
    {
        default:
	    printf("Unexpected user mode exception %d %d\n", which, type);
	    ASSERT(FALSE);
            break;
        case NoException:
            break;
        case PageFaultException:
            break;
        case ReadOnlyException:
            break;
        case BusErrorException:
            break;
        case AddressErrorException:
            break;
        case OverflowException:
            break;
        case IllegalInstrException:
            break;
        case NumExceptionTypes:
            break;
        case SyscallException:
            {
                switch(type)
                {
                    case SC_Halt:

                        DEBUG('a', "Shutdown, initiasdsdted by user program.\n");
                        printf ("\n\n Shutdown, initiadfdfted by user program.\n");
   	                interrupt->Halt();
                        break;
                    case SC_Exit:
                        break;
                    case SC_Exec:
                        break;
                    case SC_Join:
                        break;
                    case SC_Create:
                    {
                        printf("asdasd\n");
                        break;
                    }
                    case SC_Read:
                        break;
                    case SC_Write:
                        break;
                    case SC_Close:
                        break;
                    case SC_Fork:
                        break;
                    case SC_Yield:
                        break;
                    case SC_Sub: 
                        op1 = machine->ReadRegister (4); 
                        op2 = machine->ReadRegister (5); 
                        result = op1 - op2; 
                        machine->WriteRegister (2, result); 
                        interrupt->Halt();
                        break;    
                    default:
                        printf("\n Unexpected user mode exception (%d %d)", which, type); 
                        interrupt->Halt(); 
                }
            }
    }

}
