/**
 *  File: BodyTrajectory.h
 *
 *  Description: 胴体軌道情報を扱うクラスの宣言部
 *					 直線軌道
 *
 *  Created: 2007/03/06(Tue)
 *  Updated: 2007/03/06(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#ifndef __BodyTrajectory_h__
#define __BodyTrajectory_h__

#include "Trajectory.h"
#include "..\..\Math\Matrix\Matrix.h"
#include "..\..\Utility\Constants.h"

namespace Plan
{
/**
 *		//////////////////////////////////////////////////
 *				BodyTrajectoryクラス
 *		//////////////////////////////////////////////////
 */
class BodyTrajectory : public Trajectory
{
/**
 *		//////////////////////////////////////////////////
 *				メンバ変数
 *		//////////////////////////////////////////////////
 */
private:
/**
 *		距離に関係するもの
 */
	/// 遊脚ストローク
	Math::Vector moveStride;

/**
 *		時間に関係するもの
 */
	/// 復帰時間
	double moveTime;

/**
 *		//////////////////////////////////////////////////
 *				メンバ関数
 *		//////////////////////////////////////////////////
 */
public:
/**
 *		コンストラクタとデストラクタ
 */
	/// デフォルトコンストラクタ
	BodyTrajectory();
	/// コピーコンストラクタ
	BodyTrajectory(const BodyTrajectory& trajectory);
	/// デストラクタ
	virtual ~BodyTrajectory();

/**
 *		演算子
 */
	/// 代入演算子
	BodyTrajectory& operator=(const BodyTrajectory& trajectory);
	/// 等価演算子
	bool operator==(const BodyTrajectory& trajectory) const;
	/// 非等価演算子
	bool operator!=(const BodyTrajectory& trajectory) const;

/**
 *		アクセス関数
 */
	/// 胴体移動距離
	const Math::Vector& getMoveStride(void) const{return moveStride;}
	/// 胴体移動時間
	const double getMoveTime(void) const{return moveTime;}

/**
 *		セット関数
 */
	/// 胴体移動時間
	void setMoveTime(double time) {moveTime = time; return;}

/**
 *		初期化
 */
	void initializeBodyTrajectory(void);

/**
 *		軌道パラメータの設定
 *		goalPointおよびgoalTimeも自動更新される
 */
	void setBodyTrajectory(const Math::Vector& stride);
	void setBodyTrajectoryTime(double time);

/**
 *		軌道を得る
 */
	Math::Vector getPosition(double splitTime);


private:
	/// コピーコンストラクタと代入演算子のためのヘルプ関数
	void copy(const BodyTrajectory& trajectory);

};	/// end of class BodyTrajectory

}	/// end of namespace Plan

#endif	/// __BodyTrajectory_h__