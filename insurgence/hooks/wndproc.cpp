#include "wndproc.h"

#include <Windows.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND Window, UINT Message, WPARAM Wide, LPARAM Long);
HWND MainWindow;
WNDPROC oWndProc;

LRESULT __stdcall hkWndProc(HWND Window, UINT Message, WPARAM Wide, LPARAM Long)
{
    // TODO: The rest

    if (ImGui_ImplWin32_WndProcHandler(Window, Message, Wide, Long))
        return true;

	return CallWindowProcA(oWndProc, Window, Message, Wide, Long);
}

BOOL CALLBACK EnumWindowsProc(HWND Window, LPARAM Reserved)
{
    DWORD ProcessID;
    GetWindowThreadProcessId(Window, &ProcessID);

    if (ProcessID != GetCurrentProcessId()) return true;
    if (GetWindow(Window, GW_OWNER) != NULL) return true;
    if (!IsWindowVisible(Window)) return true;

    MainWindow = Window;

    return false;
}

void WndProc::Create()
{
    EnumWindows(EnumWindowsProc, 0);

    if (!MainWindow)
    {
        printf("No main window\n");
        return;
    }

    oWndProc = (WNDPROC)SetWindowLongPtrA(MainWindow, GWLP_WNDPROC, (__int64)(LONG_PTR)hkWndProc);
}

void WndProc::Destroy()
{
    if (MainWindow)
        SetWindowLongPtrA(MainWindow, GWLP_WNDPROC, (__int64)(LONG_PTR)oWndProc);
}