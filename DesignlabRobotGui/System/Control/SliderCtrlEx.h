/**
 *  �t�@�C����
 *		CSliderCtrlEx.h
 *  ����
 *		�X���C�_�[�R���g���[���̊g���iCSliderCtrl�̊g���j
 *		 CMenuEX�̃C���^�[�t�F�C�X
 *  ���t
 *		�쐬��: 2007/06/06(WED)		�X�V��: 2007/06/11(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		CSliderCtrlEx�N���X
 *	----------------------------------------------------------------------
 */
class CSliderCtrlEx : public CSliderCtrl
{
	DECLARE_DYNAMIC(CSliderCtrlEx)

/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */
	/// �w�i�̃f�o�C�X�R���e�L�X�g�n���h��
	HDC			BkgndDCHandle;
	/// �w�i��BMP�n���h��
	HBITMAP	BkgndBMPHandle;
	/// �ȑO�̔w�i��BMP�n���h��
	HBITMAP	BkgndBMPHandleOld;
	/// �X���C�_�[�w�b�h�̐F
	COLORREF	ThumbColor;
	/// �I�΂ꂽ�X���C�_�[�o�[�̐F
	COLORREF	ThumbColorSelected;
	/// �ĕ`��̃t���O
	bool			isRedraw;
	
	/// �`�����l���̓h��Ԃ�
	HBRUSH channelBkColorBrush;


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
	CSliderCtrlEx(); // �W���R���X�g���N�^
	virtual ~CSliderCtrlEx();

	DECLARE_MESSAGE_MAP()

/**
 *	------------------------------------------------------------
 *		�������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
 *	------------------------------------------------------------
 */
protected:
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	/**
	 *		�X���C�_�̃o�[�����̐F����
	 */
	void SetThumbColors(COLORREF face, COLORREF highlight);
	/**
	 *		�g�����X�y�A�����g�ȕ`��
	 */
	void DrawTransparent(BOOL bRepaint);

private:
	/// �`�����l���̕`��
	void DrawChannel(CDC *pDC, LPNMCUSTOMDRAW lpcd);
	/// �X���C�_�o�[�̕`��
	void DrawThumb(CDC *pDC, LPNMCUSTOMDRAW lpcd);
};