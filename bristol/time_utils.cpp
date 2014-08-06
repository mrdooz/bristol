#include "time_utils.hpp"

#if _WIN32
#include <windows.h>
#else
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif


using namespace bristol;

//------------------------------------------------------------------------------
namespace
{
#ifdef _WIN32
  LARGE_INTEGER g_frequency;
  bool Init()
  {
    QueryPerformanceFrequency(&g_frequency);
    return true;
  }
#else
  mach_timebase_info_data_t g_timebaseInfo;

  bool Init()
  {
    mach_timebase_info(&g_timebaseInfo);
    return true;
  }
#endif

  bool init = Init();
}

//------------------------------------------------------------------------------
TimeStamp TimeStamp::Now()
{
  TimeStamp t;
  #ifdef _WIN32
  QueryPerformanceCounter(&t._timestamp);
  #else
  t._timestamp = mach_absolute_time();
  #endif
  return t;
}

//------------------------------------------------------------------------------
TimeDuration TimeDuration::Seconds(uint64_t x)
{
#ifdef _WIN32
  return TimeDuration(x * g_frequency.QuadPart);
#else
  return TimeDuration(x * 1e9 * g_timebaseInfo.denom / g_timebaseInfo.numer);
#endif
}

//------------------------------------------------------------------------------
TimeDuration TimeDuration::Milliseconds(uint64_t x)
{
#ifdef _WIN32
  return TimeDuration((uint64_t)(x * g_frequency.QuadPart / 1e3));
#else
  return TimeDuration(x * 1e6 * g_timebaseInfo.denom / g_timebaseInfo.numer);
#endif
}

//------------------------------------------------------------------------------
TimeDuration TimeDuration::Microseconds(uint64_t x)
{
#ifdef _WIN32
  return TimeDuration((uint64_t)(x * g_frequency.QuadPart / 1e6));
#else
  return TimeDuration(x * 1e3 * g_timebaseInfo.denom / g_timebaseInfo.numer);
#endif
}

//------------------------------------------------------------------------------
TimeDuration TimeDuration::Nanoseconds(uint64_t x)
{
#ifdef _WIN32
  return TimeDuration((uint64_t)(x * g_frequency.QuadPart / 1e9));
#else
  return TimeDuration(x * g_timebaseInfo.denom / g_timebaseInfo.numer);
#endif
}

//------------------------------------------------------------------------------
uint64_t TimeDuration::TotalSeconds() const
{
#ifdef _WIN32
  return _timestamp.QuadPart / g_frequency.QuadPart;
#else
  return _timestamp * g_timebaseInfo.numer / g_timebaseInfo.denom / 1e9;
#endif
}

//------------------------------------------------------------------------------
uint64_t TimeDuration::TotalMicroseconds() const
{
#ifdef _WIN32
  return (uint64_t)(1e3 * _timestamp.QuadPart / g_frequency.QuadPart);
#else
  return _timestamp * g_timebaseInfo.numer / g_timebaseInfo.denom / 1e6;
#endif
}

//------------------------------------------------------------------------------
uint64_t TimeDuration::TotalMilliseconds() const
{
#ifdef _WIN32
  return (uint64_t)(1e6 * _timestamp.QuadPart / g_frequency.QuadPart);
#else
  return _timestamp * g_timebaseInfo.numer / g_timebaseInfo.denom / 1e3;
#endif
}

//------------------------------------------------------------------------------
uint64_t TimeDuration::TotalNanoseconds() const
{
#ifdef _WIN32
  return (uint64_t)(1e9 * _timestamp.QuadPart / g_frequency.QuadPart);
#else
  return _timestamp * g_timebaseInfo.numer / g_timebaseInfo.denom;
#endif
}

//------------------------------------------------------------------------------
TimeStamp bristol::operator+(const TimeStamp& lhs, const TimeDuration& rhs)
{
  return TimeStamp(lhs._timestamp + rhs._timestamp);
}

//------------------------------------------------------------------------------
TimeDuration bristol::operator-(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return TimeDuration(lhs._timestamp - rhs._timestamp);
}

//------------------------------------------------------------------------------
TimeDuration bristol::operator+(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return TimeDuration(lhs._timestamp + rhs._timestamp);
}

//------------------------------------------------------------------------------
TimeDuration bristol::operator-(const TimeStamp& lhs, const TimeStamp& rhs)
{
  return TimeDuration(lhs._timestamp - rhs._timestamp);
}

//------------------------------------------------------------------------------
bool bristol::operator<(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return lhs._timestamp < rhs._timestamp;
}

//------------------------------------------------------------------------------
bool bristol::operator<=(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return lhs._timestamp <= rhs._timestamp;
}

//------------------------------------------------------------------------------
bool bristol::operator==(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return lhs._timestamp == rhs._timestamp;
}

//------------------------------------------------------------------------------
bool bristol::operator>(const TimeStamp& lhs, const TimeStamp& rhs)
{
  return lhs._timestamp > rhs._timestamp;
}

//------------------------------------------------------------------------------
bool bristol::operator>(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return lhs._timestamp > rhs._timestamp;
}

//------------------------------------------------------------------------------
bool bristol::operator>=(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return lhs._timestamp >= rhs._timestamp;
}
