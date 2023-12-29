/**
 *  ファイル名
 *		PlanData.h
 *  説明
 *		移動計画のデータ（時間、安定余裕など）
 *  日付
 *		作成日: 2007/02/12(MON)		更新日: 2007/02/19(MON)
 */

//20200819  安定余裕関連コメントアウト

#ifndef __PlanData_h__
#define __PlanData_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "..\Math\Matrix\Matrix.h"
#include "..\Kinematics\AsuraParameter.h"
#include "..\Plan\PlanParameter.h"
#include "..\Utility\Constants.h"

namespace Data
{

/**
 *	----------------------------------------------------------------------
 *		PlanDataクラス
 *	----------------------------------------------------------------------
 */
class PlanData
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
private:

	/// 安定余裕
	//double stabilityMargin;  20200819

	/// 経過時間
	double elapsedTime;

	/// 側行角
	double crabAngle;


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
	PlanData();

	/// コピーコンストラクタ
	PlanData(const PlanData& PlanData);

	/// デストラクタ
	virtual ~PlanData();

/**
 *	----------------------------------------
 *	演算子
 *	----------------------------------------
 */
	/// 代入演算子
	virtual PlanData& operator=(const PlanData& planData);

	/// 等価演算子
	bool operator==(const PlanData& planData);

	/// 非等価演算子
	bool operator!=(const PlanData& planData);

/**
 *	クラス自身を複製する
 */
	//PlanData& clone(void); 20210622関数定義なし。コメントアウト

/**
 *	----------------------------------------
 *	アクセス関数
 *	----------------------------------------
 */
	/// 安定余裕
	//double getStabilityMargin(void) const{return stabilityMargin;}  20200819
	/// 時間
	double getElapsedTime(void) const{return elapsedTime;}

/**
 *	----------------------------------------
 *	セット関数
 *	----------------------------------------
 */
	/// 安定余裕
	//void setStabilityMargin(double margin) { stabilityMargin = margin; return; }  20200819
	/// 時間
	void setElapsedTime(double time) { elapsedTime = time; return; }

private:
	/// コピーコンストラクタと代入演算子のヘルプ関数
	void copy(const PlanData& planData);

};	/// end of class PlanData

}	/// end of namespace Data

#endif /// __PlanData_h__