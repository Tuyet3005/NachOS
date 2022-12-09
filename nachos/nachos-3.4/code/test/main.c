#include "syscall.h"

int
main()
{
    OpenFileId inp, sv, out, res;
    char c;
    int n, check;
    SpaceId sv_sid;
	// Tao file output
    check = CreateFile("output.txt");
	// Tao semaphore
    if (check == -1) Exit(-1);
    check = CreateSemaphore("sinhvien", 0);
    if (check == -1) Exit(-1);
    check = CreateSemaphore("voinuoc", 0);
    if (check == -1) Exit(-1);
	// Mo file inp, out
    inp = Open("input.txt", 1);
    if (inp == -1) Exit(-1);
    out = Open("output.txt", 0);
    if (out == -1) {
        Close(inp);
        Exit(-1);
    } 
    n = 0;
	// Doc so thoi diem
    while (1) {
        if (Read(&c, 1, inp) == -1) break;
        if (c >= '0' && c <= '9') n = 10*n + c - '0';
        else if (c == '\n') break;
    }
	// Lap voi moi thoi diem
    while (n--) {
		PrintInt(n);
        check = CreateFile("sinhvien.txt");
        if (check == -1) {
            Close(inp);
            Exit(-1);
        }
        sv = Open("sinhvien.txt", 0);
        if (sv == -1) {
            Close(inp);
            Exit(-1);
        }
		//Doc tu file input.txt roi xuat ra file sinhvien.txt
        while (1) {
            if (Read(c, 1, inp) == -1) break;
            Write(c, 1, sv);
            if (c == '\n') break;
        }

        Close(sv);

        sv_sid = Exec("./test/sinhvien");
        if (sv_sid != -1) check = Join(sv_sid);
        if (check == -1) {
            Close(inp);
            Close(out);
            Exit(-1);
        }

        res = Open("result.txt", 0);
        if (res == -1) {
            Close(inp);
            Close(out);
            Exit(-1);
        }
		//Doc tu file result.txt de xuat ra file output.txt
        while (Read(c, 1, res) != -1)
            Write(c, 1, out);
        Close(res);

    }
    Exit(0);
}

