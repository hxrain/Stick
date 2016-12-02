#ifndef STICK_UTILITY_HPP
#define STICK_UTILITY_HPP

#include <Stick/Iterator.hpp>
#include <utility>
#include <type_traits>

namespace stick
{
    template<class T>
    struct RemoveReference
    {
        typedef T Type;
    };

    template<class T>
    struct RemoveReference<T &>
    {
        typedef T Type;
    };

    template<class T>
    struct RemoveReference < T && >
    {
        typedef T Type;
    };

    template<typename T>
    struct EnableBitmaskOperators
    {
        static constexpr bool Value = false;
    };

    template<typename T>
    typename std::enable_if<EnableBitmaskOperators<T>::Value, T>::type
    operator|(T _a, T _b)
    {
        typedef typename std::underlying_type<T>::type underlying;
        return static_cast<T>(static_cast<underlying>(_a) | static_cast<underlying>(_b));
    }

    template<class T>
    inline T min(const T & _a, const T & _b)
    {
        return _a < _b ? _a : _b;
    }

    template<class T>
    inline T max(const T & _a, const T & _b)
    {
        return _a > _b ? _a : _b;
    }

    template<class F>
    class ScopeExit
    {
    public:

        ScopeExit(F && _func) :
            m_function(move(_func)),
            m_bCallInDestructor(true)
        {

        }

        ScopeExit(ScopeExit && _ex) :
            m_function(move(_ex.m_function)),
            m_bCallInDestructor(move(_ex.m_bCallInDestructor))
        {

        }

        ~ScopeExit()
        {
            if (m_bCallInDestructor)
                m_function();
        }

        void cancel()
        {
            m_bCallInDestructor = false;
        }

    private:

        ScopeExit(const ScopeExit &) = delete;
        void operator=(const ScopeExit &) = delete;
        ScopeExit & operator=(ScopeExit &&) = delete;

        F m_function;
        bool m_bCallInDestructor;
    };

    template<class F>
    ScopeExit<typename RemoveReference<F>::Type> makeScopeExit(F && _f)
    {
        return ScopeExit<typename RemoveReference<F>::Type>(std::forward<F>(_f));
    }

    template<class InputIter, class T>
    InputIter find(InputIter _first, InputIter _last, const T & _value)
    {
        for (; _first != _last; ++_first)
        {
            if (*_first == _value)
            {
                return _first;
            }
        }
        return _last;
    }

    template<class InputIter, class Pred>
    InputIter findIf(InputIter _first, InputIter _last, Pred _p)
    {
        for (; _first != _last; ++_first)
        {
            if (_p(*_first))
            {
                return _first;
            }
        }
        return _last;
    }
}

#endif //STICK_UTILITY_HPP
