////////////////////////////////////////////////////////////////////////////
// Shareware.cpp -- MZC3 shareware maker for Win32
// This file is part of MZC3.  See file "ReadMe.txt" and "License.txt".
////////////////////////////////////////////////////////////////////////////

#ifndef MZC_NO_SHAREWARE

#include <windows.h>
#include <tchar.h>
#include <cassert>
#include <string>

#include "SHA-256.hpp"
#include "Shareware.hpp"

using namespace std;

#ifdef MZC_NO_INLINING
    #undef MZC_INLINE
    #define MZC_INLINE  /*empty*/
    #include "Shareware_inl.hpp"
#endif

static LPCTSTR s_pszSoftware = TEXT("Software");
static LPCTSTR s_pszStartUse = TEXT("SW_StartUse");
static LPCTSTR s_pszCheckSum = TEXT("SW_CheckSum");
static const char *s_pszEncodedPassword = "SW_EncodedPassword";

////////////////////////////////////////////////////////////////////////////

LPTSTR SwLoadStringDx1(HINSTANCE hInstance, UINT uID)
{
    static TCHAR s_sz[2048];
    s_sz[0] = 0;
    ::LoadString(hInstance, uID, s_sz, 2048);
    assert(::lstrlen(s_sz) < 2048);
    return s_sz;
}

LPTSTR SwLoadStringDx2(HINSTANCE hInstance, UINT uID)
{
    static TCHAR s_sz[2048];
    s_sz[0] = 0;
    ::LoadString(hInstance, uID, s_sz, 2048);
    assert(::lstrlen(s_sz) < 2048);
    return s_sz;
}

////////////////////////////////////////////////////////////////////////////
// SwCenterDialog

void SwCenterDialog(HWND hwnd)
{
    assert(hwnd);
    assert(::IsWindow(hwnd));
    BOOL bChild = !!(::GetWindowLong(hwnd, GWL_STYLE) & WS_CHILD);

    HWND hwndOwner;
    if (bChild)
        hwndOwner = ::GetParent(hwnd);
    else
        hwndOwner = ::GetWindow(hwnd, GW_OWNER);

    RECT rc, rcOwner;
    if (hwndOwner)
        ::GetWindowRect(hwndOwner, &rcOwner);
    else
        ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcOwner, 0);

    ::GetWindowRect(hwnd, &rc);

    POINT pt;
    pt.x = rcOwner.left +
        ((rcOwner.right - rcOwner.left) - (rc.right - rc.left)) / 2;
    pt.y = rcOwner.top +
        ((rcOwner.bottom - rcOwner.top) - (rc.bottom - rc.top)) / 2;

    if (bChild && hwndOwner != NULL)
        ::ScreenToClient(hwndOwner, &pt);

    ::SetWindowPos(hwnd, NULL, pt.x, pt.y, 0, 0,
        SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

    ::SendMessage(hwnd, DM_REPOSITION, 0, 0);
}

////////////////////////////////////////////////////////////////////////////
// SwCenterMessageBox

static HHOOK s_hMsgBoxHook = NULL;

static LRESULT CALLBACK
SwMsgBoxCbtProc_(int nCode, WPARAM wParam, LPARAM /*lParam*/)
{
    if (nCode == HCBT_ACTIVATE)
    {
        HWND hwnd = (HWND)wParam;
        TCHAR szClassName[MAX_PATH];
        ::GetClassName(hwnd, szClassName, MAX_PATH);
        if (lstrcmpi(szClassName, TEXT("#32770")) == 0)
        {
            SwCenterDialog(hwnd);

            if (s_hMsgBoxHook != NULL && ::UnhookWindowsHookEx(s_hMsgBoxHook))
                s_hMsgBoxHook = NULL;
        }
    }
    return 0;
}

