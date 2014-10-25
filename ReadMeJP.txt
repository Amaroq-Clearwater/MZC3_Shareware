(Japanese, Shift_JIS)


これは何？
----------

これはMZC3_Sharewareです。MZC3_Sharewareは、C++/Win32の
シェアウェアを楽に作るためのソフトウェアです。


使い方
------

 (1) 次の7つのファイルをプロジェクトフォルダに追加して下さい。

    * Shareware.hpp
    * Shareware.cpp
    * Shareware_inl.hpp
    * Shareware.rc
    * SHA-256.hpp
    * SHA-256.cpp
    * hand.cur

 (2) もしあなたのプロジェクトにリソースファイル (*.rc)があるなら、
     そのリソースファイルの一番下に、「#include "Shareware.rc"」と
     いう行を追加し、Shareware.rc以外の上記の6つのファイルを
     プロジェクトに追加して下さい。
     
     それ以外の場合は、上記の7つのファイルをプロジェクトに追加して下さい。

 (3) ファイルShareware.rcの中の"TODO:"項目を修正して下さい。

 (4) SHA-256.exeにより、パスワードに対応するハッシュ文字列を
     取得して下さい。例えば、パスワードが"test"ならば、ハッシュ文字列は
     "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"
     になります。

    (SHA-256.exeの実行例)
      Enter 'exit' to exit.
      SHA-256> test
      9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08
      SHA-256> exit

 (5) #include "Shareware.hpp"をメインのソースファイルに追加して下さい。

 (6) クラスSW_Sharewareのg_sharewareという名前のグローバル変数の定義を
     次のように追加して下さい：

    SW_Shareware g_shareware(
        /* company registry key */      TEXT("MyCompany"),
        /* application registry key */  TEXT("MyApp"),
        /* password hash */
        "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08",
        /* trial days */                15);

    コンストラクターの第１引数は、会社を表すレジストリキーの名前です。
    コンストラクターの第２引数は、アプリを表すレジストリキーの名前です。
    コンストラクターの第３引数は、パスワードのハッシュ文字列です。
    コンストラクターの第４引数は、試用期間の日数です。

 (7) 次のようなコードをアプリケーションの始まりに追加して下さい。

    if (!g_shareware.Start(hwnd))
    {
        // 失敗。アプリケーションを終了する。
        ...
    }

 (8) 機能制限したい機能の実行の直前に次のようなコードを
     追加して下さい。

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

 (9) ビルドして下さい。あなたのシェアウェアが出来上がります。
     実際に使ってみて試してみて下さい。

 (10) 好きなようにShareware.rcを改良して下さい。


注意
----

最初の起動時には、「ライセンス キーの登録」ダイアログが
表示されません（デフォルト）。


/////////////////////////////////////////////////////
// 片山博文MZ (katahiromz) [蟻]
// ホームページ http://katahiromz.web.fc2.com/
// 掲示板       http://katahiromz.bbs.fc2.com/
// メール       katayama.hirofumi.mz@gmail.com
/////////////////////////////////////////////////////
