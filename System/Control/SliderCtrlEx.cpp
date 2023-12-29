/**
 *  �t�@�C����
 *		CSliderCtrlEx.cpp
 *  ����
 *		�X���C�_�[�R���g���[���̊g���iCSliderCtrl�̊g���j
 *		 CMenuEX�̃C���^�[�t�F�C�X
 *  ���t
 *		�쐬��: 2007/06/06(WED)		�X�V��: 2007/06/11(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\..\stdafx.h"

#include "SliderCtrlEx.h"
#include "..\ViewSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 *	----------------------------------------------------------------------
 *		CSliderCtrlEx�N���X
 *	----------------------------------------------------------------------
 */
IMPLEMENT_DYNAMIC(CSliderCtrlEx, CSliderCtrl)

BEGIN_MESSAGE_MAP(CSliderCtrlEx, CSliderCtrl)

		ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
		ON_WM_ERASEBKGND()

END_MESSAGE_MAP()

/**
 *	------------------------------------------------------------
 *		CSliderCtrlEx�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �R���X�g���N�^
CSliderCtrlEx::CSliderCtrlEx()
{

	BkgndDCHandle = NULL;
	ThumbColor = NULL;
	ThumbColorSelected = NULL;
	isRedraw = false;
	channelBkColorBrush	= CreateSolidBrush( SLDR_CHANNEL_COLOR );
}

/// �f�X�g���N�^
CSliderCtrlEx::~CSliderCtrlEx()
{	
	/// �w�肳�ꂽ�f�o�C�X�R���e�L�X�g�̃I�u�W�F�N�g��I��
	::SelectObject(BkgndDCHandle, BkgndBMPHandleOld);
	
	/// �I�u�W�F�N�g�̍폜
	::DeleteObject(BkgndBMPHandle);
	
	/// �f�o�C�X�R���e�L�X�g�̍폜
	::DeleteDC(BkgndDCHandle);			
	
	/// �u���V�̍폜
	DeleteObject(channelBkColorBrush);
}

/**
 *	------------------------------------------------------------
 *		�������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
 *		CSliderCtrlEx�̃��b�Z�[�W�n���h��
 *	------------------------------------------------------------
 */
/**
 *		�X���C�_�[�R���g���[���ɂ�著����e�E�B���h�E�ɒʒm����`����
 *		���̒ʒm��WM_NOTIFY���b�Z�[�W
 *		�����F
 *			pNMHDR:  NM_CUSTOMDRAW�\���̂ւ̃|�C���^
 *			pResult: ���݂̕`���Ԃɂ��l
 */
void CSliderCtrlEx::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	/// LPNMCUSTOMDRAW�ɃL���X�g
	LPNMCUSTOMDRAW lpcd = (LPNMCUSTOMDRAW)pNMHDR;
	
	/// HDC����CDC���쐬
	CDC *pDC = CDC::FromHandle(lpcd->hdc);

	/// ���݂̕`��X�e�[�W�ɂ�蓮�������
	/**
	 *		CDDS_POSTERASE �����T�C�N���������� 
	 *		CDDS_POSTPAINT �`��T�C�N�������� 
	 *		CDDS_PREERASE �����T�C�N�����n�܂�O 
	 *		CDDS_PREPAINT �`��T�C�N�����n�܂�O 
	 *		CDDS_ITEM dwItemSpec, uItemState, lItemlParam�����o���L�� 
	 *		CDDS_ITEMPOSTERASE �A�C�e�����������ꂽ�� 
	 *		CDDS_ITEMPOSTPAINT �A�C�e�����`�悳�ꂽ�� 
	 *		CDDS_ITEMPREERASE �A�C�e�������������O 
	 *		CDDS_ITEMPREPAINT �A�C�e�����`�悳���O 
	 *		CDDS_SUBITEM Ver.4.71�ȍ~�ŗL��
	 */
	switch (lpcd->dwDrawStage)
	{
		case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
			break;
		}
		case CDDS_ITEMPREPAINT:
		{
			/// �A�C�e���ԍ��ɂ�蓮�������
			switch(lpcd->dwItemSpec)
			{
				case TBCD_TICS:	/// �X���C�_�̖ڐ�
				{
					*pResult = CDRF_DODEFAULT;
					break;
				}
				case TBCD_THUMB:	/// �X���C�_�̃o�[
				{
					DrawThumb(pDC, lpcd);
					*pResult = CDRF_SKIPDEFAULT;
					break;
				}
				case TBCD_CHANNEL:	/// �X���C�_�̃`�����l��
				{
					DrawChannel(pDC, lpcd);
					*pResult = CDRF_SKIPDEFAULT;
					break;
				}
			}
			default:
				break;
		}	/// end of case CDDS_ITEMPREPAINT:
	}	/// end of switch (lpcd->dwDrawStage)

	return;
}

