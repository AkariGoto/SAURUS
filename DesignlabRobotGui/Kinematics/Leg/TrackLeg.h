/**
 *  ファイル名
 *		TrackLeg.h
 *  説明
 *		クローラ可変型脚機構の情報，運動学
 *  日付
 *		作成日: 2007/02/05(MON)		更新日: 2007/02/19(MON)
 */

#ifndef __TrackLeg_h__
#define __TrackLeg_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include "..\..\Math\Matrix\Matrix.h"
#include "Leg.h"
#include "..\..\Utility\Constants.h"
#include "..\AsuraParameter.h"


namespace Asura
{

/**
 *	----------------------------------------------------------------------
 *		TrackLegクラス
 *	----------------------------------------------------------------------
 */
class TrackLeg : public Leg
{
/**
 *	------------------------------------------------------------
 *		内部クラスの宣言
 *	------------------------------------------------------------
 */
	/**
	 *	TrackDataクラス：胴体の情報
	 */
	class TrackData
	{
		/**
		 *	------------------------------------------------------------
		 *		メンバ変数
		 *	------------------------------------------------------------
		 */
		public:
			/**
			 *		同時変換行列
			 */	
			/// クローラ根元
			Math::Matrix	baseTransformation;
			/// 関節: [4x4]x2
			Math::Matrix* jointTransformation;
			/// クローラ先端
			Math::Matrix	endTransformation;

			/**
			 *		位置ベクトル
			 */
			/// クローラ根元
			Math::Vector	basePosition;
			/// 関節: [3]x2
			Math::Vector* jointPosition;
			/// クローラ先端
			Math::Vector	endPosition;

			/**
			 *		関節の状態
			 */
			/// 関節角度: [3]
			Math::Vector jointAngle;
			/// 関節速度: [3]
			Math::Vector jointVelocity;
			/// 関節トルク: [3]
			Math::Vector jointTorque;

			double trackSpeed;
	
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
			TrackData();
			/// デストラクタ
			virtual ~TrackData();

		private:
		    /// オブジェクトのメモリ領域を確保する
			void newTrackData(void);
			/// オブジェクトのメモリ領域を解放する
		    void deleteTrackData(void);
	};

/**
 *	------------------------------------------------------------
 *		内部構造体の宣言
 *	------------------------------------------------------------
 */
	/**
	 *	機構的な制限
	 */
	struct TrackLimit
	{
		/// 関節角限界
		double angleMin[TRACK_JOINT_NUM];
		double angleMax[TRACK_JOINT_NUM];

		double trackSpeedMax;

	};

/**
 *	------------------------------------------------------------
 *		メンバ変数
 *	------------------------------------------------------------
 */
public:
private:
/**
 *		脚の情報
 *		初期化の順番のため先に宣言
 */
	TrackData trackData;

/**
 *		機構的な制限
 */
	TrackLimit trackLimit;

/**
 *		脚機構の駆動方式
 */
	DriveSystem driveSystem;

/**
 *		最後にエラーの原因となった関節番号（1，2，3）
 *		0は全てOK，エラー無
 */
	int trackLastErrorJointNo;

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
	/**
	 *		x, y, x: 脚の根元位置		
	 *		theta: 脚の旋回軸回りの姿勢
	 */
	explicit TrackLeg(int no, double x=0, double y=0, double z=0, double theta=0);

	/// デストラクタ
	virtual ~TrackLeg();

/**
 *	----------------------------------------
 *	アクセス関数
 *	クローラのデータを得る
 *	----------------------------------------
 */
	/**
	 *	同次変換行列
	 */
	/// クローラ根元
	const Math::Matrix& getTrackBaseTransformation(void) const {return trackData.baseTransformation;}
	/// 関節
	const Math::Matrix& getTrackJointTransformation(int jointNo) const;
	/// クローラ外側の先端
	const Math::Matrix& getTrackEndTransformation(void) const {return trackData.endTransformation;}
	
