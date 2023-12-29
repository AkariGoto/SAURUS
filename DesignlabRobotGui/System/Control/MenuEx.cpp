/**
 *  �t�@�C����
 *		MenuEx.cpp
 *  ����
 *		�g�����j���[�iCMenu�̊g���j
 *		 CMenuEX�̃C���^�[�t�F�C�X
 *  ���t
 *		�쐬��: 2007/06/11(MON)		�X�V��: 2007/06/18(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		�w�b�_�t�@�C���C���N���[�h
 *	----------------------------------------------------------------------
 */
#include "..\..\stdafx.h"
#include "MenuEx.h"
#include "..\ViewSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 *	----------------------------------------------------------------------
 *		�萔�錾
 *	----------------------------------------------------------------------
 */
/// �A�C�e���̃T�C�Y�E�]��
const int CX_LSP			= 20;	/// ���̗]���i�A�C�R���`�敔���j
const int CX_RSP			= 16;	/// �E�̗]���i���j���[������ƃV���[�g�J�b�g������̊Ԃ̗]���j
const int CX_STRSP		= 4;	/// ������̗]��
const int CY_ITEM		= 20;	/// �A�C�e���̍���
const int CX_ITEMSP		= ( CX_LSP + CX_STRSP );	/// �A�C�e���̕�����ȊO�̗]���S�̂̃T�C�Y

const int ICON_SIZE		= 16;	/// �A�C�R���̃T�C�Y�i���E�����j
const int X_ICON			= ( ( CX_LSP - ICON_SIZE ) / 2 );
const int Y_ICON			= ( ( CY_ITEM - ICON_SIZE ) / 2 );

/**
 *	----------------------------------------------------------------------
 *		CMenuEx�N���X
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		CMenuEx�N���X�̃����o�֐���`
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	�R���X�g���N�^�ƃf�X�g���N�^
 *	----------------------------------------
 */
/// �R���X�g���N�^
CMenuEx::CMenuEx()
{
	m_cxMaxStr = 0;
	m_cxMaxAccelerator = 0;
}


/// �f�X�g���N�^
CMenuEx::~CMenuEx()
{
	DestroyMenu();
}

/**
 *		LoadMenu: ���j���[���\�[�X�̃��[�h
 *			����: 
 *					lpszResourceName: ���\�[�X������������ւ̃|�C���^
 *					nIDResource: ���\�[�X��ID
 *					pMenuBmps: ���j���[�Ɋ֘A�t����C�R�}���hID�ƃr�b�g�}�b�v�������\���̂̔z��
 *									�i�K�v�Ȃ��ꍇ��NULL)
 *					nBmps: pMenuBmp�̔z�񐔁ipMenuBmp��NULL�̏ꍇ�͖����j
 *
 *			 �߂�l: ����I���Ȃ�0�ȊO��Ԃ��C����ȊO��0��Ԃ�
 */
BOOL CMenuEx::LoadMenu( LPCTSTR lpszResourceName, MENUEXBITMAP* pMenuBmps/*=NULL*/, int nBmps/*=0*/ )
{
	ASSERT( lpszResourceName );

	if ( ! CMenu::LoadMenu( lpszResourceName ) )
	{
		return FALSE;
	}

	ToOwnerDraw( this, pMenuBmps, nBmps );
	if ( pMenuBmps )
	{
	#ifdef _DEBUG
		BOOL bRet = 
	#endif

		SetItemBitmaps( pMenuBmps, nBmps );

	#ifdef _DEBUG
		ASSERT( bRet );
	#endif
	}

	return TRUE;
}

BOOL CMenuEx::LoadMenu( UINT nIDResource, MENUEXBITMAP* pMenuBmps/*=NULL*/, int nBmps/*=0*/ )
{
	return LoadMenu( MAKEINTRESOURCE( nIDResource ), pMenuBmps, nBmps );
}

/**
 *		DestroyMenu: ���j���[�̔j��
 *			����:
 *				pMenu: �j�����郁�j���[�ւ̃|�C���^
 *						�iCMenuEx�N���X�����ȊO�����NULL�ɂ���j
 *			�߂�l: ����I���Ȃ�0�ȊO��Ԃ��C����ȊO��0��Ԃ�
 */
