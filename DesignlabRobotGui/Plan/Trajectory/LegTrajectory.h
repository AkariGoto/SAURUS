/**
 *  File: LegTrajectory.h
 *
 *  Description: 脚軌道情報を扱うクラスの宣言部
 *					 長方形軌道
 *
 *  Created: 2007/03/06(Tue)
 *  Updated: 2007/03/06(Tue)
 *
 *  Copyright (C) Tokyo Institute of Technology Hirose Lab.
 */

#ifndef __LegTrajectory_h__
#define __LegTrajectory_h__

#include "Trajectory.h"
#include "..\..\Math\Matrix\Matrix.h"
#include "..\..\Utility\Constants.h"

namespace Plan
{
/**
 *		//////////////////////////////////////////////////
 *				LegTrajectoryクラス
 *		//////////////////////////////////////////////////
 */
class LegTrajectory : public Trajectory
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
	Math::Vector returnStroke;
	/// 遊脚振り上げ高さ
	Math::Vector upSwing;
	/// 遊脚振り下げ高さ
	Math::Vector downSwing;

	Math::Vector Swing;

/**
 *		時間に関係するもの
 */
	/// 復帰時間
	double returnTime;
	/// 振り上げ時間
	double upTime;
	/// 振り下げ時間
	double downTime;

/**
 *		//////////////////////////////////////////////////
 *				メンバ関数
 *		//////////////////////////////////////////////////
 */
/**
 *		コンストラクタとデストラクタ
 */
public:
	/// デフォルトコンストラクタ
	LegTrajectory();
	/// コピーコンストラクタ
	LegTrajectory(const LegTrajectory& trajectory);
	/// デストラクタ
	virtual ~LegTrajectory();

/**
 *		演算子
 */
	/// 代入演算子
	virtual LegTrajectory& operator=(const LegTrajectory& trajectory);
	/// 等価演算子
	bool operator==(const LegTrajectory& trajectory) const;
	/// 非等価演算子
	bool operator!=(const LegTrajectory& trajectory) const;

/**
 *		アクセス関数
 */
	/// 遊脚ストローク
	const Math::Vector& getReturnStroke(void) const{return returnStroke;}
	/// 遊脚振り上げ高さ
	const Math::Vector& getUpSwing(void) const{return upSwing;}
	/// 遊脚振り下げ高さ
	const Math::Vector& getDownSwing(void) const{return downSwing;}

	/// 復帰時間
	double getReturnTime(void) const{return returnTime;}
	/// 振り上げ時間
	double getUpTime(void) const{return upTime;}
	/// 振り下げ時間
	double getDownTime(void) const{return downTime;}


/**
 *		セット関数
 */
/**
 *		軌道パラメータの設定
 *		goalPointおよびgoalTimeも自動更新される
 */
	void setLegTrajectory(	
									const Math::Vector& start, 
									const Math::Vector& upPhase, 
									const Math::Vector& returnPhase, 
									const Math::Vector& downPhase
									);

	void setLegTrajectoryTime(double start, double upPhase, double returnPhase, double downPhase);

/**
 *		初期化
 */
	void initializeLegTrajectory(void);

/**
 *		軌道を得る
 *		time: 軌道中の時間（startTime < splitTime < goalTime）
 */
	Math::Vector getPosition(double splitTime);

private:
	/// コピーコンストラクタと代入演算子のためのヘルプ関数
	void copy(const LegTrajectory& trajectory);

};	/// end of class LegTrajectory

}	/// end of namespace lan
#endif	/// __LegTrajectory_h__