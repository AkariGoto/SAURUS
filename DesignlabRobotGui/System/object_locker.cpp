
#include "pch.h"

#include "System/object_locker.h"


namespace designlab_robot_gui::system
{

ObjectLocker::ObjectLocker(ILockObject* lock_object_ptr) :
    lock_object_ptr_(lock_object_ptr)
{
    if (lock_object_ptr_ != nullptr)
    {
        lock_object_ptr_->Lock();
    }
}

ObjectLocker::~ObjectLocker()
{
    if (lock_object_ptr_ != nullptr)
    {
        lock_object_ptr_->Unlock();
    }
}

}  // namespace designlab_robot_gui::system