int SwCenterMessageBox(
    HWND hwndParent, LPCTSTR pszText, LPCTSTR pszCaption, UINT uMB_)
{
    if (s_hMsgBoxHook != NULL && ::UnhookWindowsHookEx(s_hMsgBoxHook))
        s_hMsgBoxHook = NULL;

    HINSTANCE hInst = reinterpret_cast<HINSTANCE>(
        GetWindowLongPtr(hwndParent, GWLP_HINSTANCE));

    DWORD dwThreadID = ::GetCurrentThreadId();
    s_hMsgBoxHook = ::SetWindowsHookEx(WH_CBT, SwMsgBoxCbtProc_, hInst, dwThreadID);

    int nID = ::MessageBox(hwndParent, pszText, pszCaption, uMB_);

    if (s_hMsgBoxHook != NULL && ::UnhookWindowsHookEx(s_hMsgBoxHook))
        s_hMsgBoxHook = NULL;

    return nID;
}

////////////////////////////////////////////////////////////////////////////
// SwMakeStaticHyperlink

struct SW_HyperlinkStatic
{
    WNDPROC fnWndProc;
    LPTSTR  pszURL;
    HCURSOR hHandCursor;
};

static LRESULT CALLBACK
HyperlinkStatic_OnPaint(HWND hwnd, HDC hdc)
{
    // fill background
    HWND hwndParent = ::GetParent(hwnd);
    LRESULT result = ::SendMessage(
        hwndParent, WM_CTLCOLORSTATIC,
        reinterpret_cast<WPARAM>(hdc),
        reinterpret_cast<LPARAM>(hwnd));
    HBRUSH hbrBackground = reinterpret_cast<HBRUSH>(result);
    RECT rc;
    ::GetClientRect(hwnd, &rc);
    ::FillRect(hdc, &rc, hbrBackground);

    // create underlined font
    result = ::SendMessage(hwnd, WM_GETFONT, 0, 0);
    HFONT hFont = reinterpret_cast<HFONT>(result);
    LOGFONT lf;
    ::GetObject(hFont, sizeof(lf), &lf);
    lf.lfUnderline = TRUE;
    hFont = ::CreateFontIndirect(&lf);

    // set text color and background mix mode
    SetTextColor(hdc, RGB(0, 0, 192));
    SetBkMode(hdc, TRANSPARENT);

    // draw text
    HGDIOBJ hFontOld = ::SelectObject(hdc, hFont);
    {
        TCHAR szText[256];
        ::GetWindowText(hwnd, szText, 256);
        UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS;
        DWORD style = GetWindowLong(hwnd, GWL_STYLE);
        if (style & SS_CENTER)
            uFormat |= DT_CENTER;
        else if (style & SS_RIGHT)
            uFormat |= DT_RIGHT;
        else
            uFormat |= DT_LEFT;

        RECT rcText = rc;
        ::DrawText(hdc, szText, -1, &rcText, uFormat | DT_CALCRECT);

        INT nClientWidth = rc.right - rc.left;
        INT nTextWidth = rcText.right - rcText.left;
        if (style & SS_CENTER)
        {
            rcText.left += (nClientWidth - nTextWidth) / 2;
            rcText.right += (nClientWidth - nTextWidth) / 2;
            ::OffsetRect(&rcText, 0, 1);
        }
        else if (style & SS_RIGHT)
        {
            rcText.left += nClientWidth - nTextWidth;
            rcText.right += nClientWidth - nTextWidth;
            ::OffsetRect(&rcText, -1, 1);
        }
        else
        {
            ::OffsetRect(&rcText, 1, 1);
        }

        // draw focus
        ::InflateRect(&rcText, 1, 1);
        if (hwnd == ::GetFocus())
            ::DrawFocusRect(hdc, &rcText);
        ::InflateRect(&rcText, -1, -1);

        ::DrawText(hdc, szText, -1, &rc, uFormat);
    }
    ::DeleteObject(::SelectObject(hdc, hFontOld));
    return 0;
}