/**
 *		�w�i�̏���
 */
BOOL CSliderCtrlEx::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

/**
 *		�X���C�_�̃w�b�h�����̐F����
 */
void CSliderCtrlEx::SetThumbColors(COLORREF face, COLORREF highlight)
{
	ThumbColor = face;
	ThumbColorSelected = highlight;
}

/**
 *		���ߓI�ȁi�g�����X�y�A�����g�j�ȕ`��
 */
void CSliderCtrlEx::DrawTransparent(BOOL bRepaint)
{
	/// HDC��HBMP���L�����ǂ����̃`�F�b�N
	if ( BkgndDCHandle != NULL && BkgndBMPHandleOld != NULL )
	{
		::SelectObject(BkgndDCHandle, BkgndBMPHandleOld);
	}

	/// HBMP��HDC�̍폜
	::DeleteObject(BkgndBMPHandle);
	::DeleteDC(BkgndDCHandle);

	BkgndDCHandle = NULL;
	BkgndBMPHandle = NULL;

	if (bRepaint == TRUE) 
	{
		Invalidate();
		EnableWindow(FALSE);
		EnableWindow(TRUE);
	}
}

/**
 *	------------------------------------------------------------
 *		CSliderCtrlEx�N���X��private�ȃ����o�֐�
 *		OnCustomDraw�ŌĂ΂��w���v�֐�
 *	------------------------------------------------------------
 */
/**
 *		�`�����l���̕`��
 */
