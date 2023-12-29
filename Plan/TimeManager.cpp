/**
 *  �t�@�C����
 *		TimeManager.cpp
 *  ����
 *		�v��̂��߂̎��ԊǗ�������N���X
 *  ���t
 *		�쐬��: 2007/09/01(SAT)		�X�V��: 2008/10/04(SAT)
 */

#include "TimeManager.h"

using namespace std;

namespace Plan
{
/**
 *	----------------------------------------------------------------------
 *		TimeManager�N���X
 *	----------------------------------------------------------------------
 */
/**
 *	------------------------------------------------------------
 *		TimeManager�N���X��static�ϐ��̏�����
 *	------------------------------------------------------------
 */
double TimeManager::realTime = 0.0;

/**
 *	------------------------------------------------------------
 *		TimeManager�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �f�t�H���g�R���X�g���N�^
TimeManager::TimeManager()
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::setRealTime(double time)
{
	/// �����`�F�b�N
	if ( time<=0 )
	{
		cerr << "Error: [TimeManager::SetRealTime] Invalid argument\n" << endl;
		return;
	}

	realTime = time;
	return;

}

}	/// end of namespace Plan