static void
HyperlinkStatic_OnTab(HWND hwnd)
{
    if (GetAsyncKeyState(VK_SHIFT) < 0)
    {
        HWND hCtrl = hwnd;
        for (;;)
        {
            hCtrl = ::GetNextWindow(hCtrl, GW_HWNDPREV);
            if (hCtrl == NULL)
                hCtrl = ::GetWindow(hwnd, GW_HWNDLAST);
            if (hCtrl == hwnd)
                break;
            if ((::GetWindowLong(hCtrl, GWL_STYLE) & WS_TABSTOP) == 0)
                continue;
            ::SetFocus(hCtrl);
            break;
        }
    }
    else
    {
        HWND hCtrl = hwnd;
        for (;;)
        {
            hCtrl = ::GetNextWindow(hCtrl, GW_HWNDNEXT);
            if (hCtrl == NULL)
                hCtrl = ::GetWindow(hwnd, GW_HWNDFIRST);
            if (hCtrl == hwnd)
                break;
            if ((::GetWindowLong(hCtrl, GWL_STYLE) & WS_TABSTOP) == 0)
                continue;
            ::SetFocus(hCtrl);
            break;
        }
    }
}

static LRESULT CALLBACK
HyperlinkStatic_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SW_HyperlinkStatic *pHS;
    LRESULT result;

    pHS = reinterpret_cast<SW_HyperlinkStatic *>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (uMsg)
    {
    case WM_ERASEBKGND:
        return 0;

    case WM_LBUTTONDOWN:
        ::SetFocus(hwnd);
        ::ShellExecute(hwnd, NULL, pHS->pszURL, NULL, NULL, SW_SHOWNORMAL);
        break;

    case WM_MOUSEMOVE:
        if (pHS->hHandCursor)
            ::SetCursor(pHS->hHandCursor);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_SPACE || wParam == VK_RETURN)
        {
            ::ShellExecute(hwnd, NULL, pHS->pszURL, NULL, NULL, SW_SHOWNORMAL);
            return 0;
        }
        if (wParam == VK_TAB)
        {
            HyperlinkStatic_OnTab(hwnd);
            return 0;
        }
        if (wParam == VK_ESCAPE)
        {
            ::PostMessage(::GetParent(hwnd), WM_COMMAND, IDCANCEL,
                          reinterpret_cast<LPARAM>(hwnd));
            return 0;
        }
        return ::CallWindowProc(pHS->fnWndProc, hwnd, uMsg, wParam, lParam);

    case WM_GETDLGCODE:
        result = ::CallWindowProc(pHS->fnWndProc, hwnd, uMsg, wParam, lParam);
        return result | DLGC_WANTALLKEYS | DLGC_WANTTAB;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = ::BeginPaint(hwnd, &ps);
            if (hdc)
            {
                HyperlinkStatic_OnPaint(hwnd, hdc);
                ::EndPaint(hwnd, &ps);
            }
        }
        return 0;

    case WM_NCDESTROY:
        SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
        ::CallWindowProc(pHS->fnWndProc, hwnd, uMsg, wParam, lParam);
        free(pHS->pszURL);
        delete pHS;
        return 0;

    case WM_SETFOCUS:
    case WM_KILLFOCUS:
        ::InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
    return ::CallWindowProc(pHS->fnWndProc, hwnd, uMsg, wParam, lParam);
}

