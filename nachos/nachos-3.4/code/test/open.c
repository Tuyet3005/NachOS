#include "syscall.h"

int
main()
{
	OpenFileId f;
	f = Open("abc.txt", 1); // File nay co trong thu muc code
//	f = Open("stdin", 1);
//	f = Open("stdout", 0);
//	f = Open("aaa.txt", 1; // File nay khong co trong thu muc code
	PrintString("openfile id(neu loi thi bang -1):");
	PrintInt(f);
	Close(f);
	Halt();
}