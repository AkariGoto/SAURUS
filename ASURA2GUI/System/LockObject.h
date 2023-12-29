/**
 *  �t�@�C����
 *		LockObject.h
 *  ����
 *		���b�N�I�u�W�F�N�g��񋟂���N���X(Windows��p)
 *  ���t
 *		�쐬��: 2007/05/12(SAT)		�X�V��: 2007/05/19(SAT)
 */

#ifndef __LockObject_h__
#define __LockObject_h__

namespace System
{

/**
 *	----------------------------------------------------------------------
 *		LockObject�N���X
 *	----------------------------------------------------------------------
 */
class LockObject
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */

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
	LockObject() {};

	/// �f�X�g���N�^
	~LockObject() {};    

	/**
	 *	����
	 *		���b�N
	 */
	virtual void lock(void) = 0;

	/**
	 *	����
	 *		���b�N����
	 */
	virtual void unlock(void) = 0;

	/**
	 *	����
	 *		���s�X���b�h�ɂ�胍�b�N����Ă��邩
	 *	�ԋp�l
	 *		true: ���b�N����Ă���
	 */
	virtual bool isLockedByCurrentThread() const = 0;

private:
	/// �R�s�[�R���X�g���N�^�̋֎~
	LockObject(const LockObject& copy);

	/// ������Z�q�̋֎~
	LockObject& operator =(const LockObject& copy);

};	/// end of class LockObject

}	/// end of namespace System

#endif // __LockObject_h__