void CSliderCtrlEx::DrawChannel(CDC *pDC, LPNMCUSTOMDRAW lpcd)
{
	/// DC�̎擾
	/// GetParent(): �w�肳�ꂽ�E�B���h�E�̐e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h�����擾
	CClientDC clientDC( GetParent() );


	CRect crect;
	CRect wrect;

	/// �E�B���h�E�̃N���C�A���g�̈�̍��W���擾
	GetClientRect(crect);
	/// �E�B���h�E�̍��W���X�N���[�����W�n�Ŏ擾
	GetWindowRect(wrect);
	/// �X�N���[����̓_�̍��W���X�N���[�����W����w�肳�ꂽ�E�B���h�E�̃N���C�A���g���W�ɕϊ�
	GetParent()->ScreenToClient(wrect);

	/// HDC��������������
	if (BkgndDCHandle == NULL)
	{
		BkgndDCHandle = CreateCompatibleDC(clientDC.m_hDC);
		BkgndBMPHandle = CreateCompatibleBitmap(clientDC.m_hDC, crect.Width(), crect.Height());
		BkgndBMPHandleOld = (HBITMAP)::SelectObject(BkgndDCHandle, BkgndBMPHandle);
		
		/// �摜�̃r�b�g�u���b�N�]�����s��
		::BitBlt(BkgndDCHandle, 0, 0, crect.Width(), crect.Height(), clientDC.m_hDC, wrect.left, wrect.top, SRCCOPY);
		/**
		 *		����ɂ��`�F�b�N�}�[�N�𓧉ߓI�ɍ쐬����
		 *		dc�̃��������쐬���`�F�b�N�}�[�N���܂ތÂ�bmp�f�[�^�̃R�s�[��ێ�����
		 *		����̓`�F�b�N�}�[�N�����ł��낤�w�i�Ƀ`�F�b�N�}�[�N�������鎞�ɔ����Ăł���
		 */
	}

	HDC hSaveHDC;
	HBITMAP hSaveBmp;

	int iWidth = crect.Width();
	int iHeight = crect.Height();
	
	hSaveHDC = ::CreateCompatibleDC(pDC->m_hDC);
	hSaveBmp = ::CreateCompatibleBitmap(hSaveHDC, iWidth, iHeight);
	
	HBITMAP hSaveCBmpOld = (HBITMAP)::SelectObject(hSaveHDC, hSaveBmp);			
	
	/// �P�F�}�X�Nbmp�̂��߂̐F��ݒ肷��
	COLORREF crOldBack = ::SetBkColor(pDC->m_hDC, RGB(0,0,0));
	COLORREF crOldText = ::SetTextColor(pDC->m_hDC, RGB(255,255,255));		
	
	/// �}�X�N���g���ăC���[�W��]������
	::BitBlt(hSaveHDC, 0, 0, iWidth, iHeight, pDC->m_hDC, crect.left, crect.top, SRCCOPY);
	::BitBlt(pDC->m_hDC, 0, 0, iWidth, iHeight, BkgndDCHandle, 0, 0, SRCCOPY);
	::BitBlt(pDC->m_hDC, 0, 0, iWidth, iHeight, hSaveHDC, 0, 0, SRCAND);
	
	/// �l�̕ێ�
	::SetBkColor(pDC->m_hDC, crOldBack);
	::SetTextColor(pDC->m_hDC, crOldText);
	::SelectObject(hSaveHDC, hSaveCBmpOld);
	
	/// �I�u�W�F�N�g�̍폜
	::DeleteObject(hSaveBmp);
	::DeleteDC(hSaveHDC);
	crect = lpcd->rc;
	
	if ( (crect.bottom - crect.top) > (crect.right - crect.left) )
		crect.InflateRect(10, 0, 10, 0);
	else
		crect.InflateRect(0, 10, 0, 10);

	/// ���E�̕`��
	DrawEdge(pDC->m_hDC, &crect, EDGE_BUMP, BF_MONO | BF_ADJUST);
	/// �`�����l���̓h��Ԃ�
	FillRect(pDC->m_hDC, crect, channelBkColorBrush);

}

/**
 *		�X���C�_�o�[�̕`��
 */
void CSliderCtrlEx::DrawThumb(CDC *pDC, LPNMCUSTOMDRAW lpcd)
{
	CRect crect;
	GetThumbRect(&crect);
	COLORREF col;

	if (lpcd->uItemState & CDIS_SELECTED)
		col = ThumbColorSelected;
	else
		col = ThumbColor;
	
	if (col == NULL && lpcd->uItemState & CDIS_SELECTED)
		col = GetSysColor(COLOR_3DHIGHLIGHT);
	else if (col == NULL && !(lpcd->uItemState & CDIS_SELECTED))
		col = GetSysColor(COLOR_3DFACE);
	
	/// �u���V�̍쐬�ƑI��
	HBRUSH hbrush = CreateSolidBrush(col);
    HBRUSH hbOld = (HBRUSH)SelectObject(pDC->m_hDC, hbrush);

	/// �_�C�A�����h�`��THUMB��`��
	int xx, yy, dx, dy, cx, cy;
	/// THUMB�̊O�`
	xx = lpcd->rc.left;
	yy = lpcd->rc.top;
	/// ����
	dx = 3;
	dy = 3;

	cx = lpcd->rc.right - xx - 1;
	cy = lpcd->rc.bottom - yy - 1;
	
	/// ���_��POINT�\���̂Ɋi�[
	POINT pts[8] = { {xx + dx, yy}, 
							{xx, yy + dy},
							{xx, yy + cy - dy}, 
							{xx + dx, yy + cy},
							{xx + cx - dx, yy + cy}, 
							{xx + cx, yy + cy - dy}, 
							{xx + cx, yy + dy}, 
							{xx + cx -dx, yy} };

	/// �|���S����THUMB�̕`��
	pDC->Polygon( pts, 8 );
   
	SelectObject(pDC->m_hDC, hbOld);
    
	DeleteObject(hbrush);
}
