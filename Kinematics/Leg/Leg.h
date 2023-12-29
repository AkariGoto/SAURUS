/**
 *  ファイル名
 *		Leg.h
 *  説明
 *		1脚の関節や足先に関する情報，運動学などを解く
 *  日付
 *		作成日: 2007/01/31(WED)		更新日: 2018/02/12(MON)
 */

//20201015  第4リンク
//20201018  第4関節
//20221026  リンク4姿勢

#ifndef __Leg_h__
#define __Leg_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include <cassert>
#include "..\..\Math\Matrix\Matrix.h"
#include "..\..\Utility\Constants.h"
#include "..\AsuraParameter.h"

namespace Asura
{

/**
 *	----------------------------------------------------------------------
 *		Legクラス
 *	----------------------------------------------------------------------
 */
class Leg
{
/**
 *	------------------------------------------------------------
 *		内部クラスの宣言
 *	------------------------------------------------------------
 */
	/**
	 *		LegDataクラス：脚の情報
	 */
	class LegData
	{
		/**
		 *	------------------------------------------------------------
		 *		メンバ変数
		 *	------------------------------------------------------------
		 */
		public:
			/**
			 *	座標系は全て胴体座標系（ここではグローバル座標）
			 */
			/// 同時変換行列
			/// 脚根元: [4x4]
			Math::Matrix	baseTransformation;
			/// 関節: [4x4]x3
			Math::Matrix* jointTransformation;
			/// 足裏]: [4x4]
			Math::Matrix	footTransformation;

			Math::Vector bodyAngle;

			/**
			 *		位置ベクトル
			 */
			/// 脚根元: [3]
			Math::Vector	basePosition;
			/// 各関節: [3]x3
			Math::Vector* jointPosition;
			/// 足裏: [3]
			Math::Vector	footPosition;

			/**
			 *		関節の状態
			 */
			/// 関節角度: [3]-->[4]
			Math::Vector jointAngle;
			/// 関節速度: [3]
			Math::Vector jointVelocity;
			/// 関節トルク: [3]
			Math::Vector jointTorque;
			/// 足首関節角度: [1]
			//Math::Vector  FootjointAngle;//---Add
			double FootjointAngle;

			/**
			 *		アクチュエータの状態
			 */
			///アクチュエータ座標: [4]
			Math::Vector ActPos;
			/// 変換行列: [4x4]
			//Math::Matrix	ActTransformation;

			double LegTipAngle;  //20221026
			
		private:
		/**
		 *	------------------------------------------------------------
		 *		メンバ関数
		 *	------------------------------------------------------------
		 */
		/**
		 *	----------------------------------------
		 *	コンストラクタとデストラクタ
		 *	----------------------------------------
		 */
		public:
			/// コンストラクタ
			LegData();
			/// デストラクタ
			virtual ~LegData();

		private:
		    /// オブジェクトのメモリ領域を確保する
			void newLegData(void);
			/// オブジェクトのメモリ領域を解放する
		    void deleteLegData(void);
	};

/**
 *	------------------------------------------------------------
 *		内部構造体の宣言
 *	------------------------------------------------------------
 */
	/**
	 *	機構的な制限
	 */
	struct LegLimit
	{
		/// 関節角限界
		double angleMin[LEG_JOINT_NUM];
		double angleMax[LEG_JOINT_NUM];
		double FootangleMin;
		double FootangleMax;


		/// 足先反力限界
		double footReaction[3];

		/// 脚半径方向リーチ
		double reachRadiusMin;
		double reachRadiusMax;
	
		/// 脚高さ方向リーチ
		double reachHeightMin;
		double reachHeightMax;
	};

