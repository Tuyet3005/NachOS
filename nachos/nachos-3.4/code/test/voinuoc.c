#include "syscall.h"

int
main()
{
    int n, v1, v2;
    OpenFileId vn, res;
    char c;
	// Tao va mo file result.txt
    CreateFile("result.txt");
    res = Open("result.txt", 0);
    n = -1;
    v1 = v2 = 0;
	// Doc tu file voinuoc.txt
    while (1) {
        if (n == 0) break;
        Wait("voinuoc");
        vn = Open("voinuoc.txt", 1);
        n = 0;
        while (1) {
            if (Read(&c, 1, vn) == -1) break;
	
            if (c >= '0' && c <= '9') n = 10*n + c - '0';
            else break;
        }
        Close(vn);
	//Xuat ra file result.txt
        if (n != 0) {
            if (v1 <= v2) {
                v1 += n;
                if (res != -1) 
                    Write("1 ", 2, res);
            } else {
                v2 += n;
                if (res != -1)
                    Write("2 ", 2, res);
            }
        }
        Signal("sinhvien");
    }
	//Xuat ra file result.txt
    if (res != -1) {
        Write("\n", 1, res);
        Close(res);
    }
    Exit(0);
}

