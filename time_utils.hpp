#pragma once
#include <stdint.h>

namespace bristol
{
  class TimeDuration;

  class TimeStamp
  {
  public:
    TimeStamp() : _timestamp(0) { }
    static TimeStamp Now();
    bool IsValid() const { return _timestamp > 0; }

    friend TimeDuration operator-(const TimeStamp& lhs, const TimeStamp& rhs);
    friend TimeStamp operator+(const TimeStamp& lhs, const TimeDuration& rhs);
    TimeStamp& operator+=(const TimeDuration& rhs);

    friend bool operator>(const TimeStamp& lhs, const TimeStamp& rhs);

  private:
    TimeStamp(const uint64_t& t) { _timestamp = t; }

    // The timestamp is in platform dependent ticks
    uint64_t _timestamp;
  };

  class TimeDuration
  {
    friend class TimeStamp;
  public:
    TimeDuration() { _timestamp = 0; }
    TimeDuration(const uint64_t& t) { _timestamp = t; }

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

    uint64_t _timestamp;
  };

}