void SwMakeStaticHyperlink(HWND hwndCtrl, LPCTSTR pszURL/* = NULL*/)
{
    DWORD style = ::GetWindowLong(hwndCtrl, GWL_STYLE);
    style |= SS_NOPREFIX | SS_NOTIFY | WS_TABSTOP;
    ::SetWindowLong(hwndCtrl, GWL_STYLE, style);

    SW_HyperlinkStatic *pHS = new SW_HyperlinkStatic;
    if (pszURL)
    {
        pHS->pszURL = _tcsdup(pszURL);
    }
    else
    {
        TCHAR sz[256];
        ::GetWindowText(hwndCtrl, sz, 256);
        pHS->pszURL = _tcsdup(sz);
    }
    pHS->hHandCursor =
        ::LoadCursor(::GetModuleHandle(NULL), MAKEINTRESOURCE(32731));

    SetWindowLongPtr(hwndCtrl, GWLP_USERDATA,
                     reinterpret_cast<LONG_PTR>(pHS));
    pHS->fnWndProc =
        reinterpret_cast<WNDPROC>(
            SetWindowLongPtr(hwndCtrl, GWLP_WNDPROC,
                reinterpret_cast<LONG_PTR>(HyperlinkStatic_WndProc)));
}

////////////////////////////////////////////////////////////////////////////

SW_Shareware::SW_Shareware(
    LPCTSTR pszCompanyKey,
    LPCTSTR pszAppKey,
    const char *pszSha256HashHexString,
    DWORD dwTrialDays/* = 15*/,
    const char *salt/* = ""*/)
    : m_hInstance(::GetModuleHandle(NULL)),
      m_dwTrialDays(dwTrialDays),
      m_status(SW_Shareware::IN_TRIAL),
      m_pszCompanyKey(_tcsdup(pszCompanyKey)),
      m_pszAppKey(_tcsdup(pszAppKey)),
#ifdef _MSC_VER
      m_pszSha256HashHexString(_strdup(pszSha256HashHexString)),
      m_pszSalt(_strdup(salt))
#else
      m_pszSha256HashHexString(strdup(pszSha256HashHexString)),
      m_pszSalt(strdup(salt))
#endif
{
    assert(m_pszCompanyKey);
    assert(m_pszAppKey);
    assert(m_pszSha256HashHexString);
    assert(m_pszSalt);
}

SW_Shareware::SW_Shareware(
    LPCTSTR pszCompanyKey,
    LPCTSTR pszAppKey,
    const BYTE *pbHash32Bytes,
    DWORD dwTrialDays/* = 15*/,
    const char *salt/* = ""*/)
    : m_hInstance(::GetModuleHandle(NULL)),
      m_dwTrialDays(dwTrialDays),
      m_status(SW_Shareware::IN_TRIAL),
      m_pszCompanyKey(_tcsdup(pszCompanyKey)),
      m_pszAppKey(_tcsdup(pszAppKey)),
      m_pszSha256HashHexString(NULL),
#ifdef _MSC_VER
      m_pszSalt(_strdup(salt))
#else
      m_pszSalt(strdup(salt))
#endif
{
    assert(m_pszSalt);
    #ifdef MStringA
        MStringA str;
    #else
        std::string str;
    #endif
    MzcHexStringFromBytes(str, pbHash32Bytes, pbHash32Bytes + 32);
#ifdef _MSC_VER
    m_pszSha256HashHexString = _strdup(str.c_str());
#else
    m_pszSha256HashHexString = strdup(str.c_str());
#endif
}

/*virtual*/ SW_Shareware::~SW_Shareware()
{
    free(m_pszCompanyKey);
    free(m_pszAppKey);
    free(m_pszSha256HashHexString);
    free(m_pszSalt);
}

bool SW_Shareware::Start(HWND hwndParent/* = NULL*/)
{
    CheckRegistry(hwndParent);

    switch (m_status)
    {
    case SW_Shareware::IN_TRIAL_FIRST_TIME:
        OnTrialFirstTime(hwndParent);
        return true;

    case SW_Shareware::IN_TRIAL:
        OnTrial(hwndParent);
        return true;

    case SW_Shareware::REGD:
        return true;

    case SW_Shareware::OUT_OF_TRIAL:
        return OnOutOfTrial(hwndParent);

    default:
        return false;
    }
}

