
#ifndef DESIGNLABROBOTGUI_DATA_DATAHANDLER_H_
#define DESIGNLABROBOTGUI_DATA_DATAHANDLER_H_

#include "Data/asura_data.h"
#include "Data/plan_data.h"
#include "Kinematics/AsuraParameter.h"
#include "Kinematics/AsuraX.h"
#include "Plan/Planner.h"
#include "Plan/PlanParameter.h"


namespace designlab_robot_gui::data
{

class DataHandler final
{
    using AsuraX = Asura::AsuraX;
    using Planner = Plan::Planner;
    using Matrix = Math::Matrix;
    using Vector = Math::Vector;

public:
    //! デフォルトコンストラクタは使用禁止，必ず以下の引数付きコンストラクタを使用すること
    DataHandler() = delete;

    //! コンストラクタ
    //! @param[in] asuraPtr データを受け取る AsuraX クラスへのポインタ
    //! @param[in] asuraDataPtr データを渡す AsuraData クラスへのポインタ
    //! @param[in] planPtr データを受け取る Planner クラスへのポインタ
    //! @param[in] planDataPtr データを渡す planDataPtr クラスへのポインタ
    explicit DataHandler(AsuraX* asuraPtr,
                         AsuraData* asuraDataPtr,
                         Planner* planPtr,
                         PlanData* planDataPtr);

    ~DataHandler();

    /// 取得
    void acquireAsuraDataSource(AsuraX* asuraPointer);
    /// 解放
    void releaseAsuraDataSource();

    /// 取得
    void acquirePlanDataSource(Planner* planPointer);
    /// 解放
    void releasePlanDataSource();

    /// 取得
    void acquireAsuraDataTarget(AsuraData* dataPointer);
    /// 解放
    void releaseAsuraDataTarget();

    /// 取得
    void acquirePlanDataTarget(PlanData* dataPointer);
    /// 解放
    void releasePlanDataTarget();

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
     *	キネマティクスデータ
     */
    const AsuraData& getAsuraData() const { return asuraData; }

    /**
     *	胴体
     */
     /// 同次変換行列
    const Matrix& getBodyTransformation() const { return asuraData.body_transformation; }

    /// 胴体位置
    const Vector& getBodyPosition() const { return asuraData.body_position; }

    /// 胴体速度
    const Vector& getBodyVelocity() const { return asuraData.body_velocity; }

    /**
     *	脚
     */
     /// 脚根元ごとの同次変換行列
    const Matrix& getLegBaseTransformation(int legNo) const;

    /// 脚関節ごとの同次変換行列
    const Matrix& getLegJointTransformation(int legNo, int jointNo) const;

    /// 足位置ごとの同次変換行列
    const Matrix& getLegFootTransformation(int legNo) const;

    /// 脚の根元位置
    const Vector& getLegBasePosition(int legNo) const;

    /// 脚の関節位置
    const Vector& getLegJointPosition(int legNo, int jointNo) const;

    /// 足位置
    const Vector& getLegFootPosition(int legNo) const;

    /// 脚の関節角度
    const Vector& getLegJointAngle(int legNo) const;

    /// 脚の関節速度
    const Vector& getLegJointVelocity(int legNo) const;

    /// 脚の関節トルク
    const Vector& getLegJointTorque(int legNo) const;

    /// 脚の運動相
    const LegPhase getLegPhase(int legNo) const;



    /**
     *	計画データ
     */
    const PlanData& getPlanData() const { return planData; }

    const double getElapsedTime() const { return planData.elapsed_time; }

private:

    /// キネマティクスデータ
    AsuraX* asuraDataSourcePointer;
    /// 計画データ
    Planner* planDataSourcePointer;


    /**
     *	データを保存するデータへのポインタ
     */
    AsuraData* asuraDataTargetPointer;
    PlanData* planDataTargetPointer;


    // 保持データ

    // キネマティクス
    AsuraData asuraData;
    /// 計画
    PlanData planData;
};


}  // namespace designlab_robot_gui::data


#endif  // DESIGNLABROBOTGUI_DATA_DATAHANDLER_H_
