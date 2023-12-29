/**
 *  �t�@�C����
 *		MultiMediaTimer.cpp
 *  ����
 *		�}���`���f�B�A�^�C�}���g�p�����^�C�}(Windows��p)
 *  ���t
 *		�쐬��: 2007/05/12(SAT)		�X�V��: 2007/05/19(SAT)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "MultiMediaTimer.h"
#include <iostream>
using namespace std;

namespace System
{
/**
 *	----------------------------------------------------------------------
 *		MultiMediaTimer�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		PlanDaMultiMediaTimerta�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
MultiMediaTimer::MultiMediaTimer(MultiMediaTimerCallback& timerCallback_) : timerCallback(timerCallback_)
{
	/// �^�C�}���\�Ɋ֌W
	timerPeriod						= 0;
	timerResolution					= 0;
	timerID							= 0;
	timerFrequency					= 0.0;
	performanceFrequency.HighPart	= 0;
	performanceFrequency.LowPart	= 0;

	/// �t���O
	isTimerEnded					= false;
	isTimerWarned					= false;

	/// �J�n����
	startTime						= 0.0;

	/// �����p�̃}�[�J
	marker = checkMMTimerMarker;

	/// 1�b������̃C���N�������g�l�𓾂�
	::QueryPerformanceFrequency(&performanceFrequency);
	/// double�l�ɕϊ�
	timerFrequency = (double)(performanceFrequency.HighPart)*(double)(0xffffffff) + (double)(performanceFrequency.LowPart);

}

/**
 *		�f�X�g���N�^
 */
MultiMediaTimer::~MultiMediaTimer()
{
    /// �^�C�}��j��
	killTimer();
	
	/// �����Ȓl�ɃZ�b�g
	marker = 0;
}

/**
 *	����
 *		�^�C�}�R�[���o�b�N�I�u�W�F�N�g�̃Z�b�g
 *		�^�C�}���J�n����O�ɕK���s��
 *	����
 *		�R�[���o�b�N�֐��ւ̃|�C���^
 */
bool MultiMediaTimer::setMultiMediaTimerCallback(MultiMediaTimerCallback& timerCallback_)
{
	timerCallback = timerCallback_;
	return true;
}

/**
 *	����
 *		�^�C�}�̃Z�b�g
 *	����
 *		period: ����
 *		resolution: �^�C�}�̕���\
 */
bool MultiMediaTimer::setTimer(UINT period, UINT resolution)
{
	/**
	 *		�����`�F�b�N
	 *			�w�肵���^�C�}������0�ȉ���������
	 *			�w�肵���^�C�}�������w�肵���^�C�}����\��菬����������
	 */
	if ( (period <= 0) || ( (period < resolution) && resolution != 0) )
	{
		cerr << "Error: [MultiMediaTimer::setTimer] Invalid argument\n" << endl;
		return false;
	}

	/// ���݂̃^�C�}���N���A
	killTimer();

	/// �x���̎��ʎq��������
	isTimerWarned = false;

	/// �ŏ��Ɏ��ʎq��������
	timerID = TIMERR_NOERROR;

	/// ���p�ł���^�C�}�̕���\
	TIMECAPS tc;

	/// �^�C�}�̕���\�𒲍�
	MMRESULT mmresult = ::timeGetDevCaps( &tc, sizeof(TIMECAPS) );

	if ( mmresult != TIMERR_NOERROR || mmresult == TIMERR_STRUCT )
	{
		/// �Ԃ�l��TIMERR_STRUCT�̂͂�
		timerID = mmresult;

		return false;
	}

	/// �ݒ肵���^�C�}���������������V�X�e���̍ŏ��̃^�C�}����\��菬������
	/// �ݒ肵���^�C�}���������������V�X�e���̍ő�̃^�C�}����\���傫����
	if ( (period < tc.wPeriodMin) || (tc.wPeriodMax < period) )
	{
		cerr << "Error: [MultiMediaTimer::setTimer] Invalid timer period\n" << endl;
		isTimerWarned = true;
	}

	/// �ݒ肵���^�C�}����\�����������V�X�e���̍ŏ��̃^�C�}����\��菬������
	/// �ݒ肵���^�C�}����\�����������V�X�e���̍ő�̃^�C�}����\���傫����
	if ( (resolution < tc.wPeriodMin) || (tc.wPeriodMax < resolution) )
	{
		cerr << "Error: [MultiMediaTimer::setTimer] Invalid timer resolution\n" << endl;
		isTimerWarned = true;
	}

	/// �^�C�}�̕���\�𗘗p�ł���ŏ��̕���\�ɃZ�b�g
	timerResolution = min( ( max(tc.wPeriodMin, resolution) ), tc.wPeriodMax );

	/// �ŏ��^�C�}����\��ݒ�
	mmresult = ::timeBeginPeriod(timerResolution);
	/// �ݒ�Ɏ��s������
	if ( mmresult != TIMERR_NOERROR || mmresult == TIMERR_NOCANDO )
	{

		timerResolution	= 0;
		timerID				= mmresult;

		return false;
	}

	/// �^�C�}�̎����𗘗p�ł���ŏ������ɃZ�b�g
	timerPeriod = min( ( max(tc.wPeriodMin, period) ), tc.wPeriodMax);

	/// �w�肵���f�[�^���g�p���ă^�C�}�C�x���g���J�n
	mmresult = ::timeSetEvent(	timerPeriod,							/// �^�C�}����
								timerResolution,						/// �^�C�}����\
								multiMediaTimerProcedure,				/// �R�[���o�b�N�֐��̃A�h���X
								(DWORD_PTR)this,						/// �R�[���o�b�N�f�[�^
								TIME_PERIODIC | TIME_CALLBACK_FUNCTION	/// �^�C�}�̎��
							);

	/// �^�C�}�C�x���g�̐ݒ�ɐ���������
	if ( mmresult != 0 )
	{
		timerCallback.onStart();
		timerID = mmresult;
	}
	/// ���s����
	else
	{
		/// �^�C�}����\�����Z�b�g
		resetPeriodAndResolution();
	
		timerID = TIMERR_NOCANDO;

		return false;
	}

	/// �I���t���O��OFF
	isTimerEnded = false;

	/// �J�n�������擾
	startTime = getPresentTime();

	return isActive();
}

