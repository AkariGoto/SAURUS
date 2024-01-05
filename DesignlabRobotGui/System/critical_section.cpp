
#include "pch.h"

#include "System/critical_section.h"


namespace designlab_robot_gui::system
{

CriticalSection::CriticalSection() :
    lock_count_(0),
    thread_id_(kInvalidThreadId)
{
    // クリティカルセクションオブジェクトの初期化
    ::InitializeCriticalSection(&critical_section_);
}

CriticalSection::~CriticalSection()
{
    assert(thread_id_ == kInvalidThreadId);
    assert(lock_count_ == 0);

    // クリティカルセクションオブジェクトの放棄(後処理)
    ::DeleteCriticalSection(&critical_section_);
}

void CriticalSection::Lock()
{
    ::EnterCriticalSection(&critical_section_);
    lock_count_++;
    thread_id_ = ::GetCurrentThreadId();
}

void CriticalSection::Unlock()
{
    // ロックされていないのにアンロックしようとした場合はエラー
    assert(0 < lock_count_);

    lock_count_--;  // ロックカウントをデクリメント(1減算)

    if (lock_count_ == 0)
    {
        thread_id_ = kInvalidThreadId;
    }

    ::LeaveCriticalSection(&critical_section_);
}

bool CriticalSection::IsLockedByCurrentThread() const
{
    return (::GetCurrentThreadId() == thread_id_);
}

}  // namespace designlab_robot_gui::system
