#pragma once
#include <stdint.h>
#include <windows.h>

namespace bristol
{
  class TimeDuration;

  class TimeStamp
  {
  public:
    TimeStamp() { _timestamp.QuadPart = 0; }
    static TimeStamp Now();
    bool IsValid() const { return _timestamp.QuadPart > 0; }

    friend TimeDuration operator-(const TimeStamp& lhs, const TimeStamp& rhs);
    friend TimeStamp operator+(const TimeStamp& lhs, const TimeDuration& rhs);

    friend bool operator>(const TimeStamp& lhs, const TimeStamp& rhs);

  private:
    TimeStamp(const uint64_t& t) { _timestamp.QuadPart = t; }
    LARGE_INTEGER _timestamp;
  };

  class TimeDuration
  {
  public:
    TimeDuration() { _timestamp.QuadPart = 0; }
    TimeDuration(const uint64_t& t) { _timestamp.QuadPart = t; }

    static TimeDuration Seconds(uint64_t x);
    static TimeDuration Milliseconds(uint64_t x);
    static TimeDuration Microseconds(uint64_t x);
    static TimeDuration Nanoseconds(uint64_t x);

    uint64_t TotalSeconds() const;
    uint64_t TotalMicroseconds() const;
    uint64_t TotalMilliseconds() const;
    uint64_t TotalNanoseconds() const;

    friend bool operator<(const TimeDuration& lhs, const TimeDuration& rhs);
    friend bool operator<=(const TimeDuration& lhs, const TimeDuration& rhs);
    friend bool operator==(const TimeDuration& lhs, const TimeDuration& rhs);
    friend bool operator>(const TimeDuration& lhs, const TimeDuration& rhs);
    friend bool operator>=(const TimeDuration& lhs, const TimeDuration& rhs);

    friend TimeDuration operator-(const TimeStamp& lhs, const TimeStamp& rhs);
    friend TimeDuration operator-(const TimeDuration& lhs, const TimeDuration& rhs);
    friend TimeDuration operator+(const TimeDuration& lhs, const TimeDuration& rhs);
    friend TimeStamp operator+(const TimeStamp& lhs, const TimeDuration& rhs);
  private:

    LARGE_INTEGER _timestamp;
  };

}