/**
 *	����
 *		�^�C�}�̔j��
 */
bool MultiMediaTimer::killTimer()
{
	/// �N���e�B�J���Z�N�V�����ɂ�镔�����䃍�b�N
	CriticalSectionLock lock(criticalSection);

	if ( isActive() )
	{
		UINT timerID_ = timerID;
		timerID = 0;	/// �����I�ɒ�~

		/// �J�n�����^�C�}�C�x���g��j��
		::timeKillEvent(timerID_);

		/// �^�C�}�̃R�[���o�b�N�C���^�[�t�F�C�X�I�u�W�F�N�g�̏I������
		timerCallback.onStop();

		/// �t���O���N���A
		isTimerEnded = false;
	}
	else
		return false;

	/// �����ƕ���\�����Z�b�g
	resetPeriodAndResolution();

	return true;
}

/**
 *	����
 *		�^�C�}�̎����ƕ���\�̃��Z�b�g
 */
void MultiMediaTimer::resetPeriodAndResolution(void)
{
	if ( timerResolution != 0 )
	{
		/// �ŏ��^�C�}����\���N���A
		/// setTimer��timeBeginPeriod �֐��̌Ăяo����timeEndPeriod �֐��̌Ăяo������v
		::timeEndPeriod(timerResolution);
	}

	timerResolution=0;
	timerPeriod=0;

	return;
}

/**
 *	����
 *		���݂�CPU�����̎擾
 */
double MultiMediaTimer::getPresentTime(void)
{
	LARGE_INTEGER currentTime;

	/// �V�X�e���N������̌o�ߎ��Ԃ��擾
	::QueryPerformanceCounter(&currentTime);

	/// double�l�ɕϊ�
	double tics = (double)(currentTime.HighPart)*(double)(0xffffffff) + (double)(currentTime.LowPart);

	return ( (tics / timerFrequency) );
}

/**
 *	------------------------------------------------------------
 *		MultiMediaTimer�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/**
 *	����
 *		�^�C�}�v���V�[�W���̃R�[���o�b�N�֐�(static�Ȃ̂̓R�[���o�b�N�֐��̂���)
 *	����
 *		uID			: �^�C�}ID
 *		uMsg		: �s�g�p
 *		dwUser		: ���[�U��`(���this�|�C���^������)
 *		dw1			: �s�g�p
 *		dw2			: �s�g�p
 */
void CALLBACK MultiMediaTimer::multiMediaTimerProcedure(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	MultiMediaTimer* timerPointer = (MultiMediaTimer*)dwUser;

	bool isValidTimer = ( ( timerPointer != NULL ) && ( timerPointer->checkMarker() ) );

	/// �L���ȃ}���`���f�B�A�^�C�}�̃I�u�W�F�N�g�łȂ�����Ă΂Ȃ�
	if ( isValidTimer )		/// �L����������
		timerPointer->run();
}

/**
 *	����
 *		�^�C�}�̃R�[���o�b�N�֐��ŌĂяo������
 */
void MultiMediaTimer::run(void)
{
	/// ����1��O�̌Ăяo���ŏI��点��悤�Ɏw�����ꂽ��
	if ( isTimerEnded )
	{
		killTimer();
	}

	/**
	 *		���̊֐����ĂԑO��isActive()��true���ǂ����m�F
	 *		�I�[�o�[���C�h���ꂽOnStart()��1������葽������������false�ɂȂ�\��������
	 *		OnStart()�͍ŏ��ɂ��̊֐����Ă΂��O�Ɋ������Ă���K�v������
	 */
	if ( !isTimerEnded && isActive() )
	{
		/// onMultiMediaTimer������������isTimerEnded��false�̂͂�
		isTimerEnded = !timerCallback.onMultiMediaTimer();
	}

	return;
}

}	/// end of namespace System