/**
 *  File: Trajectory.h
 *
 *  Description: 軌道情報を扱うクラスの宣言部
 *
 *  Created: 2007/03/06(Tue)
 *  Updated: 2007/03/06(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#ifndef __Trajectory_h__
#define __Trajectory_h__

#include "..\..\Math\Matrix\Matrix.h"
#include "..\..\Utility\Constants.h"

namespace Plan
{
/**
 *		//////////////////////////////////////////////////
 *				Trajectoryクラス
 *		//////////////////////////////////////////////////
 */
class Trajectory
{
/**
 *		//////////////////////////////////////////////////
 *				メンバ変数
 *		//////////////////////////////////////////////////
 */
protected:
/**
 *		距離に関係するもの
 */
	/// スタート位置
	Math::Vector startPosition;
	/// ゴール位置
	Math::Vector goalPosition;

/**
 *		時間に関係するもの
 */
	/// スタート時間
	double startTime;
	/// ゴール時間
	double goalTime;

private:

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
	Trajectory();
	/// コピーコンストラクタ
	Trajectory(const Trajectory& trajectory);
	/// デストラクタ
	virtual ~Trajectory();

/**
 *		演算子
 */
	/// 代入演算子
	virtual Trajectory& operator=(const Trajectory& trajectory);
	/// 等価演算子
	bool operator==(const Trajectory& trajectory) const;
	bool operator!=(const Trajectory& trajectory) const;

/**
 *		アクセス関数
 */
	/// 距離に関係するもの
	const Math::Vector& getStartPosition(void) const {return startPosition;}
			Math::Vector& getStartPosition(void) {return startPosition;}
	const Math::Vector& getGoalPosition(void) const {return goalPosition;}
			Math::Vector& getGoalPosition(void)  {return goalPosition;}

	/// 移動距離
	const Math::Vector getDistance(void) const{return (goalPosition - startPosition);}
			Math::Vector getDistance(void) {return (goalPosition - startPosition);}

	/// 時間に関係するもの
	const double getStartTime(void) const {return startTime;}
			double getStartTime(void) {return startTime;}
	const double getGoalTime(void) const {return goalTime;}
			double getGoalTime(void) {return goalTime;}

	/// 総軌道時間
	double getTime(void) const {return (goalTime - startTime);}
	
/**
 *		セット関数
 */
/**
 *		軌道パラメータの設定
 */
	void setDistance(const Math::Vector& start, const Math::Vector& goal);
	void setTime(double start, double goal);


/**
 *		初期化
 */
	void initializeTrajectory(void);

/**
 *		軌道パラメータの移行（スタートをずらす）
 */
	void shiftStartPosition(const Math::Vector& start);
	void shiftStartTime(double start);


/**
 *		軌道を得る
 */
	virtual Math::Vector getPosition(double splitTime);

private:
	/// コピーコンストラクタと代入演算子のためのヘルプ関数
	void copy(const Trajectory& trajectory);

};	/// end of class Trajectory

}	/// end of namespace Plan

#endif	/// __Trajectory_h__