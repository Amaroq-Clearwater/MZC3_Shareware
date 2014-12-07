(Japanese, Shift_JIS)


����͉��H
----------

����� MZC3_Shareware �ł��BMZC3_Shareware �́AC++/Win32 �̃V�F�A�E�F
�A���y�ɍ�邽�߂̃t���[�����[�N�ł��B


�g����
------

 (1) ���� 7 �̃t�@�C�����v���W�F�N�g�t�H���_�ɒǉ����ĉ������B

    * Shareware.hpp
    * Shareware.cpp
    * Shareware_inl.hpp
    * Shareware.rc
    * SHA-256.hpp
    * SHA-256.cpp
    * hand.cur

 (2) �������Ȃ��̃v���W�F�N�g�Ƀ��\�[�X�t�@�C�� (*.rc) ������Ȃ�A��
     �̃��\�[�X�t�@�C���̈�ԉ��ɁA�u#include "Shareware.rc"�v�Ƃ���
     �s��ǉ����AShareware.rc �ȊO�̏�L�� 6 �̃t�@�C�����v���W�F�N
     �g�ɒǉ����ĉ������B

     ����ȊO�̏ꍇ�́A��L�� 7 �̃t�@�C�����v���W�F�N�g�ɒǉ�����
     �������B

 (3) �t�@�C�� Shareware.rc �̒���"TODO:"���ڂ��C�����ĉ������B

 (4) salt������� 8 �����ȏ�̃p�X���[�h��������v�������ׂĉ������B
     �ǂ�����Ȃ�ׂ��s�K���ȕ�����ɂ��ĉ������B
     
  �� salt������́A�p�X���[�h�ȊO�Ƀn�b�V���̃p�^�[����ω������钲��
     ���̂悤�Ȃ��̂ł��B�p�X���[�h�������Z���Ă��Asalt���[�����G�Œ�
     ����΁A��͂�����Ȃ�܂��B

 (5) SHA-256.exe �ɂ��Asalt ������ɑ΂��āA�p�X���[�h�ɑΉ�����n
     �b�V����������擾���ĉ������B�Ⴆ�΁Asalt �����񂪋󕶎���ŁA
     �p�X���[�h�� "test" �Ȃ�΁A�n�b�V���������

         "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"

     �ɂȂ�܂��B

    (SHA-256.exe�̎��s��)
        Enter 'exit' to exit.
        Enter salt string: 
        Enter password: test
        9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08
        {0x9f, 0x86, 0xd0, 0x81, 0x88, 0x4c, 0x7d, 0x65, 0x9a, 0x2f, 0xea, 0xa0, 0xc5, 0
        x5a, 0xd0, 0x15, 0xa3, 0xbf, 0x4f, 0x1b, 0x2b, 0x0b, 0x82, 0x2c, 0xd1, 0x5d, 0x6
        c, 0x15, 0xb0, 0xf0, 0x0a, 0x08, }
        Enter password: exit

 (6) #include "Shareware.hpp"�����C���̃\�[�X�t�@�C���ɒǉ����ĉ������B

 (7) �N���X SW_Shareware �� g_shareware �Ƃ������O�̃O���[�o���ϐ���
     ��`�����̂悤�ɒǉ����ĉ������F

    SW_Shareware g_shareware(
        /* company registry key */      TEXT("MyCompany"),
        /* application registry key */  TEXT("MyApp"),
        /* password hash */
        "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08",
        /* trial days */                15,
        /* salt string */               "",
        /* version string */            "");

    �R���X�g���N�^�[�̑�P�����́A��Ђ�\�����W�X�g���L�[�̖��O�ł��B
    �R���X�g���N�^�[�̑�Q�����́A�A�v����\�����W�X�g���L�[�̖��O�ł��B
    �R���X�g���N�^�[�̑�R�����́A�p�X���[�h�̃n�b�V��������ł��B
    �R���X�g���N�^�[�̑�S�����́A���p���Ԃ̓����ł��i�ȗ��j�B
    �R���X�g���N�^�[�̑�T�����́Asalt������ł��i�ȗ��j�B
    �R���X�g���N�^�[�̑�U�����́A�A�v���P�[�V�����̌��݂̃o�[�W������
    �\��������ł��i�ȗ��j�B

    �o�[�W�����́A�o�[�W�����A�b�v���̎��p���Ԃ̉����Ɏg���܂��B
    �������Ȃ��Ƃ��́A��ɋ󕶎�����w�肵�ĉ������B

 (8) ���̂悤�ȃR�[�h���A�v���P�[�V�����̎n�܂�ɒǉ����ĉ������B

    if (!g_shareware.Start(hwnd))
    {
        // ���s�B�A�v���P�[�V�������I������B
        ...
    }

 (9) �@�\�����������@�\�̎��s�̒��O�Ɏ��̂悤�ȃR�[�h��ǉ����ĉ������B

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

 (10) �r���h���ĉ������B���Ȃ��̃V�F�A�E�F�A���o���オ��܂��B���ۂ�
      �g���Ă݂Ď����Ă݂ĉ������B

 (11) �D���Ȃ悤�� Shareware.rc �����ǂ��ĉ������B


����
----

�ŏ��̋N�����ɂ́A�u���C�Z���X �L�[�̓o�^�v�_�C�A���O���\������܂���
�i�f�t�H���g�j�B


/////////////////////////////////////////////////////
// �ЎR����MZ (katahiromz) [�a]
// �z�[���y�[�W http://katahiromz.web.fc2.com/
// �f����       http://katahiromz.bbs.fc2.com/
// ���[��       katayama.hirofumi.mz@gmail.com
/////////////////////////////////////////////////////
