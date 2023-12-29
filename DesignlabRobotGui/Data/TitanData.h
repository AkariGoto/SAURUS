/**
 *  ファイル名
 *		AsuraData.h
 *  説明
 *		TITAN Xのデータ（キネマティクス，クローラなど）
 *  日付
 *		作成日: 2007/02/12(Mon)		更新日: 2007/02/19(Mon)
 */

#ifndef __AsuraData_h__
#define __AsuraData_h__

 /**
  *	----------------------------------------------------------------------
  *		ヘッダファイルインクルード
  *	----------------------------------------------------------------------
  */
#include "..\Math\Matrix\Matrix.h"
#include "..\Kinematics\AsuraParameter.h"

namespace Data
{

/**
 *	----------------------------------------------------------------------
 *		AsuraDataクラス
 *	----------------------------------------------------------------------
 */
class AsuraData
{
    /**
     *	------------------------------------------------------------
     *		メンバ変数
     *	------------------------------------------------------------
     */
private:
    /**
     *	----------------------------------------
     *	胴体
     *	----------------------------------------
     */
     /// 同次変換行列
    Math::Matrix bodyTransformation;

    /// 胴体位置
    Math::Vector bodyPosition;

    /// 胴体速度
    Math::Vector bodyVelocity;

    /// 歩行方向ベクトル
    Math::Vector walkingDirection;

    /// 移動様式
    Asura::LocomotionStyle locomotionStyle;

    /**
     *	----------------------------------------
     *	脚
     *	----------------------------------------
     */
     /// 脚根元の同次変換行列
    Math::Matrix* legBaseTransformation;

    /// 脚関節ごとの同次変換行列
    Math::Matrix** legJointTransformation;

    /// 足位置の同次変換行列
    Math::Matrix* legFootTransformation;

    /// 脚根元の関節位置
    Math::Vector* legBasePosition;

    /// 脚の関節位置
    Math::Vector** legJointPosition;

    /// 足位置
    Math::Vector* legFootPosition;

    /// 脚の関節角度
    Math::Vector* legJointAngle;

    /// 脚の関節速度
    Math::Vector* legJointVelocity;

    /// 脚の関節トルク
    Math::Vector* legJointTorque;

    /// 脚の運動相
    Asura::LegPhase* legPhase;
    /// 足首関節角度: [6]
    Math::Vector FootJointAngle;//---Add

    /// 脚のボールねじ位置（ワイヤ変位）
    Math::Vector* legActuatorPosition;
    /**
     *	----------------------------------------
     *	クローラ
     *	----------------------------------------
     */
     /// クローラ根元の同次変換行列
    Math::Matrix* trackBaseTransformation;

    /// クローラ関節ごとの同次変換行列
    Math::Matrix** trackJointTransformation;

    /// クローラ先端の同次変換行列
    Math::Matrix* trackEndTransformation;

    /// クローラ根元位置
    Math::Vector* trackBasePosition;

    /// クローラの関節位置
    Math::Vector** trackJointPosition;

    /// クローラ先端の位置
    Math::Vector* trackEndPosition;



    /// クローラの関節角度
    Math::Vector* trackJointAngle;

    /// クローラの関節速度
    Math::Vector* trackJointVelocity;

    /// クローラの関節トルク
    Math::Vector* trackJointTorque;

    /// クローラ速度
    double* trackSpeed;

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
    AsuraData();

    /// コピーコンストラクタ
    AsuraData(const AsuraData& asuraData);

    /// デストラクタ
    virtual ~AsuraData();

    /**
   *	クラス自身を複製する
   */
    AsuraData& clone(void);

    /**
     *	----------------------------------------
     *	演算子
     *	----------------------------------------
     */
     /// 代入演算子
    virtual AsuraData& operator=(const AsuraData& asuraData);

    /// 等価演算子
    bool operator==(const AsuraData& asuraData);

    /// 非等価演算子
    bool operator!=(const AsuraData& asuraData);



    /**
     *	----------------------------------------
     *	アクセス関数
     *	----------------------------------------
     */
     /**
      *	胴体
      */
      /// 同次変換行列
    const Math::Matrix& getBodyTransformation(void) const { return bodyTransformation; }
    /// 胴体位置
    const Math::Vector& getBodyPosition(void) const { return bodyPosition; }
    /// 胴体速度
    const Math::Vector& getBodyVelocity(void) const { return bodyVelocity; }
    /// 歩行方向ベクトル
    const Math::Vector& getWalkingDirection(void) const { return walkingDirection; }
    /// 移動様式
    const Asura::LocomotionStyle getLocomotionStyle(void) const { return locomotionStyle; }