	/**
	 *	DHパラメータ
	*/
	struct DHParameter
	{
		double	alpha[LEG_JOINT_NUM];
		double	a[LEG_JOINT_NUM];
		double	d[LEG_JOINT_NUM];
	};


/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:
protected:
/**
 *	脚の情報
 *	初期化の順番のため先に宣言
 */
	LegData legData;

/**
 *	機構的な制限
 */
	LegLimit legLimit;

/**
 *	DHパラメータ
 */
	DHParameter dhParam;

/**
 *	ヤコビアン
 */
	/// ヤコビ行列: [3x3]
	Math::Matrix jacobian;
	/// 逆ヤコビ行列: [3x3]
	Math::Matrix inverseJacobian;

/**
 *	脚の根元の位置（x y z）・姿勢（θ）: [4]
 */
	Math::Vector basePose;	

/**
 *	脚の姿勢指標（逆運動学時に用いる）0 or 1
 */
	int poseIndicator[2];

/**
 *	脚番号
 */
	int legNo;


/**
 *	最後にエラーの原因となった関節番号（1，2，3）
 *	0は全てOK，エラー無
 */
	int legLastErrorJointNo;

private:

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
	 *	説明
	 *		デフォルトコンストラクタ
	 *	引数
	 *		x, y, x: 脚の根元位置		
	 *		theta: 脚の旋回軸回りの姿勢
	 */
	explicit Leg(int no=1, double x=0, double y=0, double z=0, double theta=0);

	/// デストラクタ
	virtual ~Leg();

/**
 *	----------------------------------------
 *	アクセス関数
 *	脚のデータを得る
 *	----------------------------------------
 */
	/**
	 *	同次変換行列
	 */
	/// 脚根元
	const Math::Matrix& getLegBaseTransformation(void) const {return legData.baseTransformation;}
	/// 関節
	const Math::Matrix& getLegJointTransformation(int jointNo) const;
	/// 足裏
	const Math::Matrix& getLegFootTransformation(void) const {return legData.footTransformation;}
	
	/**
	 *	位置ベクトル
	 */
	/// 脚根元
	const Math::Vector& getLegBasePosition(void) const {return legData.basePosition;}
	/// 関節位置
	const Math::Vector& getLegJointPosition(int jointNo) const;
	/// 足裏位置: [3]
	const Math::Vector& getLegFootPosition(void) const {return legData.footPosition;}

	/**
	 *	関節の状態
	 */
	/// 関節角度: [3]
	const Math::Vector& getLegJointAngle(void) const {return legData.jointAngle;}
	/// 関節速度: [3]
	const Math::Vector& getLegJointVelocity(void) const {return legData.jointVelocity;}
	/// 関節トルク: [3]
	const Math::Vector& getLegJointTorque(void) const {return legData.jointTorque;}
	/// 足首関節角度 :[1]
	const double getFootJointAngle(void) const {return legData.FootjointAngle;}//------Add

	const double getLegTipAngle(void) const { return legData.LegTipAngle; }  //20221026

	/**
	 *	ヤコビアン
	 */
	/// ヤコビ行列: [3x3]
	const Math::Matrix& getJacobian(void) const {return jacobian;}
	/// 逆ヤコビ行列: [3x3]
	const Math::Matrix& getInverseJacobian(void) const {return inverseJacobian;}
	
	/// 脚の根元の位置・姿勢: [4]
	const Math::Vector& getBasePose(void) const {return basePose;}	

	/// 脚番号の取得
	const int getLegNo(void) const {return legNo;}

	/// 関節のエラー番号
	const int getLegLastErrorJointNo(void) const {return legLastErrorJointNo;}

	/// ボールねじ位置: [4]（ワイヤ変位）
	const Math::Vector& getLegActuatorPosition(void) const {return legData.ActPos;}//------Add

/**
 *	----------------------------------------
 *	セット関数
 *	----------------------------------------
 */
	/// 脚番号の設定
	void setLegNo(int no);

/**
 *	----------------------------------------
 *	運動学
 *	----------------------------------------
 */
	/**
	 *	同次変換行列
	 */
	/// 脚根元
	Math::Matrix legBaseHomogeneousTransformation(void);
	/// 関節
	Math::Matrix legJointHomogeneousTransformation(int jointNo);
	/// 足裏
	Math::Matrix legFootHomogeneousTransformation(void);

