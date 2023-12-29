/**
 *  �t�@�C����
 *		JoyPadState.h
 *  ����
 *		�W���C�p�b�h�̏�Ԃ�ێ�����N���X
 *  ���t
 *		�쐬��: 2008/03/10(MON)		�X�V��: 2008/03/10(MON)
 */

#ifndef __JoyPadState_h__
#define __JoyPadState_h__


/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\..\BasicDirectX.h"
#include "JoyPadConst.h"

/**
 *	----------------------------------------------------------------------
 *		JoyPadState�N���X
 *	----------------------------------------------------------------------
 */
class JoyPadState
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:

private:
/**
 *		���̒l
 */
 	/// ���i
	int AxisTransX;
	int AxisTransY;
	int AxisTransZ;
	
	/// ��]
	int AxisRotX;
	int AxisRotY;
	int AxisRotZ;

/**
 *	�{�^��
 */
	bool buttons[DIJOY_MAX_BUTTON_NUM];


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
	JoyPadState();
	virtual ~JoyPadState();

/**
 *		�����o�ϐ��̃N���A
 */
	virtual void clear(void);

/**
 *	----------------------------------------
 *	�Z�b�g�֐�
 *	----------------------------------------
 */
 	/**
 	 *		���̕��i�l
 	 */
	virtual void setAxisTransX(int xAxis) { AxisTransX = xAxis; }
	virtual void setAxisTransY(int yAxis) { AxisTransY = yAxis; }
	virtual void setAxisTransZ(int zAxis) { AxisTransZ = zAxis; }	

 	/**
 	 *		���̉�]�l
 	 */
	virtual void setAxisRotX(int xAxis) { AxisRotX = xAxis; }
	virtual void setAxisRotY(int yAxis) { AxisRotY = yAxis; }
	virtual void setAxisRotZ(int zAxis) { AxisRotZ = zAxis; }

	/**
	 *		�{�^��
	 *		����
	 *			id: �Ώۃ{�^��ID
	 *		�߂�l
	 *			�{�^����������Ă����true
	 */
	virtual void setButtonPressed(int id, bool pressed)
	{
		DEBUG_ASSERT( (0 <= id ) && (id < DIJOY_MAX_BUTTON_NUM) );
		
		buttons[id] = pressed;
	}

/**
 *	----------------------------------------
 *	�A�N�Z�X�֐�
 *	----------------------------------------
 */
 	/**
 	 *		���̕��i�l
 	 */
	virtual int getAxisTransX(void) { return AxisTransX; }
	virtual int getAxisTransY(void) { return AxisTransY; }
	virtual int getAxisTransZ(void) { return AxisTransZ; }

 	/**
 	 *		���̕��i�l
 	 */
	virtual int getAxisRotX(void) { return AxisRotX; }
	virtual int getAxisRotY(void) { return AxisRotY; }
	virtual int getAxisRotZ(void) { return AxisRotZ; }

	/**
	 *		�{�^����������Ă��邩
	 *		����
	 *			id �Ώۃ{�^��ID
	 *		�߂�l
	 *			�{�^����������Ă����true
	 */
	virtual bool isButtonPressed(int id) const
	{
		DEBUG_ASSERT( (0 <= id ) && (id < DIJOY_MAX_BUTTON_NUM) );
		return buttons[id];
	}


private:

};

#endif	/// __JoyPadState_h__