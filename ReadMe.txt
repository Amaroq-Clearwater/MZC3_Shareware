WHAT IS THIS?
-------------

This is MZC3_Shareware. MZC3_Shareware is a framewark to easily make 
a C++/Win32 shareware.


USAGE
-----

 (1) Add the following seven files in your project folder:

    * Shareware.hpp
    * Shareware.cpp
    * Shareware_inl.hpp
    * Shareware.rc
    * SHA-256.hpp
    * SHA-256.cpp
    * hand.cur

 (2) If your project has already a resource file (*.rc), then 
     add the line of #include "Shareware.rc" at the bottom of that 
     resource file and add the six files except Shareware.rc to your 
     project. 

     Otherwise add the seven files.

 (3) Edit "TODO:" items in file Shareware.rc.

 (4) Imagine a salt string and a password string.

 (5) Get the hash string of your imagined password by SHA-256.exe for the salt.
     For example, if the salt string was empty ("") and the password was "test", 
     the hash string will be

        "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"

     as below.

    (An example of execution of SHA-256.exe)
        Enter 'exit' to exit.
        Enter salt string: 
        Enter password: test
        9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08
        {0x9f, 0x86, 0xd0, 0x81, 0x88, 0x4c, 0x7d, 0x65, 0x9a, 0x2f, 0xea, 0xa0, 0xc5, 0
        x5a, 0xd0, 0x15, 0xa3, 0xbf, 0x4f, 0x1b, 0x2b, 0x0b, 0x82, 0x2c, 0xd1, 0x5d, 0x6
        c, 0x15, 0xb0, 0xf0, 0x0a, 0x08, }
        Enter password: exit

 (6) Add #include "Shareware.hpp" to the main source file.

 (7) Add the definition of a global variable (named g_shareware) of 
     SW_Shareware class to the main source file as follows:

    SW_Shareware g_shareware(
        /* company registry key */      TEXT("MyCompany"),
        /* application registry key */  TEXT("MyApp"),
        /* password hash */
        "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08",
        /* trial days */                15,
        /* salt string */               "");

    The first parameter of constructor is the name of the company 
    registry key. The second parameter of constructor is the name of 
    the application registry key. The third parameter of constructor is 
    the hash string of the password. The fourth parameter of 
    constructor is the number of the trial days. The fifth parameter is
    the salt string.

 (8) Add code as follows to the beginning of your application.
 
    if (!g_shareware.Start(hwnd))
    {
        // Failure. Exit the application.
        ...
    }

 (9) Add code as follows before execution of the functionality 
     to be restricted if the shareware password is not registered.

    if (!g_shareware.IsRegistered())
    {
        g_shareware.ThisCommandRequiresRegistering(hwnd);
        if (!g_shareware.UrgeRegister(hwnd))
        {
            // Don't execute the functionality.
            return;
        }
    }
    ...

 (10) Build it. Your shareware will be made up. 
     Try to execute it.

 (11) Improve Shareware.rc as you like.


NOTICE
------

The first-time start-up of the application doesn't display
the "Register the shareware password" dialog (as default).


/////////////////////////////////////////////////////
// Katayama Hirofumi MZ (katahiromz) [the ant]
// Homepage     http://katahiromz.web.fc2.com/
// BBS          http://katahiromz.bbs.fc2.com/
// E-Mail       katayama.hirofumi.mz@gmail.com
/////////////////////////////////////////////////////
