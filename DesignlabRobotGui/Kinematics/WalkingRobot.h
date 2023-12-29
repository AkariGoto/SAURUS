/**
 *  ファイル名
 *		WalkingRobot.h
 *  説明
 *		歩行ロボットの情報，運動学
 *		LegTrackクラスのコンポジション
 *  日付
 *		作成日: 2007/02/05(MON)		更新日: 2018/11/05(MON)
 */

 //  20200819  クローラ関連コメントアウト・TrackLegを介さずにLegを呼ぶ
 //  20201015  初期高さ
 //  20201018  第4関節

#ifndef __WalkingRobot_h__
#define __WalkingRobot_h__

/**
 *	----------------------------------------------------------------------
 *		ヘッダファイルインクルード
 *	----------------------------------------------------------------------
 */
#include <cassert>
#include "..\Math\Matrix\Matrix.h"
#include "Leg\Leg.h" 
#include "AsuraParameter.h"
#include "..\Plan\PlanParameter.h"

namespace Asura
{

/**
 *	----------------------------------------------------------------------
 *		WalkingRobotクラス
 *	----------------------------------------------------------------------
 */
class WalkingRobot
{

    class BodyData
    {
    public:
        /// 同時変換行列: [4x4]
        Math::Matrix transformation;

        /// 胴体の位置ベクトル：[3]
        Math::Vector position;

        /// 胴体速度：[3]
        Math::Vector velocity;

        /// コンストラクタ
        BodyData();
        /// デストラクタ
        virtual ~BodyData();

    private:
        /// オブジェクトのメモリ領域を確保する
        void initBodyData(void);
    };

    /**
     *	------------------------------------------------------------
     *		メンバ変数
     *	------------------------------------------------------------
     */
public:
protected:
    /**
     *		歩行ロボットのデータ
     */
     /// 歩行ロボットのデータ
    BodyData bodyData;

    /// 移動様式
    LocomotionStyle locomotionStyle;

    /**
     *		クローラ可変型脚機構に関する変数
     */
     /// 脚機構のインスタンス
     //TrackLeg** trackLegs;
    Leg** trackLegs;  //  20200819

    /// 脚の運動相のインスタンス
    LegPhase* legPhases;

    /**
     *		最後にエラーとなった脚番号
     *		0は全てOKでエラー無
     */
    int lastErrorTrackLegNo;

    /**
     *		最後の運動学エラー
     */
    Kinematics lastKineError;

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
     /// デフォルトコンストラクタ
    WalkingRobot();
    explicit WalkingRobot(const Math::Vector& bodyPosition);

    /// デストラクタ
    virtual ~WalkingRobot();

    /// 初期化関数
    void initializeWalkingRobot(void);
    /**
     *	----------------------------------------
     *	ロボット本体に関するもの
     *	----------------------------------------
     */
     /**
      *	----------------------------------------
      *	アクセス関数
      *	----------------------------------------
      */
      /// 胴体の同時変換行列: [4x4]
    const Math::Matrix& getBodyTransformation(void) const { return bodyData.transformation; }
    /// 胴体の位置ベクトル：[3]
    const Math::Vector& getBodyPosition(void) const { return bodyData.position; }
    /// 胴体の速度ベクトル：[3]
    const Math::Vector& getBodyVelocity(void) const { return bodyData.velocity; }

    /// 移動様式
    const LocomotionStyle getLocomotionStyle(void) const { return locomotionStyle; }
    ///運動学エラー
    const Kinematics getLastKineError(void) const { return lastKineError; }

    /// 脚オブジェクト
    //const TrackLeg& getTrackLeg(int legNo) const;    20200819
    /// 脚の運動相
    const LegPhase getLegPhase(int legNo) const;
    /// 最後にエラーを起こした脚番号を取得
    const int getLastErrorTrackLegNo(void) const { return lastErrorTrackLegNo; }

    /**
     *	----------------------------------------
     *	セット関数
     *	----------------------------------------
     */
     /// 胴体の位置を刷新
    void initializeBodyTransformation(const Math::Matrix& newBodyTransformation);
    /// 胴体の位置を刷新
    void initializeBodyPosition(const Math::Vector& newBodyPosition);
    void initializeBodyPosition(double x, double y, double z);

    /// 移動様式
    void setLocomotionStyle(LocomotionStyle style);
    /// 脚の運動相
    void setLegPhase(int legNo, LegPhase phase);

    /**
     *	----------------------------------------
     *	運動学
     *	----------------------------------------
     */
     /// 胴体の位置を決定する
    Kinematics placeBodyPosition(Math::Vector& nextBodyPosition);
    /// 胴体の姿勢を決定する
    Kinematics placeBodyPosture(Math::Matrix& nextBodyPosture);
    /// 胴体の位置姿勢を同時に決定する
    Kinematics placeBodyFrame(Math::Matrix& nextBodyFrame);