    /**
     *	脚
     */
     /// 脚根元の同次変換行列
    const Math::Matrix& getLegBaseTransformation(int legNo) const
    {
        return legBaseTransformation[legNo - 1];
    }
    /// 脚関節ごとの同次変換行列
    const Math::Matrix& getLegJointTransformation(int legNo, int jointNo) const
    {
        return legJointTransformation[legNo - 1][jointNo - 1];
    }
    /// 足位置ごとの同次変換行列
    const Math::Matrix& getLegFootTransformation(int legNo) const
    {
        return legFootTransformation[legNo - 1];
    }
    /// 脚根元の関節位置
    const Math::Vector& getLegBasePosition(int legNo) const
    {
        return legBasePosition[legNo - 1];
    }
    /// 脚の関節位置
    const Math::Vector& getLegJointPosition(int legNo, int jointNo) const
    {
        return legJointPosition[legNo - 1][jointNo - 1];
    }
    /// 足位置
    const Math::Vector& getLegFootPosition(int legNo) const
    {
        return legFootPosition[legNo - 1];
    }
    /// 脚の関節角度
    const Math::Vector& getLegJointAngle(int legNo) const
    {
        return legJointAngle[legNo - 1];
    }
    /// 脚の関節速度
    const Math::Vector& getLegJointVelocity(int legNo) const
    {
        return legJointVelocity[legNo - 1];
    }
    /// 脚の関節トルク
    const Math::Vector& getLegJointTorque(int legNo) const
    {
        return legJointTorque[legNo - 1];
    }
    /// 脚の運動相
    const Asura::LegPhase getLegPhase(int legNo) const
    {
        return legPhase[legNo - 1];
    }
    /// 足首関節角度
    /*
    const Math::Vector& getFootointAngle(int legNo) const//---Add
    {
      return FootJointAngle[legNo-1];
    }
    */
    /// 脚のボールねじ座標（ワイヤ変位）
    const Math::Vector& getLegActuatorPosition(int legNo) const
    {
        return legActuatorPosition[legNo - 1];
    }
    /**
     *	クローラ
     */
     /// クローラ根元の同次変換行列
    const Math::Matrix& getTrackBaseTransformation(int trackNo) const
    {
        return trackBaseTransformation[trackNo - 1];
    }
    /// クローラ関節ごとの同次変換行列
    const Math::Matrix& getTrackJointTransformation(int trackNo, int jointNo) const
    {
        return trackJointTransformation[trackNo - 1][jointNo - 1];
    }
    /// クローラ先端の同次変換行列
    const Math::Matrix& getTrackEndTransformation(int trackNo) const
    {
        return trackEndTransformation[trackNo - 1];
    }
    /// クローラ根元位置
    const Math::Vector& getTrackBasePosition(int trackNo) const
    {
        return trackBasePosition[trackNo - 1];
    }
    /// クローラの関節位置
    const Math::Vector& getTrackJointPosition(int trackNo, int jointNo) const
    {
        return trackJointPosition[trackNo - 1][jointNo - 1];
    }
    /// クローラ先端の位置
    const Math::Vector& getTrackEndPosition(int trackNo) const
    {
        return trackEndPosition[trackNo - 1];
    }
    /// クローラの関節角度
    const Math::Vector& getTrackJointAngle(int trackNo) const
    {
        return trackJointAngle[trackNo - 1];
    }
    /// クローラの関節速度
    const Math::Vector& getTrackJointVelocity(int trackNo) const
    {
        return trackJointVelocity[trackNo - 1];
    }
    /// クローラの関節トルク
    const Math::Vector& getTrackJointTorque(int trackNo) const
    {
        return trackJointTorque[trackNo - 1];
    }
    /// クローラ速度
    double getTrackSpeed(int trackNo) const
    {
        return trackSpeed[trackNo - 1];
    }

    /**
     *	----------------------------------------
     *	セット関数
     *	----------------------------------------
     */
     /**
      *		胴体
      */
      /// 同次変換行列
    void setBodyTransformation(const Math::Matrix& transformation)
    {
        bodyTransformation = transformation;
    }
    /// 胴体位置
    void setBodyPosition(const Math::Vector& position)
    {
        bodyPosition = position;
    }
    /// 胴体速度
    void setBodyVelocity(const Math::Vector& velocity)
    {
        bodyVelocity = velocity;
    }
    /// 歩行方向ベクトル
    void setWalkingDirection(const Math::Vector& direction)
    {
        walkingDirection = direction;
    }
    /// 移動様式
    void setLocomotionStyle(Asura::LocomotionStyle style)
    {
        locomotionStyle = style;
    }

