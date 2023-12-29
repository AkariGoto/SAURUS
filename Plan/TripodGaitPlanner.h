/**
 *  ファイル名
 *		TripodGaitPlanner.h
 *  説明
 *		基準歩容計画クラス（トロット歩容での歩行）
 *  日付
 *		作成日: 2008/12/19(FRI)		更新日: 2008/12/19(FRI)
 */

 //  20200820  TrotGaitをTripodGaitに置換
//  20200929  支持脚時のためのz軸方向を追加
//  20201005  1周期で終了
//  20201016  歩行開始時の姿勢へのセット
//  20201017  歩行開始時の姿勢へのセット
//  20201020  動作停止後の再動作
 
#ifndef __TripodGaitPlanner_h__
#define __TripodGaitPlanner_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "PlanParameter.h"
#include "Planner.h"
#include "..\Kinematics\AsuraX.h"
#include "..\Math\MathLibrary.h"

namespace Plan
{
/**
 *				座標系の定義
 *
 *		 右手系					
 *									
 *						 y		
 *		x  1			4↑		
 *	   ←◎	|-----------|◎→	
 *	     ↓	|	  x		|   x			 
 *	     y	|	  ↑	|		
 *			|  ←◎		|		
 *			|    y		|y		
 *	    x	|			|↑	
 *	   ←◎	|-----------|◎→	
 *	     ↓2			3   x	
 *	     y				
 */

/**
 *	----------------------------------------------------------------------
 *		TripodGaitPlannerクラス
 *	----------------------------------------------------------------------
 */
	class TripodGaitPlanner : public Planner
	{
		/**
		 *	------------------------------------------------------------
		 *		メンバ変数
		 *	------------------------------------------------------------
		 */
	public:

	private:
		/**
		 *	周期歩容に関係するもの
		 */

		 /**
		  *	歩行周期
		  *		単位：秒
		  */
		double cycleTime;

		/**
		 *	デューティ比
		 *		1歩行周期中におけるある脚の支持脚期間の比率
		 */
		double dutyFactor;

		/**
		 *	ストライド
		 *		1周期中に胴体が移動する距離
		 *		ストロークはデューティ比 x ストライド
		 */
		double stride;

		/**
		 *	歩行速度
		 */
		double walkingSpeed;

		/**
		 *	歩行回数のカウンタ
		 */
		int walkingCounter;

		/**
		 *	歩行方向の単位ベクトル
		 */
		Math::Vector unitWalkingDirection;

		//20200929  支持脚時に使用するz軸方向の単位ベクトル
		Math::Vector unitUpDirection;

		/**
		 *	脚基準位置
		 *		脚の遊脚運動軌跡の中点位置
		 */
		Math::Vector* footReferencePosition;


		/**
		 *	歩行を行うために必要なパラメータ群
		 */
		 /// 胴体の初期位置
		Math::Vector initialBodyPosition;

		/// 足の初期位置
		Math::Vector* initialFootPosition;

		/// 遊脚開始位置
		Math::Vector* swingStartPosition;
		/// 遊脚終了位置
		Math::Vector* swingStopPosition;

		/// 遊脚振り上げ
		Math::Vector swingUp;
		/// 遊脚振り下げ
		Math::Vector swingDown;

		/// 歩行時間（0 < t < 周期）
		double walkingTime;
		/// 歩行周期開始時間
		double cycleStartTime;
		/// 正規化した歩行時間（0 < t < 1）
		double normalizedWalkingTime;

		/// 遊脚開始時間
		double* swingStartTime;
		double* swingStopTime;

		//20201020  停止時の歩行周期の経過時間
		double cycleElapsedTime;

		/**
		 *	歩行のためのフラグ群
		 */
		 /// 歩行の準備をする必要があるかどうかのフラグ
		bool isWalkingToGetSet;

		/// 脚軌道が決定されているかどうかのフラグ
		bool isTrajectoryToGetSet;

		/// 歩行開始のフラグ
		bool isWalkingStarted;

		/// 歩行停止待機フラグ
		bool isWaitingToStop;

		//20201016
		//  姿勢移行のフラグ
		bool isSetting;

		//20201017
		//  姿勢移行に用いる変数
		Math::Vector* initialJointAngle;  //移行前の関節角度
		Math::Vector* finalJointAngle;  //移行後の関節角度
		Math::Vector* settingJointAngle;  //移行中の関節角度
		//[i][j]で(i+1)番目の脚の第(j+1)関節の角度を示す
		double* initialFootJointAngle;  //移行前の関節角度
		double* finalFootJointAngle;  //移行後の関節角度
		double* settingFootJointAngle;  //移行中の関節角度

		Math::Vector* angularVelosity;  //姿勢移行中の角速度

		//それぞれの関節を動かし終わるまでの所要時間
		double* t01;
		double* t02;
		double* t03;
		double* t04;
		double* t05;

		double settingStartTime;  //姿勢移行開始時間
		double settingTime;  //姿勢移行開始からの経過時間

/**
 *	歩行に機能追加するための変数
 */
	/// 歩行停止直前の遊脚
	int swingLegWaitingToStop;

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
	explicit TripodGaitPlanner(Asura::AsuraX* asuraPointer_ = NULL, TimeManager* timeManagerPointer_ = NULL);
	/// デストラクタ
	virtual ~TripodGaitPlanner();

/**
 *	基準歩容の初期化
 */
	void initializeTripodGaitPlanner(void);

/**
 *	歩行準備をするための関数群
 */
	/// 歩容パラメータの計算
	void calculateGaitParameters(void);

	/// 歩行初期姿勢に移行
	bool shiftToInitialStandingPosture(void);

/**
 *	------------------------------------------------------------
 *	オーバーライド関数
 *		運動を具体的に生成する関数群
 *	------------------------------------------------------------
 */
	/// 歩行開始のための初期化
	virtual bool setup(void);

	/// 歩行を開始する
	virtual bool startPlan(void);	
	
	/// 歩行を即座に停止する
	virtual bool stopPlan(void);

	/// 歩行を1歩終了時に停止する
	virtual bool standByForStop(void);

	/// 脚運動を生成する
	virtual PlanStatus activateRobot(void);

	/// ロボットの瞬間の状態を生成する
	/// 指令値生成周期ごとに呼び出すことによりロボットの連続な動作を計画する
	virtual PlanStatus createPlanSnapshot(void);

	//20201016  姿勢の移行
	virtual bool settingPlan(void);

/**
 *	歩行を開始したかどうか
 */
	bool isWalking(void) const{return isWalkingStarted;}

private:
	/// コピーコンストラクタ無効
	TripodGaitPlanner(const TripodGaitPlanner& tripodtGaitPlanner);
	/// 代入演算子無効
	TripodGaitPlanner& operator=(const TripodGaitPlanner& tripodGaitPlanner);

	/// 歩行のためのオブジェクト生成
	void newTripodGaitItems(void);
	/// 歩行のためのオブジェクト消去
	void deleteTripodGaitItems(void);
};

}

#endif	/// __TripodGaitPlanner_h__ 