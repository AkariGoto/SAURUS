/**
 *  �t�@�C����
 *		CriticalSection.h
 *  ����
 *		�N���e�B�J���Z�N�V�������g������������(Windows��p)
 *  ���t
 *		�쐬��: 2007/05/12(SAT)		�X�V��: 2007/05/19(SAT)
 */

#ifndef __CriticalSection_h__
#define __CriticalSection_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include <cassert>
#include <iostream>
#include "LockObject.h"

namespace System
{

/**
 *	----------------------------------------------------------------------
 *		CriticalSection�N���X
 *		�iWindows Critical Section��wrapper�N���X�j
 *	----------------------------------------------------------------------
 */
class CriticalSection : public LockObject
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
protected:
/**
 *	�N���e�B�J���Z�N�V�����\����
 */
	CRITICAL_SECTION criticalSection;

/**
 *	���b�N������s�������̃J�E���^
 */
	int lockCount;

/**
 *	���b�N����X���b�hID
 */
	unsigned int threadID;

/**
 *		�����ȃX���b�hID�̋K��l
 */
	static const unsigned int invalidThreadID = 0xffffffff;

/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
	/// �f�t�H���g�R���X�g���N�^
	CriticalSection() : lockCount(0), threadID(invalidThreadID)
    {
		/// �N���e�B�J���Z�N�V�����I�u�W�F�N�g�̏�����
		::InitializeCriticalSection( &criticalSection );
	}

    ~CriticalSection()    
    {
		assert(threadID == invalidThreadID);
		assert(lockCount == 0);
		/// �N���e�B�J���Z�N�V�����I�u�W�F�N�g�̕���(�㏈��)
		::DeleteCriticalSection( &criticalSection );
	}

/**
 *	����
 *		���b�N
 */
	virtual void lock(void)
	{
		::EnterCriticalSection(&criticalSection);
		lockCount++;
		threadID = ::GetCurrentThreadId();
	}

/**
 *	����
 *		�A�����b�N
 */
	virtual void unlock(void)
	{
		assert(lockCount > 0);
		lockCount--;
		
		if ( lockCount == 0 )
			threadID = invalidThreadID;

		::LeaveCriticalSection(&criticalSection);
	}

/**
 *	����
 *		���s�X���b�h�ɂ�胍�b�N����Ă��邩
 *	����
 *		true: ���b�N����Ă���
 */
	virtual bool isLockedByCurrentThread() const
	{
		return ::GetCurrentThreadId() == threadID;
	}

};	/// end of class CriticalSection

/**
 *	----------------------------------------------------------------------
 *		CriticalSectionLock�N���X
 *	----------------------------------------------------------------------
 */
class CriticalSectionLock
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
/**
 *	CriticalSection�N���X�̃I�u�W�F�N�g
 */
    CriticalSection&		criticalSection;

/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
public:
	/**
	 *	�f�t�H���g�R���X�g���N�^
	 *	�������Ƀu���b�N����
	 */
	CriticalSectionLock(CriticalSection& cs)
	: criticalSection( cs )
	{
		criticalSection.lock();
	}

	/**
	 *	�f�X�g���N�^
	 *	�u���b�N����
	 */
    ~CriticalSectionLock()
	{
		criticalSection.unlock();
	}
};	/// end of class CriticalSectionLock

}	/// end of namespace System;

#endif // __CriticalSection_h__