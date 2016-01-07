#ifndef STICK_CONDITIONVARIABLE_HPP
#define STICK_CONDITIONVARIABLE_HPP

#include <Stick/StickError.hpp>
#include <Stick/StickErrorCodes.hpp>
#include <Stick/StickScopedLock.hpp>

#ifdef STICK_PLATFORM_UNIX
#include <pthread.h>
#endif //STICK_PLATFORM_UNIX

namespace stick
{
    class Mutex;

    class ConditionVariable
    {
    public:

#ifdef STICK_PLATFORM_UNIX
        typedef pthread_cond_t NativeHandle;
#endif //STICK_PLATFORM_UNIX

        typedef ScopedLock<Mutex> LockType;


        ConditionVariable();
        
        ConditionVariable(const ConditionVariable & _other) = delete;
        ConditionVariable(ConditionVariable && _other) = delete;
        ConditionVariable & operator = (const ConditionVariable & _other) = delete;
        ConditionVariable & operator = (ConditionVariable && _other) = delete;

        ~ConditionVariable();

        Error notifyOne();

        Error notifyAll();

        Error wait(LockType & _lock);

        template<class F>
        Error wait(LockType & _lock, F && _predicate);

        NativeHandle nativeHandle() const;

    private:

        NativeHandle m_handle;
        bool m_bIsInitialized;
    };
}

#endif //STICK_CONDITIONVARIABLE_HPP

