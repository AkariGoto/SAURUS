/**
 *  ファイル名
 *		DataHandler.h
 *  説明
 *		ロボットのデータハンドラ
 *  日付
 *		作成日: 2007/02/12(Mon)		更新日: 2007/02/19(Mon)
 */
//20200819  クローラ関連コメントアウト


#ifndef __DataHandler_h__
#define __DataHandler_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "..\Kinematics\AsuraX.h"
#include "..\Plan\Planner.h"
#include "AsuraData.h"
#include "PlanData.h"
#include "..\Kinematics\AsuraParameter.h"
#include "..\Plan\PlanParameter.h"

namespace Data
{

/**
 *	----------------------------------------------------------------------
 *		DataHandlerクラス
 *	----------------------------------------------------------------------
 */
class DataHandler
{
/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:
private:
/**
 *	データを取得するロボットへのポインタ
 */
	/// キネマティクスデータ
	Asura::AsuraX* asuraDataSourcePointer;
	/// 計画データ
	Plan::Planner* planDataSourcePointer;


/**
 *	データを保存するデータへのポインタ
 */
	Data::AsuraData* asuraDataTargetPointer;
	Data::PlanData* planDataTargetPointer;

/**
 *	保持データ
 */
	/// キネマティクス
	Data::AsuraData asuraData;
	/// 計画
	Data::PlanData planData;

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
	/**
	 *	デフォルトコンストラクタ
	 *	引数
	 *		asuraPtr: データを受け取るAsuraXクラスへのポインタ
	 *		asuraDataPtr: データを渡すAsuraDataクラスへのポインタ
	 *		planPtr: データを受け取るPlannerクラスへのポインタ
	 *		planDataPtr: データを渡すplanDataPtrクラスへのポインタ
	 */
	explicit DataHandler(Asura::AsuraX* asuraPtr, Data::AsuraData* asuraDataPtr, 
								Plan::Planner* planPtr, Data::PlanData* planDataPtr);

	/// デストラクタ
	virtual ~DataHandler();

/**
 *	----------------------------------------
 *	データ元のポインタの取得・解放
 *	----------------------------------------
 */
	/// 取得
	void acquireAsuraDataSource(Asura::AsuraX* asuraPointer);
	/// 解放
	void releaseAsuraDataSource(void);

	/// 取得
	void acquirePlanDataSource(Plan::Planner* planPointer);
	/// 解放
	void releasePlanDataSource(void);

/**
 *	----------------------------------------
 *	データ先のポインタの取得・解放
 *	----------------------------------------
 */
	/// 取得
	void acquireAsuraDataTarget(Data::AsuraData* dataPointer);
	/// 解放
	void releaseAsuraDataTarget(void);

	/// 取得
	void acquirePlanDataTarget(Data::PlanData* dataPointer);
	/// 解放
	void releasePlanDataTarget(void);

/**
 *	----------------------------------------
 *	データの取り込みと転送
 *	----------------------------------------
 */
	/// 転送
	void exportAsuraData(void);
	/// 取り込み
	void importAsuraData(void);

	/// 転送
	void exportPlanData(void);
	/// 取り込み
	void importPlanData(void);


/**
 *	データ渡し
 */
	void handleAsuraData(void);
	void handlePlanData(void);

/**
 *	----------------------------------------
 *	アクセス関数
 *	----------------------------------------
 */
/**
 *	キネマティクスデータ
 */
	const Data::AsuraData& getAsuraData(void) const{return asuraData;}

	/**
	 *	胴体
	 */
	/// 同次変換行列
	const Math::Matrix& getBodyTransformation(void) const {return asuraData.getBodyTransformation();}

	/// 胴体位置
	const Math::Vector& getBodyPosition(void) const {return asuraData.getBodyPosition();}

	/// 胴体速度
	const Math::Vector& getBodyVelocity(void) const {return asuraData.getBodyVelocity();}

	/**
	 *	脚
	 */
	/// 脚根元ごとの同次変換行列
	const Math::Matrix& getLegBaseTransformation(int legNo) const;

	/// 脚関節ごとの同次変換行列
	const Math::Matrix& getLegJointTransformation(int legNo, int jointNo) const;

	/// 足位置ごとの同次変換行列
	const Math::Matrix& getLegFootTransformation(int legNo) const;

	/// 脚の根元位置
	const Math::Vector& getLegBasePosition(int legNo) const;

	/// 脚の関節位置
	const Math::Vector& getLegJointPosition(int legNo, int jointNo) const;

	/// 足位置
	const Math::Vector& getLegFootPosition(int legNo) const;

	/// 脚の関節角度
	const Math::Vector& getLegJointAngle(int legNo) const;

	/// 脚の関節速度
	const Math::Vector& getLegJointVelocity(int legNo) const;

	/// 脚の関節トルク
	const Math::Vector& getLegJointTorque(int legNo) const;

	/// 脚の運動相
	const Asura::LegPhase getLegPhase(int legNo) const;

	/**
	 *	クローラ
	 */
	/*  20200819
	/// クローラ根元ごとの同次変換行列
	const Math::Matrix& getTrackBaseTransformation(int trackNo) const;

	/// クローラ関節ごとの同次変換行列
	const Math::Matrix& getTrackJointTransformation(int trackNo, int jointNo) const;

	/// クローラ先端ごとの同次変換行列
	const Math::Matrix& getTrackEndTransformation(int trackNo) const;

	/// クローラの根元位置
	const Math::Vector& getTrackBasePosition(int trackNo) const;

	/// クローラの関節位置
	const Math::Vector& getTrackJointPosition(int trackNo, int jointNo) const;

	/// クローラの根元位置
	const Math::Vector& getTrackEndPosition(int trackNo) const;

	/// クローラの関節角度
	const Math::Vector& getTrackJointAngle(int trackNo) const;

	/// クローラの関節速度
	const Math::Vector& getTrackJointVelocity(int trackNo) const;

	/// クローラの関節トルク
	const Math::Vector& getTrackJointTorque(int trackNo) const;
	*/

/**
 *	計画データ
 */
	const Data::PlanData& getPlanData(void) const{return planData;}

	//const double getStabilityMargin(void) const{return planData.getStabilityMargin();}  20200819
	const double getElapsedTime(void) const{return planData.getElapsedTime();}

private:
};


}
#endif /// __DataHandler_h__