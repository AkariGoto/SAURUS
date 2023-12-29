/**
 *  �t�@�C����
 *		MenuEx.h
 *  ����
 *		�g�����j���[�iCMenu�̊g���j
 *		 CMenuEX�̃C���^�[�t�F�C�X
 *  ���t
 *		�쐬��: 2007/06/11(MON)		�X�V��: 2007/06/18(MON)
 */

#pragma once

/**
 *	----------------------------------------------------------------------
 *		�N���X�̑O���錾
 *	----------------------------------------------------------------------
 */
class CMenuEx;

/**
 *	----------------------------------------------------------------------
 *		�}�N���A�^�錾
 *	----------------------------------------------------------------------
 */
/**
 *		MENUEXBITMAP �\����
 */
typedef struct tagMENUEXBITMAP
{
	UINT	nID;			/// �R�}���hID
	UINT	nIDBmpRes;	/// �r�b�g�}�b�v���\�[�XID
} MENUEXBITMAP;

/**
 *		WMMENUITEM �\����
 */
typedef struct tagMENUEXITEM
{
	CMenuEx*	pThisMenu;			/// MeasureItem �Ń|�b�v�A�b�v���T�u���j���[�̃n���h���ɂȂ��Ă��܂��̂Ŏ����ւ̃|�C���^�������Ă���
	CMenuEx*	pSubMenu;			/// �T�u���j���[�ւ̃|�C���^
	CString		sItemString;		/// �A�C�e���̕�����
	CString		sItemAccelerator;	/// �A�C�e���̃V���[�g�J�b�g������
	HBITMAP	hBitmap;				/// �A�C�R���p�̃r�b�g�}�b�v
} MENUEXITEM;

/**
 *	----------------------------------------------------------------------
 *		CMenuEx�N���X
 *	----------------------------------------------------------------------
 */
class CMenuEx : public CMenu
{
/**
 *	------------------------------------------------------------
 *		�����o�ϐ�
 *	------------------------------------------------------------
 */

////////// �A�g���r���[�g //////////
private:
	UINT		m_cxMaxStr;				/// ���j���[�̕��������̍ő啝
	UINT		m_cxMaxAccelerator;		/// ���j���[�̃V���[�g�J�b�g�L�[�����̍ő啝

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
	CMenuEx();
	virtual ~CMenuEx();

/**
 *	----------------------------------------
 *	���j���[�̃��[�h�^�j��
 *	----------------------------------------
 */
	/// ���j���[���\�[�X�̃��[�h
	BOOL LoadMenu( LPCTSTR lpszResourceName, MENUEXBITMAP* pMenuBmps=NULL, int nBmps=0 );
	/// ���j���[���\�[�X�̃��[�h
	BOOL LoadMenu( UINT nIDResource, MENUEXBITMAP* pMenuBmps=NULL, int nBmps=0 );
	/// ���j���[�j��
	virtual BOOL DestroyMenu( CMenuEx* pMenu=NULL );

protected:
/**
 *	------------------------------------------------------------
 *		�I�[�o�[���C�h�֐�
 *	------------------------------------------------------------
 */
	virtual void DrawItem( LPDRAWITEMSTRUCT pDis );
	virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );

private:
/**
 *	------------------------------------------------------------
 *		����֐�
 *	------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *		�I�y���[�V�����֐�
 *	----------------------------------------
 */
	/**
	 *		���j���[�A�C�e���Ƀr�b�g�}�b�v���֘A�t����
	 */
	BOOL SetItemBitmaps( MENUEXBITMAP* pMenuBmps, int nBmps );
	/**
	 *		���j���[�S�Ă��I�[�i�[�`��ɂ���
	 */
	void ToOwnerDraw( CMenu* pMenu, MENUEXBITMAP* pMenuBmp, int nBmps );

	/// �A�C�R��
	/**
	 *		�����A�C�R����`�悷�邽�߂̃r�b�g�}�b�v�𐶐�
	 */	
	HBITMAP CreateGrayedBitmap( CDC* pDC, int w, int h );

	// ���j���[�`��
	/**
	 *		�I����`��
	 */
	void DrawSelected( CDC* pDC, CRect rItem, UINT itemState );
	
	/**
	 *		���j���[�̕����񕔕���`��
	 */
	void DrawString( CDC* pDC, LPCTSTR pszStr, LPCTSTR pszAcl, CRect rItem, UINT itemState );
	
	/**
	 *		�`�F�b�N�}�[�N��`��
	 */
	void DrawCheckMark( CDC* pDC, CRect rItem, UINT itemState );
	
	/**
	 *		�Z�p���[�^��`��
	 */
	void DrawSeparator( CDC* pDC, CRect rItem );
	
	/**
	 *		�r�b�g�}�b�v��`��
	 */
	void DrawBitmap( CDC* pDC, HBITMAP hMenuBmp, CRect rItem, UINT itemState );
};