	/**
	 *	逆同次変換行列
	 */
	/// 脚根元
	Math::Matrix legBaseInverseHomogeneousTransformation(void);
	/// 関節
	Math::Matrix legJointInverseHomogeneousTransformation(int jointNo);
	/// 足裏
	Math::Matrix legFootInverseHomogeneousTransformation(void);

	/// 順運動学
	Kinematics solveDirectKinematics(void);

	/// 逆運動学
	Kinematics solveInverseKinematics(void);

	/// 姿勢指標の変更
	void setLegPoseIndicator(int hip = 1, int knee = 1);

	/// 脚の根元設定
	void setLegBasePose(double x, double y, double z, double theta);
/**
 *	----------------------------------------
 *	脚の機構制限をチェック
 *	----------------------------------------
 */
	/// 関節の可動範囲を調査
	Kinematics checkLegJointMotionRange(void);

	/// 脚のリーチを調査
	Kinematics checkLegFootReachRange(void);

/**
 *	----------------------------------------
 *	脚のPTP制御用
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		足の位置をセット（足裏は常に重力方向と仮定）
 	 *		順運動学と逆運動学が一致しているかを確認する
	 *		途中の関節位置を計算する
	 */
	Kinematics placeLegFootPosition(const Math::Vector& nextFootPosition);

	/** 
	 *	説明
	 *		関節角をセット
	 *		セットした関節角で順運動学を解く
	 */
	Kinematics placeLegJointAngles(const Math::Vector& nextJointAngle, const double& nextFootJointAngle);
	//20201018  const double& nextFootJointAngle追加

	void setLegTipAngle(const double angle) { legData.LegTipAngle = angle; return; }  //20221026

/**
 *	----------------------------------------
 *	ヤコビアンを使っての状態計算
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		関節速度: [3]
	 *		脚先速度から計算
	 */
	void calculateLegJointVelocity(const Math::Vector& footVelocity);
	
	/**
	 *	説明
	 *		関節トルク: [3]
	 *		脚先端に加わった荷重から計算
	 */
	void calculateLegJointTorque(const Math::Vector& footReaction);

	/**
	 *	ヤコビアン自身の計算
	 */
	/// ヤコビ行列: [3x3]
	void calculateJacobian(void);
	/// 逆ヤコビ行列: [3x3]
	void calculateInverseJacobian(void);

	/**
	 *	説明
	 *		ボールねじ位置計算（ワイヤ変位）
	 */
	///指令値変換
	void calculationActatorPosition(void);

private:
/**
 *	------------------------------------------------------------
 *		値よりもインスタンスを重視してオブジェクト複製禁止
 *	------------------------------------------------------------
 */
	/// コピーコンストラクタ無効
	Leg(const Leg& leg);

	/// 代入演算子無効
	Leg& operator=(const Leg& leg)=delete;
};

/**
 *	------------------------------------------------------------
 *		inline関数
 *	------------------------------------------------------------
 */
/**
 *	----------------------------------------
 *	アクセス関数
 *	----------------------------------------
 */
/// 同時変換行列を返す
/// jointNo: 関節番号
inline const Math::Matrix& Leg::getLegJointTransformation(int jointNo) const
{
	/// 引数チェック
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );

	return ( legData.jointTransformation[jointNo-1] );
}

/// 関節位置を返す
/// jointNo: 関節番号
inline const Math::Vector& Leg::getLegJointPosition(int jointNo) const
{
	/// 引数チェック
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );

	return ( legData.jointPosition[jointNo-1] );
}


/**
 *	説明
 *		座標変換に必要な同次変換行列を導出する
 *		jointNo -> jointNo+1 への座標変換
 *
 *		A(1,1) = cos(theta);	A(1,2) = -cos(alpha)*sin(theta);		A(1,3) = sin(alpha)*sin(theta);			A(1,4) = a*cos(theta);
 *		A(2,1) = sin(theta);	A(2,2) = cos(alpha)*cos(theta);			A(2,3) = -sin(alpha)*cos(theta);		A(2,4) = a*sin(theta);
 *		A(3,1) = 0;				A(3,2) = sin(alpha);					A(3,3) = cos(alpha);					A(3,4) = d;
 *		A(4,1) = 0;				A(4,2) = 0;								A(4,3) = 0;								A(4,4) = 1;
 *//// 脚根元
