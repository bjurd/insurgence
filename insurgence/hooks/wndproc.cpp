#include "wndproc.h"

#include "../globals.h"
#include <Windows.h>

#include "../features/menu.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND Window, UINT Message, WPARAM Wide, LPARAM Long);
HWND MainWindow;
WNDPROC oWndProc;

LRESULT __stdcall hkWndProc(HWND Window, UINT Message, WPARAM Wide, LPARAM Long)
{
    // TODO: The rest (not click thru imgui)

    ImGui_ImplWin32_WndProcHandler(Window, Message, Wide, Long);

    static Menu* MenuFeature = (Menu*)Globals->FeaturesManager->Get("Menu");

    if (MenuFeature && MenuFeature->IsOpen)
    {
        switch (Message)
        {
            // Don't bother with IO since they're both forced to true in Menu setup
            case WM_MOUSEMOVE:
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MOUSEWHEEL:
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_CHAR:
                return 0;
        }
    }

	return CallWindowProcA(oWndProc, Window, Message, Wide, Long);
}

void WndProc::Create()
{
    MainWindow = FindWindow(TEXT("Valve001"), nullptr);

    if (!MainWindow)
    {
        printf("No main window\n");
        return;
    }

    oWndProc = (WNDPROC)SetWindowLongPtr(MainWindow, GWLP_WNDPROC, (__int64)(LONG_PTR)hkWndProc);

    if (!oWndProc)
        printf("Failed to hook WndProc\n");
}

void WndProc::Destroy()
{
    if (MainWindow && oWndProc)
        SetWindowLongPtr(MainWindow, GWLP_WNDPROC, (__int64)(LONG_PTR)oWndProc);
}