BOOL CMenuEx::DestroyMenu( CMenuEx* pMenu/*=NULL*/ )
{
	BOOL bCallOrgDestroyMenu = FALSE;

	if ( ! pMenu )
	{
		if ( ! m_hMenu )
		{
			return FALSE;
		}
		pMenu = this;
		bCallOrgDestroyMenu = TRUE;
	}

	ASSERT( pMenu->m_hMenu );

	UINT i = 0;

	while ( true )
	{
		/// ���j���[�̃A�C�e�����擾
		MENUITEMINFO miInfo;
		::ZeroMemory( &miInfo, sizeof(MENUITEMINFO) );
		
		miInfo.cbSize = sizeof (MENUITEMINFO);
		miInfo.fMask = MIIM_TYPE | MIIM_SUBMENU | MIIM_DATA;
		if ( ! ::GetMenuItemInfo( pMenu->m_hMenu, i++, TRUE, &miInfo ) )
		{
			break;
		}

		/// �Z�p���[�^�ȊO�ɑ΂��ď�������
		if ( ! ( miInfo.fType & MFT_SEPARATOR ) )
		{
			MENUEXITEM* pMenuItem = (MENUEXITEM*)miInfo.dwItemData;

			ASSERT( pMenuItem );

			/// �T�u���j���[����������A�ċN�Ăяo�����Ĕj��
			if ( pMenuItem->pSubMenu )
			{
				DestroyMenu( pMenuItem->pSubMenu );

				pMenuItem->pSubMenu->Detach();
				delete pMenuItem->pSubMenu;
			}

			if ( pMenuItem->hBitmap )
			{
				::DeleteObject( pMenuItem->hBitmap );
			}

			delete pMenuItem;
		}
	}

	if ( bCallOrgDestroyMenu )
	{
		CMenu::DestroyMenu();
	}

	return TRUE;
}


/**
 *	------------------------------------------------------------
 *		�I�[�o�[���C�h�֐�
 *	------------------------------------------------------------
 */
/**
 *		DrawItem: �I�[�i�[�`��iCMenu�̉��z�֐��j
 *			����: pDis
 */
void CMenuEx::DrawItem( LPDRAWITEMSTRUCT pDis )
{
	if ( pDis->CtlType != ODT_MENU )
		return;

	/// ���j���[�̃A�C�e�����擾
	MENUITEMINFO miInfo;
	::ZeroMemory( &miInfo, sizeof(MENUITEMINFO) );
	
	miInfo.cbSize = sizeof (MENUITEMINFO);
	miInfo.fMask = MIIM_TYPE | MIIM_DATA;

	if ( ! ::GetMenuItemInfo( m_hMenu, pDis->itemID, FALSE, &miInfo ) )
	{
		ASSERT( FALSE );
		return;
	}

	CDC dc;
	dc.Attach( pDis->hDC );

	/// �w�i��h��Ԃ�
	/// ���j���[�A�C�R���̈�
	CRect r = pDis->rcItem;
	r.right = CX_LSP;
	dc.FillRect( &r, &CBrush( MENU_BGICON ) );

	/// ���j���[������̈�
	r.right = pDis->rcItem.right;
	r.left  = CX_LSP;
	dc.FillRect( &r, &CBrush( MENU_BACKGROUND ) );
	
	if ( ! ( miInfo.fType & MFT_SEPARATOR ) )
	{
		/// �I����`��
		DrawSelected( &dc, pDis->rcItem, pDis->itemState );

		/// �������`��
		MENUEXITEM* pMenuItem = (MENUEXITEM*)miInfo.dwItemData;
		if ( pMenuItem->sItemAccelerator.IsEmpty() )
		{
			DrawString( &dc, pMenuItem->sItemString, NULL, pDis->rcItem, pDis->itemState );
		}
		else
		{
			DrawString( &dc, pMenuItem->sItemString, pMenuItem->sItemAccelerator, pDis->rcItem, pDis->itemState );
		}
		
		// �`�F�b�N�}�[�N��`��
		DrawCheckMark( &dc, pDis->rcItem, pDis->itemState );

		// �r�b�g�}�b�v�`��
		if ( pMenuItem->hBitmap )
		{
			DrawBitmap( &dc, pMenuItem->hBitmap, pDis->rcItem, pDis->itemState );
		}
	}
	else
	{
		// �Z�p���[�^��`��
		DrawSeparator( &dc, pDis->rcItem );
	}

	dc.Detach();
}

