#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// กำหนด ID ให้กับ Control ต่างๆ เพื่อให้ง่ายต่อการอ้างอิง
#define ID_EDIT1 101
#define ID_EDIT2 102
#define ID_BTN_ADD 103
#define ID_BTN_SUB 104
#define ID_BTN_MUL 105
#define ID_BTN_DIV 106

// สร้างตัวแปร Global เพื่อเก็บค่า Window Handle ของกล่องข้อความ (Edit Box)
HWND hEdit1, hEdit2;

// ฟังก์ชันหลักสำหรับจัดการ Event (Message) ต่างๆ ของ Window
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            // สร้างช่องกรอกตัวเลขที่ 1 และ 2
            hEdit1 = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 20, 195, 25, hwnd, (HMENU)ID_EDIT1, NULL, NULL);
            hEdit2 = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 55, 195, 25, hwnd, (HMENU)ID_EDIT2, NULL, NULL);

            // สร้างปุ่มกด +, -, *, /
            CreateWindowA("BUTTON", "+", WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 95, 40, 40, hwnd, (HMENU)ID_BTN_ADD, NULL, NULL);
            CreateWindowA("BUTTON", "-", WS_CHILD | WS_VISIBLE | WS_BORDER, 70, 95, 40, 40, hwnd, (HMENU)ID_BTN_SUB, NULL, NULL);
            CreateWindowA("BUTTON", "*", WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 95, 40, 40, hwnd, (HMENU)ID_BTN_MUL, NULL, NULL);
            CreateWindowA("BUTTON", "/", WS_CHILD | WS_VISIBLE | WS_BORDER, 170, 95, 40, 40, hwnd, (HMENU)ID_BTN_DIV, NULL, NULL);
            break;

        case WM_COMMAND: {
            // เช็คว่า Action ที่เกิดขึ้น มาจากการกดปุ่มคำนวณหรือไม่
            if (LOWORD(wp) >= ID_BTN_ADD && LOWORD(wp) <= ID_BTN_DIV) {
                char text1[100], text2[100];
                
                // ดึงข้อความจาก Edit Box ทั้งสองช่อง
                GetWindowTextA(hEdit1, text1, 100);
                GetWindowTextA(hEdit2, text2, 100);

                // เปลี่ยนข้อความ (c-string) เป็นตัวเลขทศนิยม (double)
                double num1 = atof(text1);
                double num2 = atof(text2);
                double result = 0;
                char out[100]; // สำหรับเก็บข้อความผลลัพธ์

                // ตรวจสอบว่ากดปุ่มไหน
                int op = LOWORD(wp);
                if (op == ID_BTN_ADD) {
                    result = num1 + num2;
                } else if (op == ID_BTN_SUB) {
                    result = num1 - num2;
                } else if (op == ID_BTN_MUL) {
                    result = num1 * num2;
                } else if (op == ID_BTN_DIV) {
                    if (num2 == 0) { // ดัก Error กรณีหารด้วย 0
                        MessageBoxA(hwnd, "Cannot divide by zero!", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }
                    result = num1 / num2;
                }

                // แปลงผลลัพธ์ (double) กลับเป็นข้อความ (c-string) ตาม Hint
                sprintf(out, "%f", result);
                
                // แสดงผลลัพธ์ใน Message Box ชื่อ "Result"
                MessageBoxA(hwnd, out, "Result", MB_OK | MB_ICONINFORMATION);
            }
            break;
        }

        case WM_DESTROY:
            // สั่งปิดโปรแกรมเมื่อผู้ใช้กดกากบาท
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcA(hwnd, msg, wp, lp);
    }
    return 0;
}

// ฟังก์ชัน WinMain (จุดเริ่มต้นของโปรแกรม GUI)
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASSA wc = {0};
    
    // กำหนดสีพื้นหลังให้เป็น "สีสดๆ" (ในที่นี้ขอเลือกเป็น สีฟ้าสว่าง Cyan)
    wc.hbrBackground = CreateSolidBrush(RGB(0, 255, 255)); 
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = "MyCalcClass";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassA(&wc)) return -1;

    // ตั้งค่า Style ให้หน้าต่างไม่สามารถย่อ/ขยายได้ (เอา WS_THICKFRAME และ WS_MAXIMIZEBOX ออก)
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

    // สร้าง Window กว้าง 250, สูง 200 และกำหนดชื่อ "My Calculator"
    HWND hwnd = CreateWindowA("MyCalcClass", "My Calculator", style,
        CW_USEDEFAULT, CW_USEDEFAULT, 250, 200, NULL, NULL, hInst, NULL);

    ShowWindow(hwnd, ncmdshow);

    // Message Loop สำหรับให้ Window รอรับ Action จากผู้ใช้
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}