void SwUrgeRegisterDlg_OnInit(HWND hwnd, SW_Shareware *pShareware)
{
    LPTSTR pszMsg;
    TCHAR szMsg[MAX_PATH * 3];
    pShareware->CheckDate();

    switch (pShareware->m_status)
    {
    case SW_Shareware::IN_TRIAL_FIRST_TIME:
        pszMsg = SwLoadStringDx1(pShareware->m_hInstance, 32732);
        ::wsprintf(szMsg, pszMsg, pShareware->m_dwTrialDays);
        ::SetDlgItemText(hwnd, edt1, szMsg);
        break;

    case SW_Shareware::IN_TRIAL:
        if (pShareware->m_dwlTotalMinutesRemains >= 60 * 24)
        {
            pszMsg = SwLoadStringDx1(pShareware->m_hInstance, 32732);
            ::wsprintf(szMsg, pszMsg,
                static_cast<DWORD>(
                    pShareware->m_dwlTotalMinutesRemains / (60 * 24)));
            ::SetDlgItemText(hwnd, edt1, szMsg);
        }
        else
        {
            pszMsg = SwLoadStringDx1(pShareware->m_hInstance, 32733);
            ::wsprintf(szMsg, pszMsg,
                static_cast<DWORD>(
                    pShareware->m_dwlTotalMinutesRemains / 60),
                static_cast<DWORD>(
                    pShareware->m_dwlTotalMinutesRemains % 60));
            ::SetDlgItemText(hwnd, edt1, szMsg);
        }
        break;

    case SW_Shareware::OUT_OF_TRIAL:
        pszMsg = SwLoadStringDx1(pShareware->m_hInstance, 32734);
        ::SetDlgItemText(hwnd, edt1, pszMsg);
        break;

    case SW_Shareware::REGD:
        ::EnableWindow(::GetDlgItem(hwnd, edt2), FALSE);
        ::EnableWindow(::GetDlgItem(hwnd, IDOK), FALSE);
        pszMsg = SwLoadStringDx1(pShareware->m_hInstance, 32735);
        ::SetDlgItemText(hwnd, edt1, pszMsg);
        ::SetFocus(::GetDlgItem(hwnd, IDCANCEL));
        break;
    }
}

static INT_PTR CALLBACK
SwUrgeRegisterDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static SW_Shareware *s_pShareware = NULL;
    char szPassword[sw_shareware_max_password];

    switch (uMsg)
    {
    case WM_INITDIALOG:
        SwCenterDialog(hwnd);
        s_pShareware = reinterpret_cast<SW_Shareware *>(lParam);
        SwMakeStaticHyperlink(hwnd, stc1, NULL);
        SwUrgeRegisterDlg_OnInit(hwnd, s_pShareware);
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            szPassword[0] = 0;
            GetDlgItemTextA(hwnd, edt2, szPassword, sw_shareware_max_password);
            if (s_pShareware->RegisterPassword(hwnd, szPassword))
            {
                SwUrgeRegisterDlg_OnInit(hwnd, s_pShareware);
                TCHAR szTitle[256];
                GetWindowText(hwnd, szTitle, 256);
                SwCenterMessageBox(hwnd,
                    SwLoadStringDx1(s_pShareware->m_hInstance, 32735),
                    szTitle, MB_ICONINFORMATION);
            }
            else
            {
                s_pShareware->ShowErrorMessage(hwnd, 32731);
            }
            break;

        case IDCANCEL:
            ::EndDialog(hwnd, IDCANCEL);
            break;
        }
    }
    return 0;
}

/*virtual*/ bool SW_Shareware::UrgeRegister(HWND hwndParent/* = NULL*/)
{
    ::DialogBoxParam(m_hInstance, MAKEINTRESOURCE(32731),
                     hwndParent, SwUrgeRegisterDlgProc,
                     reinterpret_cast<LPARAM>(this));
    return IsRegistered();
}