    /**
     *	----------------------------------------
     *	座標変換
     *	----------------------------------------
     */
     /// グローバルから胴体ローカルへ
    Math::Matrix transformationGlobalToLocal(const Math::Matrix& matrix);
    Math::Vector transformationGlobalToLocal(const Math::Vector& vector);
    /// 胴体ローカルからグローバルへ
    Math::Matrix transformationLocalToGlobal(const Math::Matrix& matrix);
    Math::Vector transformationLocalToGlobal(const Math::Vector& vector);

    Math::Matrix rollY(double angle);

    /**
     *	----------------------------------------
     *	脚に関するもの
     *	TrackLegクラスを介した脚クラス関数の呼び出し  (TrackLegクラスを介さない形に変更  20200819)
     *	----------------------------------------
     */
     /**
      *	----------------------------------------
      *	アクセス関数
      *	----------------------------------------
      */
      /// 脚根元の同次変換行列
    const Math::Matrix& getLegBaseTransformation(int legNo) const;
    /// 関節の同次変換行列
    const Math::Matrix& getLegJointTransformation(int legNo, int jointNo) const;
    /// 足裏の同次変換行列
    const Math::Matrix& getLegFootTransformation(int legNo) const;
    /// 脚根元の位置ベクトル
    const Math::Vector& getLegBasePosition(int legNo) const;
    /// 関節位置の位置ベクトル
    const Math::Vector& getLegJointPosition(int legNo, int jointNo) const;
    /// 足裏位置の位置ベクトル
    const Math::Vector& getLegFootPosition(int legNo) const;
    /// 関節角度ベクトル
    const Math::Vector& getLegJointAngle(int legNo) const;
    /// 足首関節角度
    const double getFootJointAngle(int legNo) const;//-------Add
    /// 関節速度ベクトル
    const Math::Vector& getLegJointVelocity(int legNo) const;
    /// 関節トルクベクトル
    const Math::Vector& getLegJointTorque(int legNo) const;
    /// 脚の根元の位置・姿勢
    const Math::Vector& getBasePose(int legNo) const;
    /// 脚番号の取得
    const int getLegNo(int legNo) const;
    /// 関節のエラー番号
    const int  getLegLastErrorJointNo(int legNo) const;
    /// アクチュエーター座標
    const Math::Vector& getLegActuatorPosition(int legNo) const;

    /**
     *	----------------------------------------
     *	セット関数
     *	----------------------------------------
     */
     /// 脚番号の設定
    void setLegNo(int legNo, int no);

    /**
     *	----------------------------------------
     *	運動学
     *	----------------------------------------
     */
     /// 順運動学
    Kinematics solveLegDirectKinematics(int legNo);
    /// 逆運動学
    Kinematics solveLegInverseKinematics(int legNo);
    /// 姿勢指標の変更
    void setLegPoseIndicator(int legNo, int hip = -1, int knee = -1);
    /// 脚の根元設定
    void setLegBasePose(int legNo, double x, double y, double z, double theta);

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
    Kinematics placeLegFootPosition(int legNo, const Math::Vector& nextFootPosition);
    /**
     *	説明
     *		関節角をセット
     *		セットした関節角で順運動学を解く
     */
    Kinematics placeLegJointAngles(int legNo, const Math::Vector& nextJointAngle, const double& nextFootJointAngle);
    // const double& nextFootJointAngle追加
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
    void calculateLegJointVelocity(int legNo, const Math::Vector& footVelocity);
    /**
     *	説明
     *		関節トルク: [3]
     *		脚先端に加わった荷重から計算
     */
    void calculateLegJointTorque(int legNo, const Math::Vector& footForce);

    /**
     *	----------------------------------------
     *	クローラに関するもの
     *	----------------------------------------
     */

