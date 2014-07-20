#include "time_utils.hpp"
#include <windows.h>

using namespace bristol;

//------------------------------------------------------------------------------
namespace
{
  LARGE_INTEGER g_frequency;
  bool Init()
  {
    QueryPerformanceFrequency(&g_frequency);
    return true;
  }

  bool init = Init();
}

//------------------------------------------------------------------------------
TimeStamp TimeStamp::Now()
{
  TimeStamp t;
  QueryPerformanceCounter(&t._timestamp);
  return t;
}

//------------------------------------------------------------------------------
TimeDuration TimeDuration::Seconds(uint64_t x)
{
  return TimeDuration(x * g_frequency.QuadPart);
}

//------------------------------------------------------------------------------
TimeDuration TimeDuration::Milliseconds(uint64_t x)
{
  return TimeDuration((uint64_t)(x * g_frequency.QuadPart / 1e3));
}

//------------------------------------------------------------------------------
TimeDuration TimeDuration::Microseconds(uint64_t x)
{
  return TimeDuration((uint64_t)(x * g_frequency.QuadPart / 1e6));
}

//------------------------------------------------------------------------------
TimeDuration TimeDuration::Nanoseconds(uint64_t x)
{
  return TimeDuration((uint64_t)(x * g_frequency.QuadPart / 1e9));
}

//------------------------------------------------------------------------------
uint64_t TimeDuration::TotalSeconds() const
{
  return _timestamp.QuadPart / g_frequency.QuadPart;
}

//------------------------------------------------------------------------------
uint64_t TimeDuration::TotalMicroseconds() const
{
  return (uint64_t)(1e3 * _timestamp.QuadPart / g_frequency.QuadPart);
}

//------------------------------------------------------------------------------
uint64_t TimeDuration::TotalMilliseconds() const
{
  return (uint64_t)(1e6 * _timestamp.QuadPart / g_frequency.QuadPart);
}

//------------------------------------------------------------------------------
uint64_t TimeDuration::TotalNanoseconds() const
{
  return (uint64_t)(1e9 * _timestamp.QuadPart / g_frequency.QuadPart);
}

//------------------------------------------------------------------------------
TimeStamp bristol::operator+(const TimeStamp& lhs, const TimeDuration& rhs)
{
  return TimeStamp(lhs._timestamp.QuadPart + rhs._timestamp.QuadPart);
}

//------------------------------------------------------------------------------
TimeDuration bristol::operator-(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return TimeDuration(lhs._timestamp.QuadPart - rhs._timestamp.QuadPart);
}

//------------------------------------------------------------------------------
TimeDuration bristol::operator+(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return TimeDuration(lhs._timestamp.QuadPart + rhs._timestamp.QuadPart);
}

//------------------------------------------------------------------------------
TimeDuration bristol::operator-(const TimeStamp& lhs, const TimeStamp& rhs)
{
  return TimeDuration(lhs._timestamp.QuadPart - rhs._timestamp.QuadPart);
}

//------------------------------------------------------------------------------
bool bristol::operator<(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return lhs._timestamp.QuadPart < rhs._timestamp.QuadPart;
}

//------------------------------------------------------------------------------
bool bristol::operator<=(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return lhs._timestamp.QuadPart <= rhs._timestamp.QuadPart;
}

//------------------------------------------------------------------------------
bool bristol::operator==(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return lhs._timestamp.QuadPart == rhs._timestamp.QuadPart;
}

//------------------------------------------------------------------------------
bool bristol::operator>(const TimeStamp& lhs, const TimeStamp& rhs)
{
  return lhs._timestamp.QuadPart > rhs._timestamp.QuadPart;
}

//------------------------------------------------------------------------------
bool bristol::operator>(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return lhs._timestamp.QuadPart > rhs._timestamp.QuadPart;
}

//------------------------------------------------------------------------------
bool bristol::operator>=(const TimeDuration& lhs, const TimeDuration& rhs)
{
  return lhs._timestamp.QuadPart >= rhs._timestamp.QuadPart;
}
