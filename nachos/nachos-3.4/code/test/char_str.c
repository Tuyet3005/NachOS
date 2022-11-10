#include "syscall.h" 
#include "copyright.h" 
#define maxlen 32 
int main() 
{ 
    char i, buffer[maxlen];
    PrintString("Nhap ki tu: ");
    i= ReadChar();
    PrintChar(i);
    PrintChar('\n');

    PrintString("Nhap chuoi: ");
    ReadString(buffer, maxlen);
    PrintString(buffer);
    PrintString("\nDone!!!");
    Halt();
}

    
