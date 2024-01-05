
#ifndef DESIGNLABROBOTGUI_SYSTEM_CRITICAL_SECTION_H_
#define DESIGNLABROBOTGUI_SYSTEM_CRITICAL_SECTION_H_

#include "System/lock_object.h"


namespace designlab_robot_gui::system
{

//! @class CriticalSection
//! @brief クリティカルセクションクラス．
//! @details
//! ILockObject クラスを継承している．
//! 継承前のクラスの関数をオーバーライドした場合は override を付ける．
//! final であるため，このクラスをさらに継承することはできない．
class CriticalSection final : public ILockObject
{
public:
    CriticalSection();  //!< コンストラクタ．

    ~CriticalSection();  //!< デストラクタ．


    void Lock() override;

    void Unlock() override;

    bool IsLockedByCurrentThread() const;

private:
    //! 無効なスレッドのID．
    static constexpr unsigned int kInvalidThreadId = 0xffffffff;


    CRITICAL_SECTION critical_section_;  //!< クリティカルセクションオブジェクト．

    int lock_count_;  //!< ロックした回数のカウンタ．

    unsigned int thread_id_;  //!< ロックするスレッドのID．
};

}  // namespace designlab_robot_gui::system


#endif  // DESIGNLABROBOTGUI_SYSTEM_CRITICAL_SECTION_H_
