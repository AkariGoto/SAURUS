/**
 *  �t�@�C����
 *		InputController.h
 *  ����
 *		DirectInput�ɂ����͂̏����N���X
 *  ���t
 *		�쐬��: 2008/02/05(MON)		�X�V��: 2008/02/19(MON)
 */

#ifndef __InputController_h__
#define __InputController_h__

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\..\..\Utility\ArrayList.h"

#include "..\..\BasicDirectX.h"
#include "JoyPad.h"
#include "JoyPadState.h"

/**
 *	----------------------------------------------------------------------
 *		InputController�N���X
 *			Singleton �p�^�[��
 *			�N���X�̃C���X�^���X��1������������Ȃ�
 *	----------------------------------------------------------------------
 */
class InputController
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
public:

private:
/**
 *	----------------------------------------
 *	DirectInput�֘A
 *	----------------------------------------
 */
	/**
	 *		DirectInput8�C���^�[�t�F�C�X
	 *		DirectInput8�I�u�W�F�N�g���Ǘ�����
	 */
	LPDIRECTINPUT8		lpDirectInput;
	/// DirectInput�Ɋ֘A�t����E�B���h�E�n���h��
	HWND					windowHandle;

/**
 *	----------------------------------------
 *	JoyPad�I�u�W�F�N�g
 *	----------------------------------------
 */
	/// JoyPad�{��
	ArrayList<InputDeviceInterface*>	joyPads;

	/// JoyPad���
	ArrayList<JoyPadState*>			joyPadStates;

	/// JoyPad�I�u�W�F�N�g�񋓈����\����
	struct DIENUMOBJARGS
	{
		InputController*	ptrInputController;
		LPDIRECTINPUTDEVICE8 ptrDIDevice;
	};

/**
 *	------------------------------------------------------------
 *		�����o�֐�
 *	------------------------------------------------------------
 */
protected:
/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
	/**
	 *	�f�t�H���g�R���X�g���N�^
	 *	�V���O���g����
	 */
	InputController();

public:
	/// �f�X�g���N�^
	virtual ~InputController();

/**
 *	����
 *		�C���X�^���X�̎擾�֐�
 */
	static InputController& getInstance(void)
	{
		static InputController theInput;
		return theInput;
	}

/**
 *	����
 *		������
 *		DirectInput������������InputController���g�p�\�ɂ���
 */
	bool initialize(const HWND hWnd);
	
/**
 *	����
 *		���\�[�X�̉��
 *		DirectInput��InputController���g�p���Ă������\�[�X
 */	
	void release(void);

/**
 *	----------------------------------------
 *	�f�o�C�X
 *	----------------------------------------
 */
	/**
	 *		�f�o�C�X�̃A�N�Z�X��
	 */
	/// �A�N�Z�X���̎擾
	virtual void acquire();
	/// �A�N�Z�X���̉��
	virtual void unacquire();

	/**
	 *		�f�o�C�X�̍X�V
	 */
	/// �f�o�C�X�o�b�t�@�̍X�V
	HRESULT updateDeviceData(void);
	/// �f�o�C�X��Ԃ̍X�V
	HRESULT updateDeviceState(void);

/**
 *	----------------------------------------
 *	JoyPad�֘A
 *	----------------------------------------
 */
	/**
	 *		JoyPad�f�o�C�X����
	 *		����
	 *			joyPadNum: ����������JoyPad�̐�
	 */
	DWORD createJoyPad(const int joyPadNum);

	/**
	 *		JoyPad�f�o�C�X�̉��
	 */
	void releaseJoyPad(void);

	/**
	 *		JoyPad�𐧌䂷�邽�߂̃C���^�[�t�F�C�X���擾
	 *		����
	 *			id: JoyPad��ID�i0�`�j
	 */
	InputDeviceInterface* getJoyPad(int id) const{return joyPads[id];}

	/**
	 *		JoyPad��Ԃ��擾
	 *		����
	 *			id: JoyPad��ID�i0�`�j
	 */
	JoyPadState*	getJoyPadState(int id) const{return joyPadStates[id];}

private:
/**
 *	�R�s�[�R���X�g���N�^�̋֎~
 */	
	InputController(const InputController& inputController);
 
/**
 *	������Z�q�̋֎~
 */
	InputController& operator =(const InputController& inputController);

/**
 *	�f�o�C�X�̉�����s���w���v�֐�
 */
	void releaseDevice(InputDeviceInterface* pDevice);

/**
 *	----------------------------------------
 *	JoyPad�֘A
 *	----------------------------------------
 */
	/// JoyPad������
	BOOL initializeJoyPad(const LPDIDEVICEINSTANCE pDevInst, LPVOID userData);

	/// JoyPad�I�u�W�F�N�g�ݒ�
	BOOL setJoyPadObjectProp(const LPDIDEVICEOBJECTINSTANCE pDevObjInst, LPVOID userData);

	/// JoyPad�̃o�b�t�@�X�V
	HRESULT updateJoyPadData(void);

	/// JoyPad��ԍX�V
	HRESULT updateJoyPadState(void);

/**
 *	----------------------------------------
 *	�R�[���o�b�N�֐�
 *	----------------------------------------
 */
	/**
	 *		JoyPad�񋓃R�[���o�b�N
	 */
	static BOOL CALLBACK enumJoyPadProcedure(LPDIDEVICEINSTANCE pDevInst, LPVOID userData);

	/**
	 *		JoyPad���񋓃R�[���o�b�N
	 */
	static BOOL CALLBACK enumJoyPadObjectProcedure(LPDIDEVICEOBJECTINSTANCE pDevObjInst, LPVOID userData);

 };

/**
 *	----------------------------------------
 *	inline�֐�
 *	----------------------------------------
 */
/**
 *	�C���X�^���X�̎擾�̏ȗ��Ăяo��
 */
inline InputController& diInputCtrl(void) { return InputController::getInstance(); }

/**
 *		JoyPad�擾�̏ȗ��Ăяo��
 *		����
 *			id: JoyPad��ID�i0�`�j
 */
inline InputDeviceInterface* diJoyPad(int id) { return diInputCtrl().getJoyPad(id); }

/**
 *		JoyPadState�擾�̏ȗ��Ăяo��
 *		����
 *			id: JoyPad��ID�i0�`�j
 */
inline JoyPadState* diJoyPadState(int id) { return diInputCtrl().getJoyPadState(id); }

#endif	/// __InputController_h__