/**
 *  �t�@�C����
 *		ComLib.cpp
 *  ����
 *		COM���C�u�����N���X
 *  ���t
 *		�쐬��: 2008/03/10(MON)		�X�V��: 2008/03/10(MON)	
 */


/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
 #include "ComLib.h"
 
 
/**
 *	------------------------------------------------------------
 *		ComLib�N���X��static�ϐ��̏�����
 *	------------------------------------------------------------
 */
int ComLib::refCounter;

/**
 *	----------------------------------------------------------------------
 *		ComLib�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		ComLib�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */
/**
 *	COM���C�u����������
 */
bool ComLib::initialize(void)
{
	refCounter++;
	
	if (refCounter == 1)
	{
		if (::CoInitialize(NULL) != S_OK)
		{
			::OutputDebugString( TEXT("Error: [ComLib::initialize] Initialization failed\n") );
			refCounter = 0;
			
			return false;
		}
	}
	
	return true;	
}


/**
 *	COM���C�u�������
 */
void ComLib::uninitialize(void)
{
	if ( refCounter <= 0) return;

	refCounter--;
	
	if (refCounter == 0)
		::CoUninitialize();	
}
