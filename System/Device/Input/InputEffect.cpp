/**
 *  �t�@�C����
 *		InputEffect.cpp
 *  ����
 *		�t�H�[�X�t�B�[�h�o�b�N�̃G�t�F�N�g����
 *  ���t
 *		�쐬��: 2008/03/10(MON)		�X�V��: 2008/03/10(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "InputEffect.h"

/**
 *	----------------------------------------------------------------------
 *		InputEffect�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		InputEffect�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
InputEffect::InputEffect(LPDIRECTINPUTEFFECT pDIEffect)
{
	DEBUG_ASSERT( pDIEffect != NULL );
	
	lpDirectInputEffect = pDIEffect;
	
	/// �Q�ƃJ�E���^�C���N�������g
	lpDirectInputEffect->AddRef();
}

/**
 *		�f�X�g���N�^
 */
InputEffect::~InputEffect()
{
	lpDirectInputEffect->Unload();
	lpDirectInputEffect->Release();
}

/**
 *		�G�t�F�N�g�̊J�n
 */
bool InputEffect::start(const DWORD iterations)
{
	HRESULT hResult;

	hResult = lpDirectInputEffect->Start(iterations, DIES_SOLO);
	if ( hResult != DI_OK )
	{
		DEBUG_TRACE( TEXT("Error: [InputEffect::start] DI8 Start() failed\n") );
		return false;
	}

	return true;
}

/**
 *		�G�t�F�N�g���Đ������ǂ������ׂ�
 */
bool InputEffect::isPlaying(void)
{
	DWORD state;

	lpDirectInputEffect->GetEffectStatus(&state);

	if ( (state & DIEGES_PLAYING) == 0 )
		return false;

	return true;
}