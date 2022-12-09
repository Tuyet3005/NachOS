#include "syscall.h"

int
main()
{
    OpenFileId sv, vn;
    SpaceId voinuoc_sid;
    char c[1];
    int check;
	// Mo file sinhvien.txt
    sv = Open("sinhvien.txt", 1);
    if (sv == -1) Exit(-1);
    voinuoc_sid = Exec("./test/voinuoc");
    if (voinuoc_sid == -1) {
        Close(sv);
        Exit(-1);
    }
    check = -1;
	// Tao va mo file voinuoc.txt
    while (check != 0) {
        check = CreateFile("voinuoc.txt");
        if (check == -1) {
            Close(sv);
            Exit(-1);
        }
        vn = Open("voinuoc.txt", 0);
        if (vn == -1) {
            Close(sv);
            Exit(-1);
        }
	//Doc tu file sinhvien.txt va ghi ra file voinuoc.txt 
        while (1) {
            if (Read(c, 1, sv) == -1) {
                Write("0", 1, vn);
                check = 0;
                break;
            } else check = -1;
            if (c[0] == ' ' || c[0] == '\n') break;
            Write(c, 1, vn);
        }
        Close(vn);
        Signal("voinuoc");
        Wait("sinhvien");
    }

    Exit(0);
}