    /**
     *	脚
     */
     /// 脚根元の同次変換行列
    void setLegBaseTransformation(int legNo, const Math::Matrix& transformation)
    {
        legBaseTransformation[legNo - 1] = transformation;
    }
    /// 脚関節ごとの同次変換行列
    void setLegJointTransformation(int legNo, int jointNo, const Math::Matrix& transformation)
    {
        legJointTransformation[legNo - 1][jointNo - 1] = transformation;
    }
    /// 足位置ごとの同次変換行列
    void setLegFootTransformation(int legNo, const Math::Matrix& transformation)
    {
        legFootTransformation[legNo - 1] = transformation;
    }
    /// 脚根元の関節位置
    void setLegBasePosition(int legNo, const Math::Vector& position)
    {
        legBasePosition[legNo - 1] = position;
    }
    /// 脚の関節位置
    void setLegJointPosition(int legNo, int jointNo, const Math::Vector& position)
    {
        legJointPosition[legNo - 1][jointNo - 1] = position;
    }
    /// 足の位置
    void setLegFootPosition(int legNo, const Math::Vector& position)
    {
        legFootPosition[legNo - 1] = position;
    }
    /// 脚の関節角度
    void setLegJointAngle(int legNo, const Math::Vector& angle)
    {
        legJointAngle[legNo - 1] = angle;
    }
    /// 脚の関節速度
    void setLegJointVelocity(int legNo, const Math::Vector& velocity)
    {
        legJointVelocity[legNo - 1] = velocity;
    }
    /// 脚の関節トルク
    void setLegJointTorque(int legNo, const Math::Vector& torque)
    {
        legJointTorque[legNo - 1] = torque;
    }
    /// 脚の運動相
    void setLegPhase(int legNo, Asura::LegPhase phase)
    {
        legPhase[legNo - 1] = phase;
    }
    ///足首の関節角度
    //*
    void setFootJointAngle(int legNo, const double JointAngle)//-----Add
    {
        //FootJointAngle[legNo-1] = (Math::Vector)JointAngle;
        FootJointAngle[legNo - 1] = JointAngle;
    }//*/
    /// 脚のアクチュエータ座標
    void setLegActuatorPosition(int legNo, const Math::Vector& ActPos)
    {
        legActuatorPosition[legNo - 1] = ActPos;
    }
    /**
     *	クローラ
     */
     /// クローラ根元の同次変換行列
    void setTrackBaseTransformation(int trackNo, const Math::Matrix& transformation)
    {
        trackBaseTransformation[trackNo - 1] = transformation;
    }
    /// クローラ関節ごとの同次変換行列
    void setTrackJointTransformation(int trackNo, int jointNo, const Math::Matrix& transformation)
    {
        trackJointTransformation[trackNo - 1][jointNo - 1] = transformation;
    }
    /// クローラ先端の同次変換行列
    void setTrackEndTransformation(int trackNo, const Math::Matrix& transformation)
    {
        trackEndTransformation[trackNo - 1] = transformation;
    }
    /// クローラ根元位置
    void setTrackBasePosition(int trackNo, const Math::Vector& position)
    {
        trackBasePosition[trackNo - 1] = position;
    }
    /// クローラの関節位置
    void setTrackJointPosition(int trackNo, int jointNo, const Math::Vector& position)
    {
        trackJointPosition[trackNo - 1][jointNo - 1] = position;
    }
    /// クローラ先端の位置
    void setTrackEndPosition(int trackNo, const Math::Vector& position)
    {
        trackEndPosition[trackNo - 1] = position;
    }
    /// クローラの関節角度
    void setTrackJointAngle(int trackNo, const Math::Vector& angle)
    {
        trackJointAngle[trackNo - 1] = angle;
    }
    /// クローラの関節速度
    void setTrackJointVelocity(int trackNo, const Math::Vector& velocity)
    {
        trackJointVelocity[trackNo - 1] = velocity;
    }
    /// クローラの関節トルク
    void setTrackJointTorque(int trackNo, const Math::Vector& torque)
    {
        trackJointTorque[trackNo - 1] = torque;
    }
    /// クローラ速度
    void setTrackSpeed(int trackNo, double speed)
    {
        trackSpeed[trackNo - 1] = speed;
    }

private:
    /// オブジェクトのメモリ領域を確保する
    void newAsuraData(void);

    /// オブジェクトのメモリ領域を解放する
    void deleteAsuraData(void);

    /// コピーコンストラクタと代入演算子のヘルプ関数
    void copy(const AsuraData& asuraData);

};	/// end of class AsuraData

}	/// end of namespace Data

#endif /// __AsuraData_h__