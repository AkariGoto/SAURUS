/**
 *  �t�@�C����
 *		BasicDirectX.h
 *  ����
 *		DirectX�̒�`
 *  ���t
 *		�쐬��: 2008/03/10(MON)		�X�V��: 2008/03/10(MON)	
 */
#ifndef __BasicDirectX_h__
#define __BasicDirectX_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "DebugOutput.h"


/**
 *	------------------------------------------------------------
 *		DirectInput
 *	------------------------------------------------------------
 */
/// DirectX�̃o�[�W�����錾�i�w�b�_���O�Ő錾����j
#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>

/**
 *	�E�B���h�E�Y�w�b�_�BWinNT4.0�AWin98�ȍ~
 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x400
#endif
#include <Windows.h>

/**
 *	------------------------------------------------------------
 *		DirectX���C�u�����̒ǉ�
 *	------------------------------------------------------------
 */
/// DirectXGUID
#pragma comment(lib, "dxguid.lib")

/// DirectInput
#pragma comment(lib, "dinput8.lib")

/**
 *	------------------------------------------------------------
 *		DirectX�}�N��
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�����[�X���ɂ����s
 *	----------------------------------------
 */
/**
 *	DirectX�̐���
 *
 *	�����[�X���ɂ����s�����
 *	����
 *		dxStatus DirectX�̃X�e�[�^�X�B�������Ă����true��Ԃ�
 */
#define DX_SUCCEEDED(dxStatus) ( (dxStatus) >= 0 )

/**
 * 	DirectX�̎��s
 *
 * 	�����[�X���ɂ����s�����
 *	����
 *		dxStatus DirectX�̃X�e�[�^�X�B���s���Ă����true��Ԃ�
 */
#define DX_FAILED(dxStatus) ( (dxStatus) < 0 )

/**
 *	----------------------------------------
 *	�f�o�b�O���̂ݎ��s
 *	----------------------------------------
 */
#ifdef _DEBUG
/**
 *	DirectX�̃A�T�[�g
 *
 *	�����[�X���ɂ͎��s����Ȃ�
 *	����
 *		expression ���s���ƃA�T�[�g������
 */
#define DX_ASSERT(expression) \
	if ( DX_FAILED(expression) ) {\
		debugPrint(TEXT("DirectX Assertion in %s (%d)\n"), __FILE__, __LINE__);\
		DebugBreak();\
	}

/**
 *	DirectX�̃��b�Z�[�W�t�A�T�[�g
 *
 *	�����[�X���ɂ͎��s����܂���
 *	����
 *		expression ���s���ƃA�T�[�g����������
 *		message �A�T�[�g���ɏo�͂��郁�b�Z�[�W
 */
#define DX_MSG_ASSERT(expression, message) \
	if ( DX_FAILED(expression) ) {\
		debugPrint(TEXT("DirectX Assertion %s\n in %s (%d)\n"), message, __FILE__, __LINE__);\
		DebugBreak();\
	}

/**
 *	DirectX�̃`�F�b�N
 *
 *	�����[�X���Ƀ`�F�b�N�͎��s����Ȃ����A�����̍\���͎��s�����
 *	����
 *		expression ���s���ƃA�T�[�g����������
 */
#define DX_CHECK(expression) DX_ASSERT(expression)

/**
 *	DirectX�̃��b�Z�[�W�t�`�F�b�N
 *
 *	�����[�X���Ƀ`�F�b�N�͎��s����Ȃ����A�����̍\���͎��s�����
 *	����
 *		expression ���s���ƃA�T�[�g����������
 *		message �A�T�[�g���ɏo�͂��郁�b�Z�[�W
 */
#define DX_MSG_CHECK(expression, message) \
	DX_MSG_ASSERT(expression, message)

/**
 *	----------------------------------------
 *	�f�o�b�O���}�N���̃����[�X���̃_�~�[
 *	----------------------------------------
 */
#else /// _DEBUG

#define DX_ASSERT(expression)
#define DX_MSG_ASSERT(expression, message)
#define DX_CHECK(expression)
#define DX_MSG_CHECK(expression, message)

#endif	/// End of _DEBUG

#endif	/// __BasicDirectX_h__