WHAT IS THIS?
-------------

This is MZC3_Shareware. MZC3_Shareware is software to easily make 
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

 (4) Get the hash string of your imagined password by SHA-256.exe.
     For example, if the password was "test", the hash string will be
     "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"
     as below.

    (An example of execution of SHA-256.exe)
      Enter 'exit' to exit.
      SHA-256> test
      9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08
      SHA-256> exit

 (5) Add #include "Shareware.hpp" to the main source file.

 (6) Add the definition of a global variable (named g_shareware) of 
     SW_Shareware class to the main source file of your source as follows:

    SW_Shareware g_shareware(
        /* company registry key */      TEXT("MyCompany"),
        /* application registry key */  TEXT("MyApp"),
        /* password hash */
        "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08",
        /* trial days */                15);

    The first parameter of constructor is the name of the company 
    registry key. The second parameter of constructor is the name of 
    the software registry key. The third parameter of constructor is 
    the hash string of the password. The fourth parameter of 
    constructor is the number of the trial days.

 (7) Add code as follows to the beginning of your application.
 
    if (!g_shareware.Start(hwnd))
    {
        // Failure. Exit the application.
        ...
    }

 (8) Add code as follows before execution of the functionality 
     to be restricted if the shareware password is not registered.

    if (!g_shareware.IsRegistered())
    {
        g_shareware.ThisCommandRequiresRegistering(hwnd);
        if (!g_shareware.UrgeRegister(hwnd))
        {
            // Don't execute the functionality.
            ...
        }
    }
    ...

 (9) Build it. Your shareware will be made up. 
     Try to execute it.

 (10) Improve Shareware.rc as you like.


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
