/**
 *  ファイル名
 *		PlanData.cpp
 *  説明
 *		移動計画のデータ（時間、安定余裕など）
 *  日付
 *		作成日: 2007/02/12(MON)		更新日: 2007/02/19(MON)
 */

 //20200819  安定余裕関連コメントアウト

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "PlanData.h"
#include "..\Utility\EngConstant.h"
 

using namespace std;
using namespace Math;
using namespace Const;
using namespace Asura;
using namespace Plan;

namespace Data
{
/**
 *	----------------------------------------------------------------------
 *		PlanDataクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		PlanDataクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
PlanData::PlanData()
{
	//stabilityMargin = 0.0;  20200819
	elapsedTime = 0.0;
	crabAngle = 0.0;
}

/// コピーコンストラクタ
PlanData::PlanData(const PlanData& planData)
{
	copy(planData);
}

/// デストラクタ
PlanData::~PlanData()
{
}

/**
 *	----------------------------------------
 *	演算子
 *	----------------------------------------
 */
/// 代入演算子
PlanData& PlanData::operator=(const PlanData& planData)
{
	/// 自己代入のチェック
	if ( &planData != this )
	{
		copy(planData);
	}

	return *this;
}

/// 等価演算子
bool PlanData::operator==(const PlanData& planData)
{
	//if ( stabilityMargin != planData.stabilityMargin ){return false;}  20200819
	if ( elapsedTime != planData.elapsedTime ){return false;}

	return true;
}

/// 非等価演算子
bool PlanData::operator!=(const PlanData& planData)
{
	if ( *this == planData )
		return false;
	else
		return true;
}

/**
 *	------------------------------------------------------------
 *		PlanDataクラスのprivateなメンバ関数
 *	------------------------------------------------------------
 */
/**
 *	説明
 *		コピーコンストラクタと代入演算子のヘルプ関数
 */
void PlanData::copy(const PlanData& planData)
{

	//stabilityMargin = planData.stabilityMargin;  20200819
	elapsedTime = planData.elapsedTime;

	return;
}

}	/// end of namespace Data