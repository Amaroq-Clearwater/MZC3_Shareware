(Japanese, Shift_JIS)


����͉��H
----------

�����MZC3_Shareware�ł��BMZC3_Shareware�́AC++/Win32��
�V�F�A�E�F�A���y�ɍ�邽�߂̃\�t�g�E�F�A�ł��B


�g����
------

 (1) ����7�̃t�@�C�����v���W�F�N�g�t�H���_�ɒǉ����ĉ������B

    * Shareware.hpp
    * Shareware.cpp
    * Shareware_inl.hpp
    * Shareware.rc
    * SHA-256.hpp
    * SHA-256.cpp
    * hand.cur

 (2) �������Ȃ��̃v���W�F�N�g�Ƀ��\�[�X�t�@�C�� (*.rc)������Ȃ�A
     ���̃��\�[�X�t�@�C���̈�ԉ��ɁA�u#include "Shareware.rc"�v��
     �����s��ǉ����AShareware.rc�ȊO�̏�L��6�̃t�@�C����
     �v���W�F�N�g�ɒǉ����ĉ������B
     
     ����ȊO�̏ꍇ�́A��L��7�̃t�@�C�����v���W�F�N�g�ɒǉ����ĉ������B

 (3) �t�@�C��Shareware.rc�̒���"TODO:"���ڂ��C�����ĉ������B

 (4) SHA-256.exe�ɂ��A�p�X���[�h�ɑΉ�����n�b�V���������
     �擾���ĉ������B�Ⴆ�΁A�p�X���[�h��"test"�Ȃ�΁A�n�b�V���������
     "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"
     �ɂȂ�܂��B

    (SHA-256.exe�̎��s��)
      Enter 'exit' to exit.
      SHA-256> test
      9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08
      SHA-256> exit

 (5) #include "Shareware.hpp"�����C���̃\�[�X�t�@�C���ɒǉ����ĉ������B

 (6) �N���XSW_Shareware��g_shareware�Ƃ������O�̃O���[�o���ϐ��̒�`��
     ���̂悤�ɒǉ����ĉ������F

    SW_Shareware g_shareware(
        /* company registry key */      TEXT("MyCompany"),
        /* application registry key */  TEXT("MyApp"),
        /* password hash */
        "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08",
        /* trial days */                15);

    �R���X�g���N�^�[�̑�P�����́A��Ђ�\�����W�X�g���L�[�̖��O�ł��B
    �R���X�g���N�^�[�̑�Q�����́A�A�v����\�����W�X�g���L�[�̖��O�ł��B
    �R���X�g���N�^�[�̑�R�����́A�p�X���[�h�̃n�b�V��������ł��B
    �R���X�g���N�^�[�̑�S�����́A���p���Ԃ̓����ł��B

 (7) ���̂悤�ȃR�[�h���A�v���P�[�V�����̎n�܂�ɒǉ����ĉ������B

    if (!g_shareware.Start(hwnd))
    {
        // ���s�B�A�v���P�[�V�������I������B
        ...
    }

 (8) �@�\�����������@�\�̎��s�̒��O�Ɏ��̂悤�ȃR�[�h��
     �ǉ����ĉ������B

    if (!g_shareware.IsRegistered())
    {
        g_shareware.ThisCommandRequiresRegistering(hwnd);
        if (!g_shareware.UrgeRegister(hwnd))
        {
            // �@�\�����s���Ȃ��B
            return;
        }
    }
    ...

 (9) �r���h���ĉ������B���Ȃ��̃V�F�A�E�F�A���o���オ��܂��B
     ���ۂɎg���Ă݂Ď����Ă݂ĉ������B

 (10) �D���Ȃ悤��Shareware.rc�����ǂ��ĉ������B


����
----

�ŏ��̋N�����ɂ́A�u���C�Z���X �L�[�̓o�^�v�_�C�A���O��
�\������܂���i�f�t�H���g�j�B


/////////////////////////////////////////////////////
// �ЎR����MZ (katahiromz) [�a]
// �z�[���y�[�W http://katahiromz.web.fc2.com/
// �f����       http://katahiromz.bbs.fc2.com/
// ���[��       katayama.hirofumi.mz@gmail.com
/////////////////////////////////////////////////////