     /**
      *	----------------------------------------
      *	アクセス関数
      *	----------------------------------------
      */
      /*  20200819
       /// クローラ根元の同次変換行列
       const Math::Matrix& getTrackBaseTransformation(int trackNo) const;
       /// クローラ関節の同次変換行列
       const Math::Matrix& getTrackJointTransformation(int trackNo, int jointNo) const;
       /// クローラ先端の同次変換行列
       const Math::Matrix& getTrackEndTransformation(int trackNo) const;
       /// クローラ根元の位置ベクトル
       const Math::Vector& getTrackBasePosition(int trackNo) const;
       /// クローラ関節の位置ベクトル
       const Math::Vector& getTrackJointPosition(int trackNo, int jointNo) const;
       ///クローラ先端の位置ベクトル
       const Math::Vector& getTrackEndPosition(int trackNo) const;
       /// 関節角度ベクトル
       const Math::Vector& getTrackJointAngle(int trackNo) const;
       /// 関節速度ベクトル
       const Math::Vector& getTrackJointVelocity(int trackNo) const;
       /// 関節トルクベクトル
       const Math::Vector& getTrackJointTorque(int trackNo) const;
       /// クローラスピード
       const double getTrackSpeed(int trackNo) const;

       /// 脚駆動方式
       const DriveSystem getDriveSystem(int trackNo) const;
       /// クローラ形態時に最後にエラーとなった関節番号
       const int getTrackLastErrorJointNo(int trackNo) const;
       */
       /**
        *	----------------------------------------
        *	クローラの運動学
        *	----------------------------------------
        */


        /**
         *	説明
         *		クローラユニットの姿勢を変える
         */
         //Kinematics changeTrackPosture(int trackNo, const Math::Vector& angle);  20200819
         /**
          *	説明
          *		関節速度: [3]
          *		クローラ先端の速度から計算
          */
          //void calculateTrackJointVelocity(int trackNo, const Math::Vector& trackVelocity);  20200819
          /**
           *	説明
           *		関節トルク: [3]
           *		クローラ先端に加わった荷重から計算
           */
           //void calculateTrackJointTorque(int trackNo, const Math::Vector& trackForce);  20200819
           /**
            *	説明
            *		クローラ速度を変更する
            */
            //void changeTrackSpeed(int trackNo, double trackSpeed);  20200819
            /**
             *	説明
             *		駆動方式の設定
             *		戻り値：設定した駆動方式
             */
             //DriveSystem setDriveSystem(int trackLegNo, DriveSystem ds);  20200819

private:
    /**
     *	説明
     *		脚のインスタンス生成
     */
    void newTrackLegs(void);
    /**
     *	説明
     *		脚のインスタンス破棄
     */
    void deleteTrackLegs(void);

};	/// end of class WalkingRobot

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
  /// 脚のインスタンス
  /*  20200819
  inline const TrackLeg& WalkingRobot::getTrackLeg(int legNo) const
 {
   /// 引数チェック
   assert( 1 <= legNo && legNo <= LEG_NUM );

   return *trackLegs[legNo-1];
 }
 */

