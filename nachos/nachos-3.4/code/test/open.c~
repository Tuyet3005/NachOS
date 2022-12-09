#include "syscall.h"

int
main()
{
	OpenFileId f;
	char c;
	f = Open("voinuoc.txt",1);
	PrintString("File id: ");
	PrintInt(f);
	PrintString("\nhahaha\n");
	PrintInt(Read(&c, 1, f));
	PrintString("\n");
	PrintChar(c);
	Close(f);
	Halt();
}
