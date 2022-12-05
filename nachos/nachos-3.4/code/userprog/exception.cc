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
#define MaxFileLength 32
#define MaxStringLength 64

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
     if (len == 0) return len; 
     int i = 0; 
     int oneChar = 0 ; 
     do { 
        oneChar = (int) buffer[i]; 
        machine->WriteMem(virtAddr + i, 1, oneChar); 
        i++; 
     } while (i < len && oneChar != 0); 
     return i; 
} 


void inc_PC()
{
   	machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    	machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
   	machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
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
            return;

        case PageFaultException:
            DEBUG('a', "\n No valid translation found");
            printf("\n\n No valid translation found");
            interrupt->Halt();
            break;
        case ReadOnlyException:
            DEBUG('a', "\n Write attempted to page marked \"read-only\"");
            printf("\n\n Write attempted to page marked");
            interrupt->Halt();
            break;
        case BusErrorException:
            DEBUG('a', "\n Translation resulted invalid physical address");
            printf("\n\n Translation resulted invalid physical address");
            interrupt->Halt();
            break;
        case AddressErrorException:
            DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
            printf("\n\n Unaligned reference or one that was beyond the end of the address space");
            interrupt->Halt();
            break;
        case OverflowException:
            DEBUG('a', "\n Integer overflow in add or sub.");
            printf("\n\n Integer overflow in add or sub.");
            interrupt->Halt();
            break;
        case IllegalInstrException:
            DEBUG('a', "\n Unimplemented or reserved instr.");
            printf("\n\n Unimplemented or reserved instr.");
            interrupt->Halt();
            break;
        case NumExceptionTypes:
            DEBUG('a', "\n Number exception types");
            printf("\n\n Number exception types");
            interrupt->Halt();
            break;
        case SyscallException:
            {
                switch(type)
                {
                    
                    case SC_Halt:

                        DEBUG('a', "Shutdown, initiated by user program.\n");
                        printf ("\n\n Shutdown, initiated by user programz.\n");
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
                        break;
                    }
                    case SC_Read:
					{
			
						int virtAddr = machine->ReadRegister(4);
						int charcount = machine->ReadRegister(5);
						int openf_id = machine->ReadRegister(6);
						int i = fileSystem->index;
			
						if (openf_id > i || openf_id < 0 || openf_id == 1) // go wrong <-- if try open `out of domain` fileSystem (10 openfile) 
						{						 	// or try to read stdout
							printf("Try to open invalib file");
							machine->WriteRegister(2, -1);
							break;
						}

						if (fileSystem->openfile[openf_id] == NULL)
						{
							machine->WriteRegister(2, -1);
							break;
						}

						char *buf = User2System(virtAddr, charcount);
			
						if (openf_id == 0) // read from stdin
						{
							int sz = gSynchConsole->Read(buf, charcount);
							System2User(virtAddr, sz, buf);
							machine->WriteRegister(2, sz);

							delete[] buf;
							break;
						}
			
						int before = fileSystem->openfile[openf_id]->GetCurrentPos();
						if ((fileSystem->openfile[openf_id]->Read(buf, charcount)) > 0)
						{
							// copy data from kernel to user space
							int after = fileSystem->openfile[openf_id]->GetCurrentPos();
							System2User(virtAddr, charcount, buf);
							machine->WriteRegister(2, after - before + 1);	// after & before just used for returning
						} else {
							machine->WriteRegister(2, -1);
						}
						delete[] buf;
						break;
			
					}
                    case SC_Write:
					{
						int virtAddr = machine->ReadRegister(4);
						int charcount = machine->ReadRegister(5);
						int openf_id = machine->ReadRegister(6);
						int i = fileSystem->index;


						if (openf_id > i || openf_id < 0 || openf_id == 0) // `out of domain` filesys + try to write to stdin 
						{
							machine->WriteRegister(2, -1);
							break;
						}
			
						if (fileSystem->openfile[openf_id] == NULL)
						{
							machine->WriteRegister(2, -1);
							break;
						}

						// read-only file	
						if (fileSystem->openfile[openf_id]->type == 1)
						{
							printf("Try to modify read-only file");
							machine->WriteRegister(2, -1);
							break;
						}

						// write to console
						char *buf = User2System(virtAddr, charcount);
						if (openf_id == 1)
						{
							int i = 0;
							while (buf[i] != '\0' && buf[i] != '\n')
							{
								gSynchConsole->Write(buf + i, 1);
								i++;
							}
							buf[i] = '\n';
							gSynchConsole->Write(buf + i, 1); // write last character

							machine->WriteRegister(2, i - 1);
							delete[] buf;
							break;
						}


						// write into file
						int before = fileSystem->openfile[openf_id]->GetCurrentPos();
						if ((fileSystem->openfile[openf_id]->Write(buf, charcount)) != 0)
						{
							int after = fileSystem->openfile[openf_id]->GetCurrentPos();
							System2User(virtAddr, after - before, buf);
							machine->WriteRegister(2, after - before + 1);
							delete[] buf;
							break;
						}
					}
                    case SC_Fork:
                        break;
                    case SC_Yield:
                        break;
		    case SC_ReadInt:
		{
			char *input = new char[MaxStringLength];
                        gSynchConsole->Read(input, MaxStringLength);
			int len = strlen(input);
			bool check = true; 
			int start = 0; 

			if (len == 0) { printf("\nKhong phai so nguyen (Mac dinh la 0)\n"); } 
			if (input[0] == '-') { start = 1; } 
			if (start == len) { printf("\nKhong phai so nguyen (Mac dinh la 0)\n"); }
			for (; start < len; start++) { 
				if(input[start] < '0' || input[start] > '9') {
					if(input[start] != '\n') {
						machine->WriteRegister(2, 0);
						printf("\nKhong phai so nguyen (Mac dinh la 0)\n");
						check = false;
						break;
					}
				}
			}
			if (check) {
				int result = atoi(input);
				machine->WriteRegister(2, result);
			}
			delete[] input;
			
                        inc_PC();
			break;
		}
                    case SC_PrintInt:
		{
			int number = machine->ReadRegister(4); 
			int sign = number < 0 ? -1 : 1;
			bool check = false;
			if (number == -2147483648){ number += 1; check = true; }
			number *= sign;
			int temp = number;
			int numlen = number == 0 ? 1 : 0; 
			while(temp > 0) {
				numlen++;
				temp /= 10;
			}
			numlen += sign == -1 ? 1 : 0;
			
			char* toScreen = new char[numlen + 1];
			toScreen[numlen] = '\0';
			while (numlen != 0) {
				toScreen[numlen-- - 1] = (char)('0' + (number % 10));
				number /= 10;
			}
			toScreen[0] = sign == -1 ? '-' : toScreen[0];
			if (check) { toScreen[strlen(toScreen) - 1] = '8'; }
			gSynchConsole->Write(toScreen, strlen(toScreen));
			inc_PC();
			
			break;
		} 
			

                    case SC_PrintChar:
                    {
                        int val = machine->ReadRegister(4);
                        char input[] = {val};
                        int res = gSynchConsole->Write(input, 1);
                        machine->WriteRegister(2, res == -1);
                        inc_PC();
                        break;
                    }
                    case SC_PrintString:
                    {
                        int addr = machine->ReadRegister(4);
                        char *input = User2System(addr, MaxStringLength);
                        int res = gSynchConsole->Write(input, MaxStringLength);
                        machine->WriteRegister(2, res == -1);
                        inc_PC();
                        break;
                    }

                    case SC_ReadChar:
                    {
                        char *input = new char[MaxStringLength];
                        gSynchConsole->Read(input, MaxStringLength);
                        machine->WriteRegister(2, input[0]);
                        delete[] input;
                        inc_PC();
                        break;
                    }

                    case SC_ReadString:
                    {
                        int bufAddr = machine->ReadRegister(4);
                        int len = machine->ReadRegister(5);
                        char *input = new char[len];
                        int res = gSynchConsole->Read(input, len);
                        System2User(bufAddr, len, input);
                        machine->WriteRegister(2, res == -1);
                        delete[] input;
                        inc_PC();
                        break;
                    }
			case SC_CreateFile:
		{
			int virtAddr;
			char* filename;
			DEBUG('a', "\n SC_CreateFile call...");
			DEBUG('a', "\n Reading virtual address of file");

			virtAddr = machine->ReadRegister(4);
			filename = User2System(virtAddr, 255);
			
			if (strlen(filename) == 0) {
				printf("\n File name is not valid");
				DEBUG('a', "\n File name is not valid");
				machine->WriteRegister(2, -1);
				break;
			}

			if (filename == NULL) {
				printf("\n Not enough memory in system");		
				DEBUG('a', "\n Not enough memory in system");	
				delete filename;
				break;
			}
			
			DEBUG('a', "\n Finish reading filename");

			if (!fileSystem->Create(filename, 0)) {
				printf("\n Error create file '%s'", filename);
				machine->WriteRegister(2, -1);
				delete filename;
				break;
			}

			machine->WriteRegister(2, 0);
			delete filename;
			inc_PC();
			break;
                }
					case SC_Close:
					{
						int no = machine->ReadRegister(4);
						int i = fileSystem->index;

						// opened [i] files, and want to close file No.[no] (no > i) --> go wrong
						if (i < no)
						{
							printf("Close file failed \n");
							machine->WriteRegister(2, -1);
							inc_PC();
							break;
						}

						fileSystem->openfile[no] == NULL;
						delete fileSystem->openfile[no];
						machine->WriteRegister(2, 0);
						printf("Close file success\n");
						inc_PC();
						break;
					}
					case SC_Open:
					{
						int bufAddr = machine->ReadRegister(4); 
						int type = machine->ReadRegister(5);
						char *buf;

						// if already opened 10 files
						if (fileSystem->index > 10)
						{
							machine->WriteRegister(2, -1);
							delete[] buf;
							inc_PC();
							break;
						}
				
						// if open stdin or stdout, number of openfiles dont increase
						buf = User2System(bufAddr, MaxFileLength + 1);
						if (strcmp(buf, "stdin") == 0)
						{
							printf("Stdin mode\n");
							machine->WriteRegister(2, 0);
							delete[] buf;
							inc_PC();
							break;
						}
						if (strcmp(buf, "stdout") == 0)
						{
							printf("Stdout mode\n");
							machine->WriteRegister(2, 1);
							delete[] buf;
							inc_PC();
							break;
						}

						// if opening file succeed
						// should not use OpenFile* temp to store = fileSystem->openfile[fileSystem->index]
						// cause, i dont have a method to destroy this pointer correctly
						if ((fileSystem->openfile[fileSystem->index] = fileSystem->Open(buf, type)) != NULL)
						{

							printf("\nOpen file success '%s'\n", buf);
							machine->WriteRegister(2, fileSystem->index - 1);
						}
						else 
						{
							printf("Can not open file '%s'\n", buf);
							machine->WriteRegister(2, -1);
						}
						delete[] buf;
						inc_PC();
						break;

					}

                }
            }
    }

}