bool SW_Shareware::CheckRegistry(HWND hwndParent)
{
    LONG result;
    HKEY hkeySoftware, hkeyCompany, hkeyApp;
    DWORD dwDisp;
    bool bHasAppKey = false;
    bool bCanUse = false;

    result = ::RegCreateKeyEx(HKEY_CURRENT_USER, s_pszSoftware, 0,
                              NULL, 0, KEY_READ | KEY_WRITE, NULL,
                              &hkeySoftware, &dwDisp);
    if (result == ERROR_SUCCESS)
    {
        result = ::RegOpenKeyEx(hkeySoftware,
                                m_pszCompanyKey, 0,
                                KEY_READ | KEY_WRITE, &hkeyCompany);
        if (result == ERROR_SUCCESS)
        {
            result = ::RegOpenKeyEx(hkeyCompany, m_pszAppKey, 0,
                                    KEY_READ | KEY_WRITE, &hkeyApp);
            if (result == ERROR_SUCCESS)
            {
                bHasAppKey = true;
                bCanUse = CheckAppKey(hwndParent, hkeyApp);
                ::RegCloseKey(hkeyApp);
            }
            ::RegCloseKey(hkeyCompany);
        }
        ::RegCloseKey(hkeySoftware);
    }

    if (!bHasAppKey)
    {
        SetRegistryFirstTime(hwndParent);
        m_status = SW_Shareware::IN_TRIAL_FIRST_TIME;
        bCanUse = true;
    }

    return bCanUse;
}

bool SW_Shareware::SetRegistryFirstTime(HWND hwndParent)
{
    LONG result;
    HKEY hkeySoftware, hkeyCompany, hkeyApp;
    DWORD dwDisp;
    bool bSuccess = false;

    result = ::RegCreateKeyEx(HKEY_CURRENT_USER, s_pszSoftware, 0,
                              NULL, 0, KEY_READ | KEY_WRITE, NULL,
                              &hkeySoftware, &dwDisp);
    if (result == ERROR_SUCCESS)
    {
        result = ::RegCreateKeyEx(hkeySoftware, m_pszCompanyKey,
            0, NULL, 0, KEY_READ | KEY_WRITE, NULL, &hkeyCompany, &dwDisp);
        if (result == ERROR_SUCCESS)
        {
            result = ::RegCreateKeyEx(hkeyCompany, m_pszAppKey,
                0, NULL, 0, KEY_READ | KEY_WRITE, NULL, &hkeyApp, &dwDisp);
            if (result == ERROR_SUCCESS)
            {
                FILETIME ft;
                ::GetSystemTimeAsFileTime(&ft);

                DWORD cb = static_cast<DWORD>(sizeof(ft));
                result = ::RegSetValueEx(hkeyApp, s_pszStartUse,
                    0, REG_BINARY, reinterpret_cast<const BYTE *>(&ft), cb);
                if (result == ERROR_SUCCESS)
                {
                    result = ::RegQueryValueEx(
                        hkeyApp, s_pszStartUse, NULL,
                        NULL, NULL, NULL);
                    if (result == ERROR_SUCCESS)
                    {
                        bSuccess = true;
                    }
                    else
                    {
                        ShowErrorMessage(hwndParent, 32736);
                    }
                }

                ::RegDeleteValueA(hkeyApp, s_pszEncodedPassword);

                ::RegCloseKey(hkeyApp);
            }
            ::RegCloseKey(hkeyCompany);
        }
        ::RegCloseKey(hkeySoftware);
    }

    return bSuccess;
}

