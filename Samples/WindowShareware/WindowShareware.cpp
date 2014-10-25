////////////////////////////////////////////////////////////////////////////
// WindowShareware.cpp --- sample of MZC3_Shareware
////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "resource.h"

////////////////////////////////////////////////////////////////////////////

#ifndef MZC_NO_SHAREWARE
    #include "Shareware.hpp"

    SW_Shareware    g_shareware(
        TEXT("Katayama Hirofumi MZ"),
        TEXT("WindowShareware"),
        // password is "test"
        "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08",
        15
    );
#endif  // ndef MZC_NO_SHAREWARE

HINSTANCE       g_hInstance;
HWND            g_hMainWnd;

static LPCTSTR s_pszName = TEXT("WindowShareware");

////////////////////////////////////////////////////////////////////////////

BOOL MainWnd_OnCreate(HWND hwnd, LPCREATESTRUCT /*lpCreateStruct*/)
{
    #ifndef MZC_NO_SHAREWARE
        ::PostMessage(hwnd, WM_COMMAND, ID_SHAREWARE_START, 0);
    #endif
    return TRUE;
}

void MainWnd_OnDestroy(HWND /*hwnd*/)
{
    ::PostQuitMessage(0);
}

#ifndef MZC_NO_SHAREWARE
    void MainWnd_OnSharewareStart(HWND hwnd)
    {
        if (!g_shareware.Start(hwnd))
        {
            ::DestroyWindow(hwnd);
        }
    }
#endif  // ndef MZC_NO_SHAREWARE

////////////////////////////////////////////////////////////////////////////

void AddDlg_OnOK(HWND hwnd)
{
    int nX, nY, nZ;
    nX = ::GetDlgItemInt(hwnd, edt1, NULL, TRUE);
    nY = ::GetDlgItemInt(hwnd, edt2, NULL, TRUE);
    nZ = nX + nY;
    ::SetDlgItemInt(hwnd, edt3, nZ, TRUE);
}

void SubDlg_OnOK(HWND hwnd)
{
    int nX, nY, nZ;
    nX = ::GetDlgItemInt(hwnd, edt1, NULL, TRUE);
    nY = ::GetDlgItemInt(hwnd, edt2, NULL, TRUE);
    nZ = nX - nY;
    ::SetDlgItemInt(hwnd, edt3, nZ, TRUE);
}

void MulDlg_OnOK(HWND hwnd)
{
    int nX, nY, nZ;
    nX = ::GetDlgItemInt(hwnd, edt1, NULL, TRUE);
    nY = ::GetDlgItemInt(hwnd, edt2, NULL, TRUE);
    nZ = nX * nY;
    ::SetDlgItemInt(hwnd, edt3, nZ, TRUE);
}

void DivDlg_OnOK(HWND hwnd)
{
    int nX, nY, nZ;
    nX = ::GetDlgItemInt(hwnd, edt1, NULL, TRUE);
    nY = ::GetDlgItemInt(hwnd, edt2, NULL, TRUE);
    if (nY != 0)
    {
        nZ = nX / nY;
        ::SetDlgItemInt(hwnd, edt3, nZ, TRUE);
    }
    else
    {
        ::SetDlgItemText(hwnd, edt3, TEXT("#N/A!"));
    }
}

////////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK
AddDlg_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM /*lParam*/)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            AddDlg_OnOK(hwnd);
            break;

        case IDCANCEL:
            ::EndDialog(hwnd, IDCANCEL);
            break;
        }
        break;
    }
    return 0;
}

INT_PTR CALLBACK
SubDlg_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM /*lParam*/)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            SubDlg_OnOK(hwnd);
            break;

        case IDCANCEL:
            ::EndDialog(hwnd, IDCANCEL);
            break;
        }
        break;
    }
    return 0;
}

INT_PTR CALLBACK
MulDlg_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM /*lParam*/)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            MulDlg_OnOK(hwnd);
            break;

        case IDCANCEL:
            ::EndDialog(hwnd, IDCANCEL);
            break;
        }
        break;
    }
    return 0;
}

