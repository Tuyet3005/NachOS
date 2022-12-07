#include "syscall.h" 
#include "copyright.h" 

int main() 
{ 
    PrintInt(CreateSemaphore("my sem", 5));
    PrintInt(Wait("my sem"));
    PrintInt(Wait("my sem"));
    PrintInt(Wait("my sem"));
    PrintInt(Wait("my sem"));
    PrintInt(Wait("my sem"));
    PrintInt(Signal("my sem"));
    PrintInt(Wait("my sem"));
    Halt();
}
