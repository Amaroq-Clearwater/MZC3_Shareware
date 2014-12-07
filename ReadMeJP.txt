(Japanese, Shift_JIS)


これは何？
----------

これは MZC3_Shareware です。MZC3_Shareware は、C++/Win32 のシェアウェ
アを楽に作るためのフレームワークです。


使い方
------

 (1) 次の 7 つのファイルをプロジェクトフォルダに追加して下さい。

    * Shareware.hpp
    * Shareware.cpp
    * Shareware_inl.hpp
    * Shareware.rc
    * SHA-256.hpp
    * SHA-256.cpp
    * hand.cur

 (2) もしあなたのプロジェクトにリソースファイル (*.rc) があるなら、そ
     のリソースファイルの一番下に、「#include "Shareware.rc"」という
     行を追加し、Shareware.rc 以外の上記の 6 つのファイルをプロジェク
     トに追加して下さい。

     それ以外の場合は、上記の 7 つのファイルをプロジェクトに追加して
     下さい。

 (3) ファイル Shareware.rc の中の"TODO:"項目を修正して下さい。

 (4) salt文字列と 8 文字以上のパスワード文字列を思い浮かべて下さい。
     どちらもなるべく不規則な文字列にして下さい。
     
  ※ salt文字列は、パスワード以外にハッシュのパターンを変化させる調味
     料のようなものです。パスワードが少し短くても、saltが充分複雑で長
     ければ、解析が難しくなります。

 (5) SHA-256.exe により、salt 文字列に対して、パスワードに対応するハ
     ッシュ文字列を取得して下さい。例えば、salt 文字列が空文字列で、
     パスワードが "test" ならば、ハッシュ文字列は

         "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"

     になります。

    (SHA-256.exeの実行例)
        Enter 'exit' to exit.
        Enter salt string: 
        Enter password: test
        9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08
        {0x9f, 0x86, 0xd0, 0x81, 0x88, 0x4c, 0x7d, 0x65, 0x9a, 0x2f, 0xea, 0xa0, 0xc5, 0
        x5a, 0xd0, 0x15, 0xa3, 0xbf, 0x4f, 0x1b, 0x2b, 0x0b, 0x82, 0x2c, 0xd1, 0x5d, 0x6
        c, 0x15, 0xb0, 0xf0, 0x0a, 0x08, }
        Enter password: exit

 (6) #include "Shareware.hpp"をメインのソースファイルに追加して下さい。

 (7) クラス SW_Shareware の g_shareware という名前のグローバル変数の
     定義を次のように追加して下さい：

    SW_Shareware g_shareware(
        /* company registry key */      TEXT("MyCompany"),
        /* application registry key */  TEXT("MyApp"),
        /* password hash */
        "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08",
        /* trial days */                15,
        /* salt string */               "",
        /* version string */            "");

    コンストラクターの第１引数は、会社を表すレジストリキーの名前です。
    コンストラクターの第２引数は、アプリを表すレジストリキーの名前です。
    コンストラクターの第３引数は、パスワードのハッシュ文字列です。
    コンストラクターの第４引数は、試用期間の日数です（省略可）。
    コンストラクターの第５引数は、salt文字列です（省略可）。
    コンストラクターの第６引数は、アプリケーションの現在のバージョンを
    表す文字列です（省略可）。

    バージョンは、バージョンアップ時の試用期間の延長に使われます。
    延長しないときは、常に空文字列を指定して下さい。

 (8) 次のようなコードをアプリケーションの始まりに追加して下さい。

    if (!g_shareware.Start(hwnd))
    {
        // 失敗。アプリケーションを終了する。
        ...
    }

 (9) 機能制限したい機能の実行の直前に次のようなコードを追加して下さい。

    if (!g_shareware.IsRegistered())
    {
        g_shareware.ThisCommandRequiresRegistering(hwnd);
        if (!g_shareware.UrgeRegister(hwnd))
        {
            // 機能を実行しない。
            return;
        }
    }
    ...

 (10) ビルドして下さい。あなたのシェアウェアが出来上がります。実際に
      使ってみて試してみて下さい。

 (11) 好きなように Shareware.rc を改良して下さい。


注意
----

最初の起動時には、「ライセンス キーの登録」ダイアログが表示されません
（デフォルト）。


/////////////////////////////////////////////////////
// 片山博文MZ (katahiromz) [蟻]
// ホームページ http://katahiromz.web.fc2.com/
// 掲示板       http://katahiromz.bbs.fc2.com/
// メール       katayama.hirofumi.mz@gmail.com
/////////////////////////////////////////////////////
