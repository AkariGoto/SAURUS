/**
 *  ファイル名
 *		InputEffect.cpp
 *  説明
 *		フォースフィードバックのエフェクト効果
 *  日付
 *		作成日: 2008/03/10(MON)		更新日: 2008/03/10(MON)
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "InputEffect.h"

/**
 *	----------------------------------------------------------------------
 *		InputEffectクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		InputEffectクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
InputEffect::InputEffect(LPDIRECTINPUTEFFECT pDIEffect)
{
	DEBUG_ASSERT( pDIEffect != NULL );
	
	lpDirectInputEffect = pDIEffect;
	
	/// 参照カウンタインクリメント
	lpDirectInputEffect->AddRef();
}

/**
 *		デストラクタ
 */
InputEffect::~InputEffect()
{
	lpDirectInputEffect->Unload();
	lpDirectInputEffect->Release();
}

/**
 *		エフェクトの開始
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
 *		エフェクトが再生中かどうか調べる
 */
bool InputEffect::isPlaying(void)
{
	DWORD state;

	lpDirectInputEffect->GetEffectStatus(&state);

	if ( (state & DIEGES_PLAYING) == 0 )
		return false;

	return true;
}