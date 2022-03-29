/* -------------------------------------------------------------------
                    MyWindows.c -- 基本窗口模型
                                《Windows 程序设计（SDK）》视频教程
--------------------------------------------------------------------*/

#include <windows.h>
#include <windowsx.h>
#include <iostream>

#define side 150



int Winner(int a[3][3])
{
	int row = 0, col = 0,
		lcross = 0, rcross = 0;
	for(int i = 0; i < 3; i++)
	{
		row = a[0][i] + a[1][i] + a[2][i];
		col = a[i][0] + a[i][1] + a[i][2];
		lcross += a[i][i];
		rcross += a[i][2-i];
		if(row == 3 || col == 3||lcross == 3 || rcross == 3) return 1;
		else if(row == -3 || col == -3||lcross == -3 || rcross == -3) return -1;
	}
	for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(a[i][j] == 0)
                return 0;
    return 2;
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
        static TCHAR szAppName[] = TEXT("MyWindows");
        HWND hwnd;
        MSG msg;
        WNDCLASS wndclass;

        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc = WndProc;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hInstance = hInstance;
        wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wndclass.lpszMenuName = NULL;
        wndclass.lpszClassName = szAppName;

        if (!RegisterClass(&wndclass))
        {
                MessageBox(NULL, TEXT("这个程序需要在 Windows NT 才能执行！"), szAppName, MB_ICONERROR);
                return 0;
        }

        hwnd = CreateWindow(szAppName,
                TEXT("####"),
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                NULL,
                NULL,
                hInstance,
                NULL);

        ShowWindow(hwnd, iCmdShow);
        UpdateWindow(hwnd);

        while (GetMessage(&msg, NULL, 0, 0))
        {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
        }

        return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
        HDC hdc;
        PAINTSTRUCT ps;
        RECT rect;
        HPEN hPen, hPen1;
        HBRUSH hBrush;
        static POINT point;
        static int iCount = 0;
        static bool flag = 0;
        static int sqflag[3][3];

        switch (message)
        {
        case WM_PAINT:

                hdc = BeginPaint(hwnd, &ps);
                GetClientRect(hwnd, &rect);
                hPen = CreatePen(PS_SOLID, 10, 0);
                SelectObject(hdc, hPen);

                MoveToEx(hdc, rect.right/2 - side/2, rect.bottom/2 - side/2*3, NULL);
                LineTo(hdc, rect.right/2 - side/2, rect.bottom/2 + side/2 + side);//left
                MoveToEx(hdc, rect.right/2 + side/2, rect.bottom/2 - side/2*3, NULL);
                LineTo(hdc, rect.right/2 + side/2, rect.bottom/2 + side/2 + side);//right

                MoveToEx(hdc, rect.right/2 - side/2 - side, rect.bottom/2 - side/2, NULL);
                LineTo(hdc, rect.right/2 + side/2 + side, rect.bottom/2 - side/2);//top
                MoveToEx(hdc, rect.right/2 - side/2 - side, rect.bottom/2 + side/2, NULL);
                LineTo(hdc, rect.right/2 + side/2 + side, rect.bottom/2 + side/2);//bottom

                if(flag)
                {
                    for(int i = 0; i < 3; i++)
                    for(int j = 0; j < 3; j++)
                    {
                        if(sqflag[i][j] == 1)
                        Ellipse(hdc, rect.right/2 - side/2*3 + i * side, rect.bottom/2 - side/2*3 + j * side,
                                rect.right/2 - side/2 + i * side, rect.bottom/2 - side/2 + j * side);
                        else if(sqflag[i][j] == -1)
                        {
                            MoveToEx(hdc, rect.right/2 - side/2*3 + i * side, rect.bottom/2 - side/2*3 + j * side, NULL);
                            LineTo(hdc, rect.right/2 - side/2 + i * side, rect.bottom/2 - side/2 + j * side);
                            MoveToEx(hdc, rect.right/2 - side/2 + i * side, rect.bottom/2 - side/2*3 + j * side, NULL);
                            LineTo(hdc, rect.right/2 - side/2*3 + i * side, rect.bottom/2 - side/2 + j * side);
                        }
                    }
                }
                EndPaint(hwnd, &ps);
                return 0;


        case WM_LBUTTONUP:
                point.x = GET_X_LPARAM(lParam);
                point.y = GET_Y_LPARAM(lParam);
                hdc = GetDC(hwnd);
                GetClientRect(hwnd, &rect);
                for (int i = 0; i < 3; i++)
                    for(int j = 0; j < 3; j++)
                {
                    if(rect.right/2 - side*3/2 + i * side < point.x && point.x < rect.right/2 - side /2 + i* side
                        &&rect. bottom/2 - side *3/2 + j*side < point.y && point.y < rect.bottom /2 -side/2 + j* side
                        && sqflag[i][j]==0)
                    {
                        flag ++;
                        iCount ++;
                        if(iCount%2 == 1)
                        sqflag[i][j] = 1;
                        else sqflag[i][j] = -1;
                        SendMessage(hwnd, WM_PAINT, NULL, NULL);
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                }
                if(wParam & MK_CONTROL)
                {
                for (int i = 0; i < 3; i++)
                    for(int j = 0; j < 3; j++)
                {
                    if(rect.right/2 - side*3/2 + i * side < point.x && point.x < rect.right/2 - side /2 + i* side
                        &&rect. bottom/2 - side *3/2 + j*side < point.y && point.y < rect.bottom /2 -side/2 + j* side
                        )
                    {
                        flag ++;
                        iCount ++;
                        if(iCount%2 == 1)
                        sqflag[i][j] = 1;
                        else sqflag[i][j] = -1;
                        SendMessage(hwnd, WM_PAINT, NULL, NULL);
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                }
                }
                    if(Winner(sqflag) == 1)
                    {
                        MessageBoxA(hwnd, TEXT("先手胜！"), TEXT("分出胜负！"), MB_OK);
                        SendMessageA(hwnd, WM_RBUTTONDOWN, NULL, NULL);
                    }
                    if(Winner(sqflag) == -1)
                    {
                        MessageBoxA(hwnd, TEXT("后手胜！"), TEXT("分出胜负！"), MB_OK);
                        SendMessageA(hwnd, WM_RBUTTONDOWN, NULL, NULL);
                    }
                    if(Winner(sqflag) == 2)
                    {
                        MessageBoxA(hwnd, TEXT("胜负未定！"), TEXT("平局"), MB_OK);
                        SendMessageA(hwnd, WM_RBUTTONDOWN, NULL, NULL);
                    }
                ReleaseDC(hwnd, hdc);

            return 0;

        case WM_RBUTTONDOWN:
            flag = 0;
            iCount = 0;
            for(int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                    sqflag[i][j] = 0;
            SendMessageA(hwnd, WM_PAINT, NULL, NULL);
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;

        case WM_DESTROY:
                iCount = 0;
                PostQuitMessage(0);
                return 0;
        }
        return DefWindowProc(hwnd, message, wParam, lParam);
}
