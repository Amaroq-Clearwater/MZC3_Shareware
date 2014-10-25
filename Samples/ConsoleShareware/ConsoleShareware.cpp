////////////////////////////////////////////////////////////////////////////
// ConsoleShareware.cpp --- sample of MZC3_Shareware
////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <tchar.h>

#include <iostream>
#include <string>

#if defined(UNICODE) || defined(_UNICODE)
    #error You lose.
#endif

using namespace std;

////////////////////////////////////////////////////////////////////////////

#ifndef MZC_NO_SHAREWARE
    #include "Shareware.hpp"

    class SW_ConsoleShareware : public SW_Shareware
    {
    public:
        SW_ConsoleShareware(LPCTSTR pszCompanyKey,
                            LPCTSTR pszAppKey,
                            const char *pszSha256HashHexString,
                            DWORD dwTrialDays = 15)
        : SW_Shareware(pszCompanyKey, pszAppKey,
                       pszSha256HashHexString, dwTrialDays)
        {
        }

        virtual bool UrgeRegister(HWND /*hwndParent*/)
        {
            cout << "Enter shareware password: ";
            string password;
            getline(cin, password);
            if (RegisterPassword(NULL, password.c_str()))
            {
                cout << "OK, registered." << endl;
                return true;
            }
            else
            {
                cout << "ERROR: Password mismatched." << endl;
            }
            return false;
        }

        virtual void ShowErrorMessage(HWND /*hwndParent*/, UINT uStringID)
        {
            cout << SwLoadStringDx2(m_hInstance, uStringID) << endl;
        }
    };

    SW_ConsoleShareware g_shareware(
        TEXT("Katayama Hirofumi MZ"),
        TEXT("ConsoleShareware"),
        // password is "test"
        "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08",
        15
    );
#endif  // ndef MZC_NO_SHAREWARE

////////////////////////////////////////////////////////////////////////////

int get_number(void)
{
    string str;
    getline(cin, str);
    return atoi(str.c_str());
}

////////////////////////////////////////////////////////////////////////////

void DoAdd(void)
{
    double X, Y, Z;
    cout << "I'll calculate X + Y." << endl;
    cout << "Enter X: ";
    X = get_number();
    cout << "Enter Y: ";
    Y = get_number();
    Z = X + Y;
    cout << "Result is " << Z << endl;
}

void DoSub(void)
{
    double X, Y, Z;
    cout << "I'll calculate X - Y." << endl;
    cout << "Enter X: ";
    X = get_number();
    cout << "Enter Y: ";
    Y = get_number();
    Z = X - Y;
    cout << "Result is " << Z << endl;
}

void DoMul(void)
{
    double X, Y, Z;
    cout << "I'll calculate X * Y." << endl;
    cout << "Enter X: ";
    X = get_number();
    cout << "Enter Y: ";
    Y = get_number();
    Z = X * Y;
    cout << "Result is " << Z << endl;
}

void DoDiv(void)
{
    double X, Y, Z;
    cout << "I'll calculate X / Y." << endl;
    cout << "Enter X: ";
    X = get_number();
    cout << "Enter Y: ";
    Y = get_number();
    if (Y == 0)
    {
        cout << "I cannot calculate it." << endl;
    }
    else
    {
        Z = X / Y;
        cout << "Result is " << Z << endl;
    }
}

////////////////////////////////////////////////////////////////////////////

int main(void)
{
#ifndef MZC_NO_SHAREWARE
    if (!g_shareware.Start(NULL))
    {
        // cannot use this application
        return 1;
    }
#endif  // ndef MZC_NO_SHAREWARE

    for (;;)
    {
        cout <<
            "\n[Menu]\n"
            "1. Addition\n"
            "2. Subtraction\n"
            "3. Multiplication (registration required)\n"
            "4. Division (registration required)\n"
            "5. Exit" << endl;

        cout << "Enter a number (1 - 5): ";
        switch (get_number())
        {
        case 1:
            DoAdd();
            break;

        case 2:
            DoSub();
            break;

        case 3:
#ifndef MZC_NO_SHAREWARE
            if (!g_shareware.IsRegistered())
            {
                cout << "Sorry, this feature requires the registration." <<
                        endl;

                if (g_shareware.UrgeRegister(NULL))
                {
                    DoMul();
                }
            }
            else
#endif  // ndef MZC_NO_SHAREWARE
            {
                DoMul();
            }
            break;

        case 4:
#ifndef MZC_NO_SHAREWARE
            if (!g_shareware.IsRegistered())
            {
                cout << "Sorry, this feature requires the registration." <<
                        endl;

                if (g_shareware.UrgeRegister(NULL))
                {
                    DoDiv();
                }
            }
            else
#endif  // ndef MZC_NO_SHAREWARE
            {
                DoDiv();
            }
            break;

        case 5:
            goto label_break2;

        default:
            cout << "Please enter a 1-to-5 number." << endl;
            break;
        }
    }
label_break2:

    return 0;
}

////////////////////////////////////////////////////////////////////////////
