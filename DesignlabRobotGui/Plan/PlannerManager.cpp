/**
 *  ファイル名
 *		PlannerManager.cpp
 *  説明
 *		動作計画を管理するクラスの宣言部
 *  日付
 *		作成日: 2007/09/01(SAT)		更新日: 2008/10/04(SAT)
 */


#include "PlannerManager.h"
#include "iostream"

using namespace std;

namespace Plan
{

/**
 *	----------------------------------------------------------------------
 *		PlannerManagerクラス
 *	----------------------------------------------------------------------
 */

/**
 *	------------------------------------------------------------
 *		PlannerManagerクラスのメンバ関数定義
 *	------------------------------------------------------------
 */

/**
 *	----------------------------------------
 *	コンストラクタとデストラクタ
 *	----------------------------------------
 */
/// デフォルトコンストラクタ
PlannerManager::PlannerManager() : plannerPointer(NULL)
{
}

/// デストラクタ
PlannerManager::~PlannerManager()
{
	plannerPointer = NULL;
}

/**
 *	目的の動作を切替
 */
bool PlannerManager::switchPlan(Planner* planner, Plan::Strategy strategy)
{
	if ( strategy == Plan::NO_STRATEGY )
	{
		/// 計画をなくす
		plannerPointer = NULL;

		/// 計画の記録
		lastStrategy = currentStrategy;
		currentStrategy = strategy;
		
		return true;
	}

	/// 引数のチェック
	if ( planner == NULL )
	{
		cerr << "Error: [PlannerManager::switchPlan] Invalid argument" << endl;
		return false;
	}

	/// 計画の切替え
	plannerPointer = planner;

	/// 計画の記録
	lastStrategy = currentStrategy;
	currentStrategy = strategy;

	return true;
}

/*
 *	実行中かどうかの判定
 */
bool PlannerManager::isActive(void)
{
	if ( plannerPointer == NULL ) return false;

	return plannerPointer->isActive();
}

/**
 *	運動を具体的に生成する関数群
 */
/// 計画開始のための初期化
bool PlannerManager::setup(void)
{
	if ( plannerPointer == NULL ) return false;
	if (currentStrategy == TRIPOD_1_CYCLE)  //20201005
	{
		plannerPointer->standByForStop();
	}
	return plannerPointer->setup();
}

/// 計画を開始する
bool PlannerManager::startMotion(void)
{
	if ( plannerPointer == NULL ) return false;

	return plannerPointer->startPlan();
}
	
/// 計画を停止する
bool PlannerManager::stopMotion(void)
{
	if ( plannerPointer == NULL ) return false;

	return plannerPointer->stopPlan();
}

/// 停止の待機状態にする
bool PlannerManager::standByForStop(void)
{
	if ( plannerPointer == NULL ) return false;

	return plannerPointer->standByForStop();
}

/// 運動を生成する
PlanStatus PlannerManager::activateRobot(void)
{
	if ( plannerPointer == NULL ) return Plan::INVALID;

	return plannerPointer->activateRobot();
}

/// ロボットの瞬間の状態を生成する
PlanStatus PlannerManager::createSnapshot(void)
{
	if ( plannerPointer == NULL ) return Plan::INVALID;

	return plannerPointer->createPlanSnapshot();
}


}