inline Math::Matrix Leg::legBaseHomogeneousTransformation(void)
{
	/// 戻り値の行列
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	A(1,1) = cos( basePose(4) );
	A(2,1) = sin( basePose(4) );
	A(3,1) = 0;
	A(4,1) = 0;

	A(1,2) = -sin( basePose(4) );
	A(2,2) = cos( basePose(4) );
	A(3,2) = 0;
	A(4,2) = 0;

	A(1,3) = 0;
	A(2,3) = 0;
	A(3,3) = 1;
	A(4,3) = 0;

	A(1,4) = basePose(1);
	A(2,4) = basePose(2);
	A(3,4) = basePose(3);
	A(4,4) = 1;

	return A;

}

/// 関節
inline Math::Matrix Leg::legJointHomogeneousTransformation(int jointNo)
{
	/// 引数チェック
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );

	/// 戻り値の行列
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	A(1,1) = cos( legData.jointAngle(jointNo) );	
	A(2,1) = sin( legData.jointAngle(jointNo) );	
	A(3,1) = 0;											
	A(4,1) = 0;											

	A(1,2) = -cos( dhParam.alpha[jointNo-1] )*sin( legData.jointAngle(jointNo) );	
	A(2,2) = cos( dhParam.alpha[jointNo-1] )*cos( legData.jointAngle(jointNo) );	
	A(3,2) = sin( dhParam.alpha[jointNo-1] );
	A(4,2) = 0;

	A(1,3) = sin( dhParam.alpha[jointNo-1] )*sin( legData.jointAngle(jointNo) );	
	A(2,3) = -sin( dhParam.alpha[jointNo-1] )*cos( legData.jointAngle(jointNo) );
	A(3,3) = cos( dhParam.alpha[jointNo-1] );
	A(4,3) = 0;

	A(1,4) = dhParam.a[jointNo-1]*cos( legData.jointAngle(jointNo) );
	A(2,4) = dhParam.a[jointNo-1]*sin( legData.jointAngle(jointNo) );
	A(3,4) = dhParam.d[jointNo-1];
	A(4,4) = 1;

	return A;

}

/// 足裏
/// 胴体と足裏は常に平行と仮定して解く→基準姿勢や姿勢の移行を可能にするために，各関節の値から解く（20201018）
inline Math::Matrix Leg::legFootHomogeneousTransformation(void)
{
	/// 戻り値の行列
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);
	/// 足首関節角度
	//double angle = Const::PI / 2 - legData.jointAngle(2) - legData.jointAngle(3);//変更

	//20201015
	/*
	double angle;
	if (StandardPos)  //StandardPosがtrueの時のみ第4リンクが地面と平行として順運動学を解く
	{
		angle = 0;
	}
	else  //それ以外の場合は第4リンクは地面と垂直
	{
		angle = Const::PI / 2 - legData.jointAngle(2) - legData.jointAngle(3);//変更
	}
	*/
	//20201018
	double angle = legData.FootjointAngle;

	A(1,1) = cos( angle );	
	A(2,1) = sin( angle );	
	A(3,1) = 0;											
	A(4,1) = 0;											

	A(1,2) = -sin( angle );	
	A(2,2) = cos( angle );	
	A(3,2) = 0;
	A(4,2) = 0;

	A(1,3) = 0;	
	A(2,3) = 0;
	A(3,3) = 1;
	A(4,3) = 0;

	A(1,4) = FOOT*cos( angle );
	A(2,4) = FOOT*sin( angle );
	A(3,4) = 0;
	A(4,4) = 1;

	return A;
}