INT_PTR CALLBACK
DivDlg_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM /*lParam*/)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            DivDlg_OnOK(hwnd);
            break;

        case IDCANCEL:
            ::EndDialog(hwnd, IDCANCEL);
            break;
        }
        break;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////

void MainWnd_OnAdd(HWND hwnd)
{
    ::DialogBox(g_hInstance, MAKEINTRESOURCE(1), hwnd, AddDlg_DlgProc);
}

void MainWnd_OnSub(HWND hwnd)
{
    ::DialogBox(g_hInstance, MAKEINTRESOURCE(2), hwnd, SubDlg_DlgProc);
}

void MainWnd_OnMul(HWND hwnd)
{
    #ifndef MZC_NO_SHAREWARE
        if (!g_shareware.IsRegistered())
        {
            g_shareware.ThisCommandRequiresRegistering(hwnd);
            if (!g_shareware.UrgeRegister(hwnd))
                return;
        }
    #endif
    ::DialogBox(g_hInstance, MAKEINTRESOURCE(3), hwnd, MulDlg_DlgProc);
}

void MainWnd_OnDiv(HWND hwnd)
{
    #ifndef MZC_NO_SHAREWARE
        if (!g_shareware.IsRegistered())
        {
            g_shareware.ThisCommandRequiresRegistering(hwnd);
            if (!g_shareware.UrgeRegister(hwnd))
                return;
        }
    #endif
    ::DialogBox(g_hInstance, MAKEINTRESOURCE(4), hwnd, DivDlg_DlgProc);
}

////////////////////////////////////////////////////////////////////////////

void MainWnd_OnCommand(HWND hwnd, int id, HWND /*hwndCtl*/, UINT /*codeNotify*/)
{
    switch (id)
    {
    case ID_EXIT:
        ::DestroyWindow(hwnd);
        break;

#ifndef MZC_NO_SHAREWARE
    case ID_SHAREWARE_START:
        MainWnd_OnSharewareStart(hwnd);
        break;
#endif  // ndef MZC_NO_SHAREWARE

    case ID_ADD:
        MainWnd_OnAdd(hwnd);
        break;

    case ID_SUB:
        MainWnd_OnSub(hwnd);
        break;

    case ID_MUL:
        MainWnd_OnMul(hwnd);
        break;

    case ID_DIV:
        MainWnd_OnDiv(hwnd);
        break;
    }
}

LRESULT CALLBACK
MainWnd_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    HANDLE_MSG(hwnd, WM_CREATE, MainWnd_OnCreate);
    HANDLE_MSG(hwnd, WM_DESTROY, MainWnd_OnDestroy);
    HANDLE_MSG(hwnd, WM_COMMAND, MainWnd_OnCommand);
    default:
        return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

////////////////////////////////////////////////////////////////////////////

int WINAPI _tWinMain(
    HINSTANCE   hInstance,
    HINSTANCE   /*hPrevInstance*/,
    LPTSTR      /*lpCmdLine*/,
    int         nCmdShow)
{
    g_hInstance = hInstance;

    WNDCLASSEX wcx;
    ZeroMemory(&wcx, sizeof(wcx));
    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_DBLCLKS;
    wcx.lpfnWndProc = MainWnd_WndProc;
    wcx.hInstance = hInstance;
    wcx.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(1));
    wcx.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_3DFACE + 1);
    wcx.lpszMenuName = MAKEINTRESOURCE(1);
    wcx.lpszClassName = s_pszName;
    wcx.hIconSm = NULL;
    if (!::RegisterClassEx(&wcx))
    {
        ::MessageBox(NULL, TEXT("RegisterClassEx failed"), NULL,
                     MB_ICONERROR);
        return 1;
    }

    g_hMainWnd = ::CreateWindow(
        s_pszName, s_pszName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 500, 350,
        NULL, NULL, hInstance, NULL);
    if (g_hMainWnd == NULL)
    {
        ::MessageBox(NULL, TEXT("CreateWindow failed"), NULL,
                     MB_ICONERROR);
        return 2;
    }

    ::ShowWindow(g_hMainWnd, nCmdShow);
    ::UpdateWindow(g_hMainWnd);

    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

////////////////////////////////////////////////////////////////////////////
