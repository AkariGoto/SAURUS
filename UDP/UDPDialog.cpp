/**
 *  ファイル名
 *		UDPDialog.cpp
 *  説明
 *		UDP通信のログ表示のためのダイアログ
 *  日付
 *		作成日: 2007/06/11(Mon)		更新日: 2007/11/10(Sat)
 */

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "stdafx.h"

#include "UDPDialog.h"

#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Asura;
using namespace Comm;
using namespace Data;
using namespace System;
using namespace Const;
using namespace Plan;
//追加
using namespace UDP;