/**
 *	説明
 *		同次変換行列の逆行列を導出する
 *		jointNo+1 -> jointNo への座標変換
 *
 *		A(1,1) = cos(theta);					A(1,2) = sin(theta);					A(1,3) = 0;				A(1,4) = -a;
 *		A(2,1) = -cos(alpha)*sin(theta);		A(2,2) = cos(alpha)*cos(theta);			A(2,3) = sin(alpha);	A(2,4) = -d*sin(alpha);
 *		A(3,1) = sin(alpha)*sin(theta);			A(3,2) = -sin(alpha)*cos(theta);		A(3,3) = cos(alpha);	A(3,4) = -d*cos(alpha);
 *		A(4,1) = 0;								A(4,2) = 0;								A(3,4) = 0;				A(4,4) = 1;
 */
/// 脚根元
inline Math::Matrix Leg::legBaseInverseHomogeneousTransformation(void)
{
	/// 戻り値の行列
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	A(1,1) = cos( basePose(4) );
	A(2,1) = -sin( basePose(4) );//-追加
	A(3,1) = 0;
	A(4,1) = 0;

	A(1,2) = sin( basePose(4) );//-追加
	A(2,2) = cos( basePose(4) );
	A(3,2) = 0;
	A(4,2) = 0;

	A(1,3) = 0;
	A(2,3) = 0;
	A(3,3) = 1;
	A(4,3) = 0;

	A(1,4) = -( cos( basePose(4) )*basePose(1) + sin( basePose(4) )*basePose(2) );
	A(2,4) = -( -sin( basePose(4) )*basePose(1) + cos( basePose(4) )*basePose(2) );
	A(3,4) = -basePose(3);
	A(4,4) = 1;

	return A;
}

/// 関節
inline Math::Matrix Leg::legJointInverseHomogeneousTransformation(int jointNo)
{
	/// 引数チェック
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );

	/// 戻り値の行列
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	A(1,1) = cos( legData.jointAngle(jointNo) );	
	A(2,1) = -cos( dhParam.alpha[jointNo-1] )*sin( legData.jointAngle(jointNo) );		
	A(3,1) = sin( dhParam.alpha[jointNo-1] )*sin( legData.jointAngle(jointNo) );												
	A(4,1) = 0;											

	A(1,2) = sin( legData.jointAngle(jointNo) );
	A(2,2) = cos( dhParam.alpha[jointNo-1] )*cos( legData.jointAngle(jointNo) );	
	A(3,2) = -sin( dhParam.alpha[jointNo-1] )*cos( legData.jointAngle(jointNo) );
	A(4,2) = 0;

	A(1,3) = 0;
	A(2,3) = sin( dhParam.alpha[jointNo-1] );
	A(3,3) = cos( dhParam.alpha[jointNo-1] );
	A(4,3) = 0;

	A(1,4) = -dhParam.a[jointNo-1];
	A(2,4) = -dhParam.d[jointNo-1]*sin(  dhParam.alpha[jointNo-1] );
	A(3,4) = -dhParam.d[jointNo-1]*cos(  dhParam.alpha[jointNo-1] );
	A(4,4) = 1;

	return A;
}

/// 足裏
inline Math::Matrix Leg::legFootInverseHomogeneousTransformation(void)
{
	/// 戻り値の行列
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);
	/// 足首関節角度<-----変数で置き換える
	//double angle = Const::PI/2 - legData.jointAngle(2) - legData.jointAngle(3);
	double angle = legData.LegTipAngle - legData.jointAngle(2) - legData.jointAngle(3);  //20221026

	A(1,1) = cos( angle );
	A(2,1) = -sin( angle );
	A(3,1) = 0;
	A(4,1) = 0;

	A(1,2) = sin( angle );
	A(2,2) = cos( angle );
	A(3,2) = 0;
	A(4,2) = 0;

	A(1,3) = 0;
	A(2,3) = 0;
	A(3,3) = 1;
	A(4,3) = 0;

	A(1,4) = -FOOT;
	A(2,4) = 0;
	A(3,4) = 0;
	A(4,4) = 1;

	return A;
}


}	/// end of namespace Asura
 

#endif	/// __Leg_h__