/**
 *		MeasureItem: ���j���[���̐ݒ�iCMenu�̉��z�֐��j
 *			����: lpMeasureItemStruct
 */
void CMenuEx::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
	if ( lpMeasureItemStruct->CtlType != ODT_MENU )
	{
		return;
	}

	MENUEXITEM* pMenuItem = (MENUEXITEM*)lpMeasureItemStruct->itemData;
	if ( ! pMenuItem )
	{
		/// �Z�p���[�^
		lpMeasureItemStruct->itemWidth = 1;
		lpMeasureItemStruct->itemHeight= 1;
		return;
	}

	/// ���j���[�������\�����邽�߂̎l�p�`���v�Z����
	HDC hDC = ::GetDC( NULL );

	/// ���j���[�t�H���g�̎擾
	NONCLIENTMETRICS nm;
	nm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo( SPI_GETNONCLIENTMETRICS, nm.cbSize, &nm, 0 );
	HFONT hFont = ::CreateFontIndirect( &nm.lfMenuFont );
	HFONT hOldFont = (HFONT)::SelectObject( hDC, hFont );

	/// �������`�悷�邽�߂ɕK�v�Ȏl�p�`���擾����
	CRect rItemStr( 0, 0, 0, 0 );
	::DrawText( hDC, pMenuItem->sItemString, pMenuItem->sItemString.GetLength(), &rItemStr, DT_CALCRECT | DT_SINGLELINE | DT_EXPANDTABS );

	CRect rItemAcl( 0, 0, 0, 0 );
	if ( ! pMenuItem->sItemAccelerator.IsEmpty() )
	{
		::DrawText( hDC, pMenuItem->sItemAccelerator, pMenuItem->sItemAccelerator.GetLength(), &rItemAcl, DT_CALCRECT | DT_SINGLELINE | DT_EXPANDTABS );
	}

	::SelectObject( hDC, hOldFont );
	::DeleteObject( hFont );
	::ReleaseDC( NULL, hDC );

	// ���j���[�̕��ƍ�����ݒ�
	pMenuItem->pThisMenu->m_cxMaxStr = max( pMenuItem->pThisMenu->m_cxMaxStr, (UINT)rItemStr.Width() );
	if ( rItemAcl.Width() )
	{
		pMenuItem->pThisMenu->m_cxMaxAccelerator = max( pMenuItem->pThisMenu->m_cxMaxAccelerator, (UINT)rItemAcl.Width() + CX_RSP );
	}

	lpMeasureItemStruct->itemWidth = pMenuItem->pThisMenu->m_cxMaxStr + pMenuItem->pThisMenu->m_cxMaxAccelerator + CX_ITEMSP;
	lpMeasureItemStruct->itemHeight= max( CY_ITEM, rItemStr.Height() );
}

/**
 *	------------------------------------------------------------
 *		CMenuEx�N���X��private�ȃ����o�֐�
 *	------------------------------------------------------------
 */
/**
 *		SetItemBitmap: ���j���[�A�C�e���Ƀr�b�g�}�b�v���֘A�t����
 *			����: 
 *				pMenuBmps: ���j���[�Ɋ֘A�t����C�R�}���hID�ƃr�b�g�}�b�v�������\���̂̔z��
 *				nBmps: pMenuBmp�̔z��
 *			�߂�l: ����I���Ȃ�0�ȊO��Ԃ��C����ȊO��0��Ԃ�
 */
