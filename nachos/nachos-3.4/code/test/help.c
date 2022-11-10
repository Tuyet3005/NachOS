#include "syscall.h"

int
main()
{
	PrintString("\n===================================================================");
    	PrintString("\nThong tin cua nhom:\n");
	PrintString("1. Le Xuan Hoang       \t 20120089\n");
	PrintString("2. Nguyen Thi Anh Tuyet\t 20120422\n");
	PrintString("3. Nguyen Le Hai Duong \t 20120460\n");
	PrintString("4. Le Xuan Huy         \t 20120494\n");
	PrintString("\n===================================================================");
	PrintString("\nMo ta chuong trinh ascii: (Chay file ascii)\n");
	PrintString("\tChuong trinh nay in ra bang ma ascii\n\n");
	PrintString("Mo ta chuong trinh sort: (Chay file sort)\n");
	PrintString("\tNguoi dung se nhap vao 1 so n (n<=100).\n");
	PrintString("\tTiep theo, nguoi dung se nhap n so nguyen. \n");
	PrintString("\tChuong trinh su dung bubble sort de sap xep mang.\n");
	PrintString("\tChuong trinh in mang theo tu tu tang dan ra man hinh.\n");
	Halt();
}
