
#ifndef DESIGNLABROBOTGUI_SYSTEM_OBJECT_LOCKER_H_
#define DESIGNLABROBOTGUI_SYSTEM_OBJECT_LOCKER_H_

#include "System/lock_object.h"


namespace designlab_robot_gui::system
{

//! @class ObjectLocker
//! @brief オブジェクトロッカークラス．
//! @details
//! ILockObject クラスをコンストラクタで受け取り，ロックする．
//! そして，デストラクタでアンロックする．
class ObjectLocker final
{
public:
    //!< デフォルトコンストラクタは削除，必ず引数付きで呼び出す．
    ObjectLocker() = delete;

    explicit ObjectLocker(ILockObject* lock_object_ptr);  //!< コンストラクタ．

    ~ObjectLocker();  //!< デストラクタ．


private:
    ILockObject* lock_object_ptr_;  //!< ロックオブジェクトのポインタ．
};

}  // namespace designlab_robot_gui::system


#endif  // DESIGNLABROBOTGUI_SYSTEM_OBJECT_LOCKER_H_