BOOL CMenuEx::SetItemBitmaps( MENUEXBITMAP* pMenuBmps, int nBmps )
{
	ASSERT( pMenuBmps );
	ASSERT( nBmps );

	for ( int i = 0; i < nBmps; i++ )
	{
		/// ���j���[�̃A�C�e�����擾
		MENUITEMINFO miInfo;
		::ZeroMemory( &miInfo, sizeof(MENUITEMINFO) );
		miInfo.cbSize = sizeof(MENUITEMINFO);
		miInfo.fMask = MIIM_DATA;

		if ( ::GetMenuItemInfo( m_hMenu, pMenuBmps[ i ].nID, FALSE, &miInfo ) )
		{
			MENUEXITEM* pMenuItem = (MENUEXITEM*)miInfo.dwItemData;
			ASSERT( pMenuItem );

			pMenuItem->hBitmap = (HBITMAP)::LoadImage( AfxGetInstanceHandle()
													 , MAKEINTRESOURCE( pMenuBmps[ i ].nIDBmpRes )
													 , IMAGE_BITMAP
													 , ICON_SIZE, ICON_SIZE
													 , LR_LOADMAP3DCOLORS );

			if ( ! pMenuItem->hBitmap )
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

/**
 *		ToOwnerDraw: ���j���[�S�Ă��I�[�i�[�`��ɂ���
 *			����:
 *				pMenu: CMenu�ւ̃|�C���^
 *				pMenuBmps: ���j���[�Ɋ֘A�t����C�R�}���hID�ƃr�b�g�}�b�v�������\���̂̔z��
 *								�i�K�v�Ȃ��ꍇ��NULL)
 *				nBmps: pMenuBmp�̔z�񐔁ipMenuBmp��NULL�̏ꍇ�͖����j
 */
void CMenuEx::ToOwnerDraw( CMenu* pMenu, MENUEXBITMAP* pMenuBmps, int nBmps )
{
	ASSERT( pMenu );
	ASSERT( pMenu->m_hMenu );

	UINT i = 0;

	while ( true )
	{
		/// ���j���[�̃A�C�e�����擾
		MENUITEMINFO miInfo;
		::ZeroMemory( &miInfo, sizeof(MENUITEMINFO) );
		miInfo.cbSize = sizeof (MENUITEMINFO);
		miInfo.fMask = MIIM_TYPE | MIIM_SUBMENU | MIIM_ID;

		TCHAR buf[ 256 ];
		miInfo.dwTypeData = buf;
		miInfo.cch = 256;
		if ( ! ::GetMenuItemInfo( pMenu->m_hMenu, i, TRUE, &miInfo ) )
		{
			break;
		}
		
		miInfo.fMask = ( pMenu == this ) ? 0 : MIIM_TYPE;

		/// ���j���[�̃A�C�e���f�[�^�ɕێ�����\���̂̏���
		MENUEXITEM* pMenuItem = NULL;
		if ( ! ( miInfo.fType & MFT_SEPARATOR ) )
		{
			pMenuItem = new MENUEXITEM;

			/// TAB�O�ƌ���킯��
			TCHAR* psz = _tcschr( buf, '\t' );
			if ( psz )
			{
				*psz = '\0';
				psz++;
				pMenuItem->sItemAccelerator = psz;
			}

			pMenuItem->pThisMenu	= (CMenuEx*)pMenu;
			pMenuItem->pSubMenu		= NULL;
			pMenuItem->sItemString	= buf;
			pMenuItem->hBitmap		= NULL;

			miInfo.fMask |= MIIM_DATA;
			miInfo.dwItemData = (ULONG_PTR)pMenuItem;
		}

		/// �T�u���j���[����������ACMenuEx��V��������ăA�^�b�`����
		if ( miInfo.hSubMenu )
		{
			ASSERT( pMenuItem );

			pMenuItem->pSubMenu = new CMenuEx;
			pMenuItem->pSubMenu->Attach( miInfo.hSubMenu );
			ToOwnerDraw( pMenuItem->pSubMenu, pMenuBmps, nBmps );

			if ( pMenuBmps )
			{
			#ifdef _DEBUG
				BOOL bRet = 
			#endif

				pMenuItem->pSubMenu->SetItemBitmaps( pMenuBmps, nBmps );

			#ifdef _DEBUG
				ASSERT( bRet );
			#endif
			}
		}

		/// �I�[�i�[�`��t���O���Z�b�g
		if ( pMenu != this )
		{
			miInfo.fType |= MFT_OWNERDRAW;
		}

		if ( ! ::SetMenuItemInfo( pMenu->m_hMenu, i, TRUE, &miInfo ) )
		{
			break;
		}
		
		i++;
	}
}

/**
 *		CreateGrayedBitmap: �����A�C�R����`�悷�邽�߂̃r�b�g�}�b�v�𐶐�
 *			 ����:
 *					pDC: CDC�ւ̃|�C���^
 *					width: ��
 *					height: ����
 *			�߂�l: ����I��������r�b�g�}�b�v�n���h���C����ȊO��NULL��Ԃ�
 *			���l: �r�b�g�}�b�v�� DeleteObject()�Ŕj�����Ȃ��Ă͂Ȃ�Ȃ�
 */
HBITMAP CMenuEx::CreateGrayedBitmap( CDC* pDC, int width, int height )
{
	ASSERT( pDC );
	ASSERT( pDC->GetSafeHdc() );

	/// �r�b�g�}�b�v��񐶐�
	DWORD dwSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 16;
	BITMAPINFO* pbmi = (BITMAPINFO*)new BYTE[ dwSize ];
	::ZeroMemory( pbmi, dwSize );

	pbmi->bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth			= width;
	pbmi->bmiHeader.biHeight			= height;
	pbmi->bmiHeader.biPlanes			= 1;
	pbmi->bmiHeader.biBitCount		= 4;
	pbmi->bmiHeader.biCompression	= BI_RGB;

	/// �p���b�g�̐ݒ�
	for ( int i = 0; i < 16; i++ )
	{
		COLORREF clr;
		switch ( i )
		{
			case 1:	clr = ::GetSysColor( COLOR_3DSHADOW );	break; 
			case 2:	clr = ::GetSysColor( COLOR_3DLIGHT );	break; 
			default:
				clr = ::GetSysColor( COLOR_3DFACE );
				break;
		}
		pbmi->bmiColors[ i ].rgbRed		= GetRValue( clr );
		pbmi->bmiColors[ i ].rgbGreen	= GetGValue( clr );
		pbmi->bmiColors[ i ].rgbBlue		= GetBValue( clr );
	}

	BYTE* pBits;
	HBITMAP hBitmap = ::CreateDIBSection( pDC->GetSafeHdc(), 
															pbmi, DIB_RGB_COLORS, (PVOID*)&pBits, 0, 0 );

	delete [] (BYTE*)pbmi;

	return hBitmap;
}

/**
 *		DrawSelected: �I����`��
 *			����: 
 *				pDC: �`�悷��CDC�ւ̃|�C���^
 *				rItem: ���j���[�A�C�e���̊O�ڎl�p�`
 *				itemState:  DRAWITEMSTRUCT�\���̂�itemState�Ɠ���
 */
void CMenuEx::DrawSelected( CDC* pDC, CRect rItem, UINT itemState )
{
	ASSERT( pDC );
	ASSERT( pDC->GetSafeHdc() );
	
	if ( ! ( itemState & ODS_SELECTED ) || ( itemState & ODS_GRAYED ) )
	{
		return;
	}
	
	/// �I���̕`��
	CPen	pen( PS_SOLID, 1, MENU_SELBORDER );
	CBrush	brush( MENU_SELECTED );

	CPen*	pOldPen		= pDC->SelectObject( &pen );
	CBrush* pOldBrush	= pDC->SelectObject( &brush );

	pDC->Rectangle( &rItem );

	pDC->SelectObject( pOldBrush );
	pDC->SelectObject( pOldPen );
}


/**
 *		DrawString: ���j���[�̕����񕔕���`��
 *			����:
 *				pDC: �`�悷��CDC�ւ̃|�C���^
 *				pszStr: �`�悷�镶����
 *				pszAcl: �`�悷��V���[�g�J�b�g������
 *				rItem: ���j���[�A�C�e���̊O�ڎl�p�`
 *				itemState: DRAWITEMSTRUCT�\���̂�itemState�Ɠ���
 */
void CMenuEx::DrawString( CDC* pDC, LPCTSTR pszStr, LPCTSTR pszAcl, CRect rItem, UINT itemState )
{
	ASSERT( pDC );
	ASSERT( pDC->GetSafeHdc() );

	/// �`��F�̐ݒ�
	COLORREF OldClr;
	if ( itemState & ODS_GRAYED )
	{ 
		OldClr = pDC->SetTextColor( MENU_GRAYED_STR );
	}
	else if ( itemState & ODS_SELECTED )
	{
		OldClr = pDC->SetTextColor( MENU_SELECTED_STR );
	}
	else
	{
		OldClr = pDC->SetTextColor( MENU_NORMAL_STR );
	}

	/// �w�i�𓧉�
	int OldBkMode = pDC->GetBkMode();
	pDC->SetBkMode( TRANSPARENT );

	/// �������`�悷��l�p�`���v�Z
	rItem.left	+= CX_LSP + CX_STRSP;
	rItem.right	= rItem.left + m_cxMaxStr;

	/// ������̕`��
	pDC->DrawText( pszStr
				 , _tcslen( pszStr )
				 , &rItem
				 , DT_SINGLELINE | DT_EXPANDTABS | DT_LEFT | DT_VCENTER );

	if ( pszAcl )
	{
		/// �V���[�g�J�b�g�������`�悷��l�p�`���v�Z
		rItem.left = rItem.right + CX_RSP;
		rItem.right= rItem.right + m_cxMaxAccelerator;

		/// �V���[�g�J�b�g������̕`��
		pDC->DrawText( pszAcl
					 , _tcslen( pszAcl )
					 , &rItem
					 , DT_SINGLELINE | DT_EXPANDTABS | DT_LEFT | DT_VCENTER );
	}

	pDC->SetBkMode( OldBkMode );
	pDC->SetTextColor( OldClr );
}

/**
 *		DrawCheckMark: �I�`�F�b�N�}�[�N��`��
 *			����:
 *				pDC: �`�悷��CDC�ւ̃|�C���^
 *				rItem: ���j���[�A�C�e���̊O�ڎl�p�`
 *				itemState: DRAWITEMSTRUCT�\���̂�itemState�Ɠ���
 */
void CMenuEx::DrawCheckMark( CDC* pDC, CRect rItem, UINT itemState )
{
	ASSERT( pDC );
	ASSERT( pDC->GetSafeHdc() );

	/// �`�F�b�N����Ė������̏���
	if ( ! ( itemState & ODS_CHECKED ) )
	{
		return;
	}
	
	/// �`�F�b�N��`�悷��r�b�g�}�b�v����
	HBITMAP hBitmap;
	if ( itemState & ODS_GRAYED )
	{
		hBitmap = CreateGrayedBitmap( pDC, ICON_SIZE, ICON_SIZE );
	}
	else
	{
		hBitmap = ::CreateCompatibleBitmap( pDC->GetSafeHdc(), ICON_SIZE, ICON_SIZE ); 
	}

	ASSERT( hBitmap );

	/// ������DC�̐���
	CDC memDC;
	memDC.CreateCompatibleDC( pDC );
	HBITMAP hOldBmp	= (HBITMAP)::SelectObject( memDC.GetSafeHdc(), hBitmap );


	/// GDI�I�u�W�F�N�g����
	CPen pen( PS_SOLID, 1, MENU_CHECKBORDER );
	CBrush brush( MENU_BGCHECK );

	/// GDI�I�u�W�F�N�g�I��
	CPen*	pOldPen		= memDC.SelectObject( &pen );
	CBrush* pOldBrush	= memDC.SelectObject( &brush );

	/// ������DC�Ƀ`�F�b�N��`��
	/// �`�F�b�N�{�b�N�X�̕`��
	memDC.Rectangle( CRect( 0, 0, ICON_SIZE, ICON_SIZE ) );

	/// �`�F�b�N�}�[�N
	POINT points[ 6 ] = { 
							{  5,  6 }
						  , {  7,  8 }
						  , { 10,  5 }
						  , { 10,  7 }
						  , {  7, 10 }
						  , {  5,  8 }
						};

	CBrush brushCheck( MENU_CHECKBORDER );
	memDC.SelectObject( &brushCheck );
	memDC.Polygon( points, 6 );


	/// �`�F�b�N��`�悷��l�p�`���v�Z
	rItem.left	 = X_ICON;
	rItem.top	 = rItem.top + Y_ICON;
	rItem.right	 = rItem.left + ICON_SIZE;
	rItem.bottom = rItem.top + ICON_SIZE;

	/// ������DC����pDC�ɓ]��
	pDC->BitBlt( rItem.left, rItem.top, rItem.Width(), rItem.Height(), &memDC, 0, 0, SRCCOPY );

	/// GDI�I�u�W�F�N�g�����ɖ߂�
	::SelectObject( memDC.GetSafeHdc(), hOldBmp );
	memDC.SelectObject( pOldBrush );
	memDC.SelectObject( pOldPen );

	::DeleteObject( hBitmap );
}

/**
 *		DrawSeparator: �Z�p���[�^��`��
 *			����: 
 *				pDC: �`�悷��CDC�ւ̃|�C���^
 *				rItem: ���j���[�A�C�e���̊O�ڎl�p�`
 */
void CMenuEx::DrawSeparator( CDC* pDC, CRect rItem )
{
	ASSERT( pDC );
	ASSERT( pDC->GetSafeHdc() );

	CPen pen( PS_SOLID, 1, MENU_SEPARATOR );
	CPen* pOldPen = pDC->SelectObject( &pen );

	pDC->MoveTo( rItem.left  + CX_LSP + CX_STRSP, rItem.top );
	pDC->LineTo( rItem.right - CX_STRSP, rItem.top );

	pDC->SelectObject( pOldPen );
}

/**
 *		DrawBitmap: �r�b�g�}�b�v��`��
 *			����: 
 *				pDC: �`�悷��CDC�ւ̃|�C���^
 *				hMeuBmp: �`�悷��r�b�g�}�b�v�̃n���h��
 *				rItem: ���j���[�A�C�e���̊O�ڎl�p�`
 *				itemState: DRAWITEMSTRUCT�\���̂�itemState�Ɠ���
 *
 */
void CMenuEx::DrawBitmap( CDC* pDC, HBITMAP hMenuBmp, CRect rItem, UINT itemState )
{
	ASSERT( pDC );
	ASSERT( pDC->GetSafeHdc() );
	ASSERT( hMenuBmp );

	// �r�b�g�}�b�v����
	HBITMAP hWorkBmp;
	if ( itemState & ODS_GRAYED )
	{
		hWorkBmp = CreateGrayedBitmap( pDC, ICON_SIZE, ICON_SIZE );
	}
	else
	{
		hWorkBmp = ::CreateCompatibleBitmap( pDC->GetSafeHdc(), ICON_SIZE, ICON_SIZE ); 
	}

	ASSERT( hWorkBmp );

	/// ������DC�̐���
	CDC memSrcDC;
	memSrcDC.CreateCompatibleDC( pDC );
	HBITMAP hOldSrcBmp = (HBITMAP)::SelectObject( memSrcDC.GetSafeHdc(), hMenuBmp );

	CDC memWorkDC;
	memWorkDC.CreateCompatibleDC( pDC );
	HBITMAP hOldWorkBmp	= (HBITMAP)::SelectObject( memWorkDC.GetSafeHdc(), hWorkBmp );
	
	/// �]���p�r�b�g�}�b�v��hMenuBmp�������r�b�g�}�b�v��]��
	memWorkDC.BitBlt( 0, 0, ICON_SIZE, ICON_SIZE, &memSrcDC, 0, 0, SRCCOPY );

	/// �r�b�g�}�b�v��`�悷��l�p�`���v�Z
	CRect rBmp = rItem;
	rBmp.left	= X_ICON;
	rBmp.top	= rBmp.top + Y_ICON;
	rBmp.right	= rBmp.left + ICON_SIZE;
	rBmp.bottom	= rBmp.top + ICON_SIZE;

	/// ���E���`��̂��߂Ɏl�p�`���Čv�Z
	rItem.left	= 0;
	rItem.right	= rItem.left + CX_LSP;
	rItem.bottom= rItem.top + CY_ITEM;

	/// ���E���̕`��
	if ( itemState & ODS_CHECKED )
	{
		pDC->DrawEdge( &rItem, EDGE_ETCHED, BF_RECT | BF_SOFT | BF_MIDDLE );
	}
	else if ( ( itemState & ODS_SELECTED ) && ! ( itemState & ODS_GRAYED ) )
	{
		pDC->DrawEdge( &rItem, EDGE_BUMP, BF_RECT | BF_SOFT | BF_MIDDLE );
	}

	/// ������DC����pDC�ɓ]��
	pDC->BitBlt( rBmp.left, rBmp.top, rBmp.Width(), rBmp.Height(), &memWorkDC, 0, 0, SRCCOPY );

	/// GDI�I�u�W�F�N�g�����ɖ߂�
	::SelectObject( memWorkDC.GetSafeHdc(), hOldWorkBmp );
	::SelectObject( memSrcDC.GetSafeHdc(), hOldSrcBmp );

	::DeleteObject( hWorkBmp );
}