	/**
	 *	位置ベクトル
	 */
	/// 脚根元
	const Math::Vector& getTrackBasePosition(void) const {return trackData.basePosition;}
	/// 関節
	const Math::Vector& getTrackJointPosition(int jointNo) const;
	/// クローラ外側の先端
	const Math::Vector& getTrackEndPosition(void) const {return trackData.endPosition;}

	/**
	 *	関節の状態
	 */
	/// 関節角度: [3]
	const Math::Vector& getTrackJointAngle(void) const {return trackData.jointAngle;}
	/// 関節速度: [3]
	const Math::Vector& getTrackJointVelocity(void) const {return trackData.jointVelocity;}
	/// 関節トルク: [3]
	const Math::Vector& getTrackJointTorque(void) const {return trackData.jointTorque;}

	/// クローラ速度
	const double getTrackSpeed(void) const {return trackData.trackSpeed;}

	/// 脚駆動方式
	const DriveSystem getDriveSystem(void) const {return driveSystem;}

	/// 関節のエラー番号
	const int getTrackLastErrorJointNo(void) const {return trackLastErrorJointNo;}

/**
 *	----------------------------------------
 *	運動学
 *	----------------------------------------
 */
	/**
	 *	同次変換行列を得る
	 */
	/// クローラ根元
	Math::Matrix trackBaseHomogeneousTransformation(void);
	/// 関節
	Math::Matrix trackJointHomogeneousTransformation(int jointNo);
	/// クローラ外側の先端
	Math::Matrix trackEndHomogeneousTransformation(void);

	/**
	 *	逆同次変換行列を得る
	 */
	/// クローラ根元
	Math::Matrix trackBaseInverseHomogeneousTransformation(void);
	/// 関節
	Math::Matrix trackJointInverseHomogeneousTransformation(int jointNo);
	/// クローラ外側の先端
	Math::Matrix trackEndInverseHomogeneousTransformation(void);

	/// クローラユニットの姿勢を変える
	Kinematics changeTrackPosture(const Math::Vector& angle);

/**
 *	----------------------------------------
 *	状態計算関数
 *	----------------------------------------
 */
	/**
	 *	説明
	 *		関節速度: [3]
	 *		クローラ先端の速度から計算
	 */
	void calculateTrackJointVelocity(const Math::Vector& trackVelocity);
	
	/**
	 *	説明
	 *		関節トルク: [3]
	 *		クローラ先端に加わった荷重から計算
	 */
	void calculateTrackJointTorque(const Math::Vector& trackReaction);

	/**
	 *	説明
	 *		クローラ速度を変更する
	 */
	void changeTrackSpeed(double trackSpeed);

	/**
	 *	説明
	 *		駆動方式の設定
	 *		戻り値：設定した駆動方式
	 */
	DriveSystem setDriveSystem(DriveSystem ds)  {driveSystem = ds; return driveSystem;}

	/**
	 *	説明
	 *		クローラの機構制限をチェックする
	 *		関節の可動範囲を調査
	*/
	Kinematics checkTrackJointMotionRange(void);

private:

/**
 *	------------------------------------------------------------
 *		値よりもインスタンスを重視してオブジェクト複製禁止
 *	------------------------------------------------------------
 */
	/// コピーコンストラクタ無効
	TrackLeg(const TrackLeg& trackLeg);

