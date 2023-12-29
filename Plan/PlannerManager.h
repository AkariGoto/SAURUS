/**
 *  ファイル名
 *		PlannerManager.h
 *  説明
 *		動作計画を管理するクラスの宣言部
 *  日付
 *		作成日: 2007/09/01(SAT)		更新日: 2008/10/04(SAT)
 */

#ifndef __PlannerManager_h__
#define __PlannerManager_h__

#include "PlanParameter.h"
#include "Planner.h"

/**
 *	Stateデザインパターンで構成
 */

namespace Plan
{

/**
 *	----------------------------------------------------------------------
 *		PlannerManagerクラス
 *	----------------------------------------------------------------------
 */
class PlannerManager
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:

private:
/**
 *		目的の動作計画へのポインタ
 */
	Planner*	plannerPointer;

/**
 *		動作計画
 */
	/// 前回の計画
	Plan::Strategy lastStrategy;

	/// 選択した計画
	Plan::Strategy currentStrategy;

/**
 *	------------------------------------------------------------
 *		メンバ関数
 *	------------------------------------------------------------
 */
public:
/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
	/// デフォルトコンストラクタ
	explicit PlannerManager();
	/// デストラクタ
	virtual ~PlannerManager();

/**
 *	目的の動作を切替
 */
	bool switchPlan(Planner* planner, Plan::Strategy strategy);

/**
 *	----------------------------------------
 *	アクセス関数
 *	----------------------------------------
 */
	Plan::Strategy getCurrentStrategy(void) const{return currentStrategy;}
	Plan::Strategy getLastStrategy(void) const{return lastStrategy;}

/**
 *	実行中かどうか
 */
	bool isActive(void);

/**
 *	運動を具体的に生成する関数群
 */
	/// 計画開始のための初期化
	bool setup(void);

	/// 計画を開始する
	bool startMotion(void);
	
	/// 計画を停止する
	bool stopMotion(void);

	/// 停止の待機状態にする
	bool standByForStop(void);

	/// 運動を生成する
	PlanStatus activateRobot(void);

	/// ロボットの瞬間の状態を生成する
	PlanStatus createSnapshot(void);
};

}


#endif	/// __PlannerManager_h__