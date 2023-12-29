/**
 *  �t�@�C����
 *		ViewSetting.h
 *  ����
 *		View�N���X�̐ݒ�
 *  ���t
 *		�쐬��: 2007/06/06(WED)		�X�V��: 2007/06/11(MON)
 */

//  20200819  SCREEN_NUMBER�ύX

#ifndef __ViewSetting_h__
#define __ViewSetting_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\Utility\ColorNames.h"

/**
 *	----------------------------------------------------------------------
 *		�萔�錾
 *	----------------------------------------------------------------------
 */
/// �`�悷��X�N���[����
//const int SCREEN_NUMBER = 4;
const int SCREEN_NUMBER = 1;  //  20200819  �ύX

/// FormView�̔w�i
const COLORREF DLG_BACK_GREEN_COLOR		= RGB(32, 192, 208);
const COLORREF DLG_BACK_BLUE_COLOR		= RGB(64, 128, 192);

/// FormView��edit�w�i
const COLORREF EDIT_BACK_GREEN_COLOR	= RGB(32, 128, 160);
const COLORREF EDIT_BACK_BLUE_COLOR		= RGB(32, 48, 112);

/// static�e�L�X�g�F
const COLORREF TXT_YELLOW_COLOR_DARK	= RGB(192, 192, 0);
const COLORREF TXT_YELLOW_COLOR_LIGHT	= RGB(250, 240, 64);

/// edit�e�L�X�g�F
const COLORREF TXT_BLUE_COLOR_DARK		= RGB(32, 255, 255);
const COLORREF TXT_BLUE_COLOR_LIGHT		= RGB(64, 255, 255);


/// �{�^���F
const COLORREF BTN_COLOR1				= RGB(0, 64, 128);
const COLORREF SELECTED_BTN_COLOR1		= RGB(255, 32, 160);

/// �{�^���̃L���v�V�����F
const COLORREF BTN_CAPTION_COLOR1		= RGB(255, 255, 255);

/// �X���C�_�[�̃`�����l���F
const COLORREF SLDR_CHANNEL_COLOR		= RGB(32, 128, 160);
/// �X���C�_�[��THUMB�F
const COLORREF SLDR_THUMB_COLOR			= RGB(64, 255, 255);
/// �X���C�_�[��THUMB�̃n�C���C�g�F
const COLORREF SLDR_THUMB_HILIGHT_COLOR	= RGB(250, 240, 64);


/**
 *		���j���[�F
 */
const COLORREF MENU_SELECTED			= RGB( 192, 255, 255 );		/// �I��
const COLORREF MENU_SELBORDER			= RGB(   0,  50, 128 );		/// �I���̋��E��
const COLORREF MENU_NORMAL_STR			= RGB(   0, 128,  64 );		/// �ʏ�̕�����
const COLORREF MENU_SELECTED_STR		= RGB(  32,  64, 255 );		/// �I����Ԃ̕�����
const COLORREF MENU_GRAYED_STR			= RGB( 140, 140, 140 );		/// �����̕�����
const COLORREF MENU_SEPARATOR			= RGB( 128, 128, 128 );		/// �Z�p���[�^
const COLORREF MENU_BACKGROUND			= RGB( 224, 255, 240 );		/// �w�i
const COLORREF MENU_BGICON				= RGB( 192, 240, 192 );		/// �A�C�R���̔w�i
const COLORREF MENU_BGCHECK				= RGB( 224, 224, 255 );		/// �`�F�b�N�̔w�i
const COLORREF MENU_CHECKBORDER			= RGB(  64, 160, 255 );		/// �`�F�b�N�̋��E��

/**
 *		�񋓎q
 */
typedef enum
{
	GREEN_THEMED,
	BLUE_THEMED,
}ViewStyle;

#endif	/// __ViewSetting_h__