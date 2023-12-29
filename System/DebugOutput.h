/**
 *  �t�@�C����
 *		DebugOutput.h
 *  ����
 *		Windows�ł̃f�o�b�O�p�c�[��
 *  ���t
 *		�쐬��: 2007/05/19(SAT)		�X�V��: 2007/05/26(SAT)
 */

#ifndef __DebugOutput_h__
#define __DebugOutput_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <windows.h>
#include <winnt.h>
#include <tchar.h>
#include <stdarg.h>


/**
 *	----------------------------------------------------------------------
 *		�萔�A�}�N���錾
 *	----------------------------------------------------------------------
 */
/// Visual Studio �̃f�o�b�O�E�B���h�E�̉����ɑΉ����镶����(�o�C�g���ł͂Ȃ�)
const int DEBUG_MESSAGE_LENGTH = 256;

#ifdef _DEBUG

/**
 *	�f�o�b�O���[�h�̎��̂ݏo�͂���iMFC��TRACE�}�N���Ɠ��l�j
 */
inline void debugPrint(const _TCHAR* fmt, ...)
{
	/// ���[�J���ϐ�
    va_list ap;
    TCHAR buffer[DEBUG_MESSAGE_LENGTH];

	/// �ϒ�������R�s�[
    va_start(ap, fmt);
		_vsntprintf(buffer, DEBUG_MESSAGE_LENGTH, fmt, ap);
	va_end(ap);
    
	/// �f�o�b�O�E�B���h�E�ɏo��
	::OutputDebugString(buffer);
}

#define DEBUG_TRACE ::debugPrint
#define DEBUG_TRACE_EX ::debugPrint("%s (%d):", __FILE__, __LINE__), ::debugPrint
#define DEBUG_ASSERT(x) \
			if ( !(x) )	{\
				debugPrint(TEXT("Assertion failed! in %s (%d)\n"), __FILE__, __LINE__);\
				DebugBreak();\
			}
#else
	#define DEBUG_TRACE		__noop
	#define DEBUG_ASSERT	__noop
#endif	/// _DEBUG

#endif	/// __DebugOutput_h__