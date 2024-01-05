
#ifndef DESIGNLABROBOTGUI_SYSTEM_LOCK_OBJECT_H_
#define DESIGNLABROBOTGUI_SYSTEM_LOCK_OBJECT_H_


namespace designlab_robot_gui::system
{

//! @class ILockObject
//! @brief ロックオブジェクトのインターフェイス．
//! @details
//! インターフェースは純粋仮想関数のみを持ち，メンバ変数を持たない．
//! 純粋仮想関数は実装する必要がなく，継承先で実装する．
//! そのため，このクラスのインスタンスを生成することはできず，必ず継承して使用する．
class ILockObject
{
public:
    ILockObject() = default;  //!< デフォルトコンストラクタはデフォルト(何もしない)．
    ILockObject(const ILockObject& copy) = delete;  //!< コピーコンストラクタは禁止．
    ILockObject(const ILockObject&& copy) = delete;  //!< ムーブコンストラクタは禁止．
    ILockObject& operator =(const ILockObject& copy) = delete;  //!< 代入演算子は禁止．

    ~ILockObject() = default;  //!< デストラクタはデフォルト(何もしない)．

    //! @brief ロックを行う．
    virtual void Lock() = 0;

    //! @brief ロックを解除する．
    virtual void Unlock() = 0;

    //! @brief 現行スレッドによりロックされているかを返す．
    //! @retval true ロックされている．
    //! @retval false ロックされていない．
    virtual bool IsLockedByCurrentThread() const = 0;
};

}  // namespace designlab_robot_gui::system


#endif  // DESIGNLABROBOTGUI_SYSTEM_LOCK_OBJECT_H_