bool SW_Shareware::RegisterPassword(HWND hwndParent, const char *pszPassword)
{
    LONG result;
    HKEY hkeySoftware, hkeyCompany, hkeyApp;
    DWORD dwDisp;
    bool bSuccess = false;

    // check password
    if (!IsPasswordValid(pszPassword))
    {
        ::Sleep(750);
        return false;
    }

    // duplicate and encode the password
#ifdef _MSC_VER
    char *pszEncodedPassword = _strdup(pszPassword);
#else
    char *pszEncodedPassword = strdup(pszPassword);
#endif
    DWORD size = static_cast<DWORD>(::lstrlenA(pszEncodedPassword));
    EncodePassword(pszEncodedPassword, size);

#ifndef NDEBUG
    DecodePassword(pszEncodedPassword, size);
    assert(memcmp(pszEncodedPassword, pszPassword, size) == 0);
    EncodePassword(pszEncodedPassword, size);
#endif

    result = ::RegCreateKeyEx(HKEY_CURRENT_USER, s_pszSoftware, 0,
                              NULL, 0, KEY_READ | KEY_WRITE, NULL,
                              &hkeySoftware, &dwDisp);
    if (result == ERROR_SUCCESS)
    {
        result = ::RegCreateKeyEx(hkeySoftware, m_pszCompanyKey,
            0, NULL, 0, KEY_READ | KEY_WRITE, NULL, &hkeyCompany, &dwDisp);
        if (result == ERROR_SUCCESS)
        {
            result = ::RegCreateKeyEx(hkeyCompany, m_pszAppKey,
                0, NULL, 0, KEY_READ | KEY_WRITE, NULL, &hkeyApp, &dwDisp);
            if (result == ERROR_SUCCESS)
            {
                bSuccess = true;

                // set password
                DWORD cb = size;
                result = ::RegSetValueExA(hkeyApp, s_pszEncodedPassword, 0, REG_BINARY,
                    reinterpret_cast<const BYTE *>(pszEncodedPassword), cb);
                if (result != ERROR_SUCCESS)
                {
                    bSuccess = false;
                }
                else
                {
                    result = ::RegQueryValueExA(hkeyApp, s_pszEncodedPassword, NULL,
                        NULL, NULL, NULL);
                    if (result != ERROR_SUCCESS)
                    {
                        ShowErrorMessage(hwndParent, 32736);
                        bSuccess = false;
                    }
                }

                if (bSuccess)
                {
                    // set check sum
                    DWORD dwCheckSum = GetUserCheckSum();
                    cb = static_cast<DWORD>(sizeof(DWORD));
                    result = ::RegSetValueEx(hkeyApp, s_pszCheckSum, 0, REG_DWORD,
                        reinterpret_cast<const BYTE *>(&dwCheckSum), cb);
                    if (result != ERROR_SUCCESS)
                    {
                        bSuccess = false;
                    }
                    else
                    {
                        result = ::RegQueryValueEx(hkeyApp, s_pszCheckSum, NULL,
                            NULL, NULL, NULL);
                        if (result != ERROR_SUCCESS)
                        {
                            ShowErrorMessage(hwndParent, 32736);
                            bSuccess = false;
                        }
                    }
                }

                ::RegCloseKey(hkeyApp);
            }
            ::RegCloseKey(hkeyCompany);
        }
        ::RegCloseKey(hkeySoftware);
    }

    if (bSuccess)
    {
        m_status = SW_Shareware::REGD;
    }

    free(pszEncodedPassword);

    return bSuccess;
}

