#include "syscall.h" 
#include "copyright.h" 
#define maxlen 32 

int main() {
	char filename[maxlen];
	PrintString("Nhap ten file: ");
	ReadString(filename, maxlen);
	CreateFile(filename);
	Halt();
}
