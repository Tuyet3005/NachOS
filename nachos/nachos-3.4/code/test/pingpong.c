#include "syscall.h"

typedef SpaceId;
int main() 
{
	SpaceId pingPID, pongPID;

	PrintString("Ping-Pong test starting ...\n");
	
	pingPID = Exec("./test/ping");
	pongPID = Exec("./test/pong");
	PrintInt(pingPID);
	PrintInt(pongPID);
	Join(pingPID);
	Join(pongPID);
}
