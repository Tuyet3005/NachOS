#include "syscall.h"
#define maxlen 255
int
main()
{
	int i, n, arr[100], j, tmp;
	char buffer[maxlen];
    PrintString("Nhap n: ");
//Nhap n (buffer la chuoi nhap vao, maxlen la chieu dai toi da cua chuoi
	n = ReadInt(buffer,maxlen);
//Nhap cac phan tu cua mang
	for (i = 0 ; i < n ; i ++){
		PrintString("Nhap so nguyen thu ");
		PrintInt(i+1);
		PrintString(": ");
		arr[i] = ReadInt(buffer, maxlen);
	}
//Thuat toan bubble sort sap xep mang
	for (i = 0; i < n - 1; i++){
		for (j = 0; j < n - 1 - i; j++){
			if (arr[j] > arr[j+1]){
				tmp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = tmp;
			}
		}
	}
// In ra cac phan tu cua mang
	for (i = 0; i < n ; i++){
		PrintInt(arr[i]);
		PrintChar(32);
	}
	Halt();
}
