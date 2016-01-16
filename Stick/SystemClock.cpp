#include <Stick/SystemClock.hpp>
#if STICK_PLATFORM == STICK_PLATFORM_OSX
#include <sys/time.h>
#elif STICK_PLATFORM == STICK_PLATFORM_LINUX
#include <time.h>
#endif //STICK_PLATFORM

namespace stick
{
#if STICK_PLATFORM == STICK_PLATFORM_OSX || STICK_PLATFORM == STICK_PLATFORM_LINUX

    SystemClock::TimePoint SystemClock::now()
    {
        timeval val;
        gettimeofday(&val, 0);
        return TimePoint(val.tv_sec * 1000000000 + val.tv_usec * 1000);
    }
    
#endif //STICK_PLATFORM
}