 /// 脚の運動相
inline const LegPhase WalkingRobot::getLegPhase(int legNo) const
{
    /// 引数チェック
    assert(1 <= legNo && legNo <= LEG_NUM);

    return legPhases[legNo - 1];
}

/**
 *	----------------------------------------
 *	座標変換
 *	----------------------------------------
 */
 /// グローバルから胴体ローカルへ
 /// 同時座標変換行列を変換する
inline Math::Matrix WalkingRobot::transformationGlobalToLocal(const Math::Matrix& matrix)
{
    assert(matrix.getRow() == Const::DH_DIMENSION && matrix.getColumn() == Const::DH_DIMENSION);

    Math::Matrix result(Const::DH_DIMENSION, Const::DH_DIMENSION);
    Math::Matrix transform(Const::DH_DIMENSION, Const::DH_DIMENSION);

    transform(1, 1) = bodyData.transformation(1, 1);
    transform(2, 1) = bodyData.transformation(1, 2);
    transform(3, 1) = bodyData.transformation(1, 3);
    transform(4, 1) = 0;

    transform(1, 2) = bodyData.transformation(2, 1);
    transform(2, 2) = bodyData.transformation(2, 2);
    transform(3, 2) = bodyData.transformation(2, 3);
    transform(4, 2) = 0;

    transform(1, 3) = bodyData.transformation(3, 1);
    transform(2, 3) = bodyData.transformation(3, 2);
    transform(3, 3) = bodyData.transformation(3, 3);
    transform(4, 3) = 0;


    transform(1, 4) = -(bodyData.transformation(1, 1) * bodyData.transformation(1, 4) +
                bodyData.transformation(2, 1) * bodyData.transformation(2, 4) +
                bodyData.transformation(3, 1) * bodyData.transformation(3, 4));
    transform(2, 4) = -(bodyData.transformation(1, 2) * bodyData.transformation(1, 4) +
                bodyData.transformation(2, 2) * bodyData.transformation(2, 4) +
                bodyData.transformation(3, 2) * bodyData.transformation(3, 4));
    transform(3, 4) = -(bodyData.transformation(1, 3) * bodyData.transformation(1, 4) +
                bodyData.transformation(2, 3) * bodyData.transformation(2, 4) +
                bodyData.transformation(3, 3) * bodyData.transformation(3, 4));
    transform(4, 4) = 1;

    result = transform * matrix;

    return result;

}

/// グローバルから胴体ローカルへ
/// 3次元位置ベクトルを変換する
inline Math::Vector WalkingRobot::transformationGlobalToLocal(const Math::Vector& vector)
{
    assert(vector.getSize() == Const::THREE_DIMENSION);

    Math::Vector result(Const::THREE_DIMENSION);
    Math::Vector left(Const::DH_DIMENSION);
    Math::Vector right(Const::DH_DIMENSION);
    Math::Matrix transform(Const::DH_DIMENSION, Const::DH_DIMENSION);

    /// グローバルから胴体ローカルへの同次変換行列
    transform(1, 1) = bodyData.transformation(1, 1);
    transform(2, 1) = bodyData.transformation(1, 2);
    transform(3, 1) = bodyData.transformation(1, 3);
    transform(4, 1) = 0;

    transform(1, 2) = bodyData.transformation(2, 1);
    transform(2, 2) = bodyData.transformation(2, 2);
    transform(3, 2) = bodyData.transformation(2, 3);
    transform(4, 2) = 0;

    transform(1, 3) = bodyData.transformation(3, 1);
    transform(2, 3) = bodyData.transformation(3, 2);
    transform(3, 3) = bodyData.transformation(3, 3);
    transform(4, 3) = 0;


    transform(1, 4) = -(bodyData.transformation(1, 1) * bodyData.transformation(1, 4) +
                bodyData.transformation(2, 1) * bodyData.transformation(2, 4) +
                bodyData.transformation(3, 1) * bodyData.transformation(3, 4));
    transform(2, 4) = -(bodyData.transformation(1, 2) * bodyData.transformation(1, 4) +
                bodyData.transformation(2, 2) * bodyData.transformation(2, 4) +
                bodyData.transformation(3, 2) * bodyData.transformation(3, 4));
    transform(3, 4) = -(bodyData.transformation(1, 3) * bodyData.transformation(1, 4) +
                bodyData.transformation(2, 3) * bodyData.transformation(2, 4) +
                bodyData.transformation(3, 3) * bodyData.transformation(3, 4));
    transform(4, 4) = 1;

    /// 一時代入
    right(1) = vector(1);
    right(2) = vector(2);
    right(3) = vector(3);
    right(4) = 1;

    /// 変換
    left = transform * right;

    /// 代入し直し
    result(1) = left(1);
    result(2) = left(2);
    result(3) = left(3);

    return result;

}

/// 胴体ローカルからグローバルへ
/// 同時座標変換行列を変換する
inline Math::Matrix WalkingRobot::transformationLocalToGlobal(const Math::Matrix& matrix)
{
    assert(matrix.getRow() == Const::DH_DIMENSION && matrix.getColumn() == Const::DH_DIMENSION);

    Math::Matrix result(Const::DH_DIMENSION, Const::DH_DIMENSION);

    result = bodyData.transformation * matrix;

    return result;
}

/// 胴体ローカルからグローバルへ
/// 3次元位置ベクトルを変換する
inline Math::Vector WalkingRobot::transformationLocalToGlobal(const Math::Vector& vector)
{
    assert(vector.getSize() == Const::THREE_DIMENSION);

    Math::Vector result(Const::THREE_DIMENSION);
    Math::Vector left(Const::DH_DIMENSION);
    Math::Vector right(Const::DH_DIMENSION);

    /// 一時代入
    right(1) = vector(1);
    right(2) = vector(2);
    right(3) = vector(3);
    right(4) = 1;

    /// 変換
    left = bodyData.transformation * right;

    /// 代入し直し
    result(1) = left(1);
    result(2) = left(2);
    result(3) = left(3);

    return result;
}

inline Math::Matrix WalkingRobot::rollY(double angle)
{
    /// 戻り値の行列
    Math::Matrix A(Const::DH_DIMENSION, Const::DH_DIMENSION);

    A(1, 1) = cos(angle);
    A(2, 1) = 0;
    A(3, 1) = -sin(angle);
    A(4, 1) = 0;

    A(1, 2) = 0;
    A(2, 2) = 1;
    A(3, 2) = 0;
    A(4, 2) = 0;

    A(1, 3) = sin(angle);
    A(2, 3) = 0;
    A(3, 3) = cos(angle);
    A(4, 3) = 0;

    A(1, 4) = 0;
    A(2, 4) = 0;
    A(3, 4) = HG;
    A(4, 4) = 1;

    return A;
}

}	/// emd of namespace Asura
#endif /// __WalkingRobot_h__