bool SW_Shareware::CheckAppKey(HWND hwndParent, HKEY hkeyApp)
{
    LONG result;
    DWORD cb;

    m_dwlTotalMinutesRemains = 0;

    // check password
    char szPassword[sw_shareware_max_password];
    cb = sw_shareware_max_password - 1;
    result = ::RegQueryValueExA(hkeyApp, s_pszEncodedPassword, NULL,
        NULL, reinterpret_cast<LPBYTE>(&szPassword), &cb);
    if (result == ERROR_SUCCESS)
    {
        szPassword[cb] = 0;
        DecodePassword(szPassword, cb);
        if (IsPasswordValid(szPassword))
        {
            // check check sum
            DWORD dwValue;
            cb = static_cast<DWORD>(sizeof(DWORD));
            result = ::RegQueryValueEx(hkeyApp, s_pszCheckSum, NULL,
                NULL, reinterpret_cast<LPBYTE>(&dwValue), &cb);
            if (result == ERROR_SUCCESS && dwValue == GetUserCheckSum())
            {
                m_status = SW_Shareware::REGD;
                return true;
            }
        }
    }

    // check date
    FILETIME ft;
    cb = static_cast<DWORD>(sizeof(ft));
    result = ::RegQueryValueEx(hkeyApp, s_pszStartUse, NULL,
        NULL, reinterpret_cast<LPBYTE>(&ft), &cb);
    if (result == ERROR_SUCCESS)
    {
        m_ftStart = ft;
        if (CheckDate())
        {
            m_status = SW_Shareware::IN_TRIAL;
            return true;
        }
        else
        {
            m_status = SW_Shareware::OUT_OF_TRIAL;
            return false;
        }
    }
    else
    {
        // set date
        ::GetSystemTimeAsFileTime(&ft);
        m_ftStart = ft;
        cb = static_cast<DWORD>(sizeof(ft));
        ::RegSetValueEx(hkeyApp, s_pszStartUse, 0, REG_BINARY,
            reinterpret_cast<BYTE *>(&ft), cb);

        // query date for registry check
        LONG result;
        result = ::RegQueryValueEx(hkeyApp, s_pszStartUse, NULL,
            NULL, NULL, NULL);
        if (result == ERROR_SUCCESS)
        {
            m_status = SW_Shareware::IN_TRIAL;
            return true;
        }
        else
        {
            ShowErrorMessage(hwndParent, 32736);
            return false;
        }
    }
}

/*virtual*/ bool
SW_Shareware::IsPasswordValid(const char *pszPassword) const
{
    #ifdef MStringA
        MStringA str;
    #else
        std::string str;
    #endif
    MzcGetSha256HexString(str, pszPassword, m_pszSalt);
    return (str == m_pszSha256HashHexString);
}

bool SW_Shareware::CheckDate()
{
    ULARGE_INTEGER uli;

    uli.LowPart = m_ftStart.dwLowDateTime;
    uli.HighPart = m_ftStart.dwHighDateTime;
    DWORDLONG dwlStart = uli.QuadPart;

    FILETIME ftNow;
    ::GetSystemTimeAsFileTime(&ftNow);
    uli.LowPart = ftNow.dwLowDateTime;
    uli.HighPart = ftNow.dwHighDateTime;
    DWORDLONG dwlNow = uli.QuadPart;

    LONGLONG delta = GetTrialDays();
    delta *= 24 * 60 * 60;
    delta *= 10000000;

    if (dwlStart + delta > dwlNow)
    {
        m_dwlTotalMinutesRemains = (dwlStart + delta) - dwlNow;
        m_dwlTotalMinutesRemains /= 10000000;
        m_dwlTotalMinutesRemains /= 60;
        return true;
    }

    m_dwlTotalMinutesRemains = 0;
    return false;
}

DWORD SW_Shareware::GetUserCheckSum() const
{
    DWORD dwCheckSum = 0xDeadFace;
    TCHAR szUser[64];
    DWORD dwSize = 64;
    if (::GetUserName(szUser, &dwSize))
    {
        LPTSTR pch = szUser;
        while (*pch)
        {
            dwCheckSum += *pch;
            ++pch;
        }
    }
    return dwCheckSum;
}

/*virtual*/ void
SW_Shareware::EncodePassword(void *pass, DWORD size) const
{
    // TODO:
    BYTE *pb = reinterpret_cast<BYTE *>(pass);
    while (size--)
    {
        *pb ^= 0xFF;
        pb++;
    }
}

/*virtual*/ void
SW_Shareware::DecodePassword(void *pass, DWORD size) const
{
    // TODO:
    BYTE *pb = reinterpret_cast<BYTE *>(pass);
    while (size--)
    {
        *pb ^= 0xFF;
        pb++;
    }
}

////////////////////////////////////////////////////////////////////////////

#endif  // ndef MZC_NO_SHAREWARE
