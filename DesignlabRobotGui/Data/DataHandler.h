
#ifndef DESIGNLABROBOTGUI_DATA_DATAHANDLER_H_
#define DESIGNLABROBOTGUI_DATA_DATAHANDLER_H_


#include "Kinematics\AsuraX.h"
#include "Plan\Planner.h"
#include "AsuraData.h"
#include "PlanData.h"
#include "Kinematics\AsuraParameter.h"
#include "Plan\PlanParameter.h"

namespace Data
{

class DataHandler final
{
public:
    DataHandler() = delete;  //!< デフォルトコンストラクタは使用禁止，必ず以下の引数付きコンストラクタを使用すること

    //! コンストラクタ
    //! @param[in] asuraPtr データを受け取る AsuraX クラスへのポインタ
    //! @param[in] asuraDataPtr データを渡す AsuraData クラスへのポインタ
    //! @param[in] planPtr データを受け取る Planner クラスへのポインタ
    //! @param[in] planDataPtr データを渡す planDataPtr クラスへのポインタ
    explicit DataHandler(Asura::AsuraX* asuraPtr,
                         Data::AsuraData* asuraDataPtr,
                         Plan::Planner* planPtr,
                         Data::PlanData* planDataPtr);

    ~DataHandler();

    /**
     *	----------------------------------------
     *	データ元のポインタの取得・解放
     *	----------------------------------------
     */
     /// 取得
    void acquireAsuraDataSource(Asura::AsuraX* asuraPointer);
    /// 解放
    void releaseAsuraDataSource();

    /// 取得
    void acquirePlanDataSource(Plan::Planner* planPointer);
    /// 解放
    void releasePlanDataSource();

    /**
     *	----------------------------------------
     *	データ先のポインタの取得・解放
     *	----------------------------------------
     */
     /// 取得
    void acquireAsuraDataTarget(Data::AsuraData* dataPointer);
    /// 解放
    void releaseAsuraDataTarget();

    /// 取得
    void acquirePlanDataTarget(Data::PlanData* dataPointer);
    /// 解放
    void releasePlanDataTarget();

    /**
     *	----------------------------------------
     *	データの取り込みと転送
     *	----------------------------------------
     */
     /// 転送
    void exportAsuraData();
    /// 取り込み
    void importAsuraData();

    /// 転送
    void exportPlanData();
    /// 取り込み
    void importPlanData();


    /**
     *	データ渡し
     */
    void handleAsuraData();
    void handlePlanData();

    /**
     *	----------------------------------------
     *	アクセス関数
     *	----------------------------------------
     */
     /**
      *	キネマティクスデータ
      */
    const Data::AsuraData& getAsuraData(void) const { return asuraData; }

    /**
     *	胴体
     */
     /// 同次変換行列
    const Math::Matrix& getBodyTransformation(void) const { return asuraData.getBodyTransformation(); }

    /// 胴体位置
    const Math::Vector& getBodyPosition(void) const { return asuraData.getBodyPosition(); }

    /// 胴体速度
    const Math::Vector& getBodyVelocity(void) const { return asuraData.getBodyVelocity(); }

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
     *	計画データ
     */
    const Data::PlanData& getPlanData(void) const { return planData; }

    const double getElapsedTime(void) const { return planData.getElapsedTime(); }

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
};


}  // namespace Data

#endif  // DESIGNLABROBOTGUI_DATA_DATAHANDLER_H_