	/// 代入演算子無効
	TrackLeg& operator=(const TrackLeg& trackLeg);

};	/// end of class TrackLeg

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
/// 関節の同次変換行列を返す
/// jointNo: 関節番号
inline const Math::Matrix& TrackLeg::getTrackJointTransformation(int jointNo) const
{
	/// 引数チェック
	assert( 1 <= jointNo && jointNo <= TRACK_JOINT_NUM  );

	return trackData.jointTransformation[jointNo-1];

}

/// 関節位置を返す
/// jointNo: 関節番号
inline const Math::Vector& TrackLeg::getTrackJointPosition(int jointNo) const
{
	/// 引数チェック
	assert( 1 <= jointNo && jointNo <= TRACK_JOINT_NUM  );

	return trackData.jointPosition[jointNo-1];
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
 */
/// 脚根元
inline Math::Matrix TrackLeg::trackBaseHomogeneousTransformation(void)
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
inline Math::Matrix TrackLeg::trackJointHomogeneousTransformation(int jointNo)
{
	/// 引数チェック
	assert( 1 <= jointNo && jointNo <= LEG_JOINT_NUM );

	/// 戻り値の行列
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	A(1,1) = cos( trackData.jointAngle(jointNo) );	
	A(2,1) = sin( trackData.jointAngle(jointNo) );	
	A(3,1) = 0;											
	A(4,1) = 0;											

	A(1,2) = -cos( dhParam.alpha[jointNo-1] )*sin( trackData.jointAngle(jointNo) );	
	A(2,2) = cos( dhParam.alpha[jointNo-1] )*cos( trackData.jointAngle(jointNo) );	
	A(3,2) = sin( dhParam.alpha[jointNo-1] );
	A(4,2) = 0;

	A(1,3) = sin( dhParam.alpha[jointNo-1] )*sin( trackData.jointAngle(jointNo) );	
	A(2,3) = -sin( dhParam.alpha[jointNo-1] )*cos( trackData.jointAngle(jointNo) );
	A(3,3) = cos( dhParam.alpha[jointNo-1] );
	A(4,3) = 0;

	A(1,4) = dhParam.a[jointNo-1]*cos( trackData.jointAngle(jointNo) );
	A(2,4) = dhParam.a[jointNo-1]*sin( trackData.jointAngle(jointNo) );
	A(3,4) = dhParam.d[jointNo-1];
	A(4,4) = 1;

	return A;
}

/// クローラ外側先端
inline Math::Matrix TrackLeg::trackEndHomogeneousTransformation(void)
{
	/// 戻り値の行列
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	double angle = -Const::PI;

	A(1,1) = cos( angle );	
	A(2,1) = sin( angle );	
	A(3,1) = 0;											
	A(4,1) = 0;											

	A(1,2) = -cos( dhParam.alpha[2] )*sin( angle );	
	A(2,2) = cos( dhParam.alpha[2] )*cos( angle );	
	A(3,2) = sin( dhParam.alpha[2] );
	A(4,2) = 0;

	A(1,3) = sin( dhParam.alpha[2] )*sin( angle );	
	A(2,3) = -sin( dhParam.alpha[2] )*cos( angle );
	A(3,3) = cos( dhParam.alpha[2] );
	A(4,3) = 0;

	A(1,4) = dhParam.a[2]*cos( angle );
	A(2,4) = dhParam.a[2]*sin( angle );
	A(3,4) = dhParam.d[2];
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
inline Math::Matrix TrackLeg::trackBaseInverseHomogeneousTransformation(void)
{
	/// 戻り値の行列
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	A(1,1) = cos( basePose(4) );
	A(2,1) = -sin( basePose(4) );
	A(3,1) = 0;
	A(4,1) = 0;

	A(1,2) = sin( basePose(4) );
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
inline Math::Matrix TrackLeg::trackJointInverseHomogeneousTransformation(int jointNo)
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

/// クローラ外側先端
inline Math::Matrix TrackLeg::trackEndInverseHomogeneousTransformation(void)
{
	/// 戻り値の行列
	Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

	double angle = -Const::PI;

	A(1,1) = cos( angle );	
	A(2,1) = -cos( dhParam.alpha[2] )*sin( angle );		
	A(3,1) = sin( dhParam.alpha[2] )*sin( angle );												
	A(4,1) = 0;											

	A(1,2) = sin( angle );
	A(2,2) = cos( dhParam.alpha[2] )*cos( angle );	
	A(3,2) = -sin( dhParam.alpha[2] )*cos( angle );
	A(4,2) = 0;

	A(1,3) = 0;
	A(2,3) = sin( dhParam.alpha[2] );
	A(3,3) = cos( dhParam.alpha[2] );
	A(4,3) = 0;

	A(1,4) = -dhParam.a[2];
	A(2,4) = -dhParam.d[2]*sin(  dhParam.alpha[2] );
	A(3,4) = -dhParam.d[2]*cos(  dhParam.alpha[2] );
	A(4,4) = 1;

	return A;
}

}
#endif /// __TrackLeg_h__