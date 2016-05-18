#include "U2Time.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
inline TimeDelta TimeDelta::FromDays(u2int64 days) 
{
    return TimeDelta(days * Time::kMicrosecondsPerDay);
}
//---------------------------------------------------------------------
inline TimeDelta TimeDelta::FromHours(u2int64 hours) 
{
    return TimeDelta(hours * Time::kMicrosecondsPerHour);
}
//---------------------------------------------------------------------
inline TimeDelta TimeDelta::FromMinutes(u2int64 minutes) 
{
    return TimeDelta(minutes * Time::kMicrosecondsPerMinute);
}
//---------------------------------------------------------------------
inline TimeDelta TimeDelta::FromSeconds(u2int64 secs) 
{
    return TimeDelta(secs * Time::kMicrosecondsPerSecond);
}
//---------------------------------------------------------------------
inline TimeDelta TimeDelta::FromMilliseconds(u2int64 ms) 
{
    return TimeDelta(ms * Time::kMicrosecondsPerMillisecond);
}
//---------------------------------------------------------------------
inline TimeDelta TimeDelta::FromMicroseconds(u2int64 us) 
{
    return TimeDelta(us);
}
//---------------------------------------------------------------------
inline Time TimeDelta::operator+(Time t) const 
{
    return Time(t.us_ + delta_);
}
//---------------------------------------------------------------------
u2int32 TimeDelta::InDays() const
{
    return static_cast<u2int32>(delta_ / Time::kMicrosecondsPerDay);
}
//---------------------------------------------------------------------
u2int32 TimeDelta::InHours() const 
{
    return static_cast<u2int32>(delta_ / Time::kMicrosecondsPerHour);
}
//---------------------------------------------------------------------
u2int32 TimeDelta::InMinutes() const 
{
    return static_cast<u2int32>(delta_ / Time::kMicrosecondsPerMinute);
}
//---------------------------------------------------------------------
double TimeDelta::InSecondsF() const 
{
    return static_cast<double>(delta_) / Time::kMicrosecondsPerSecond;
}
//---------------------------------------------------------------------
u2int64 TimeDelta::InSeconds() const 
{
    return delta_ / Time::kMicrosecondsPerSecond;
}
//---------------------------------------------------------------------
double TimeDelta::InMillisecondsF() const 
{
    return static_cast<double>(delta_) / Time::kMicrosecondsPerMillisecond;
}
//---------------------------------------------------------------------
u2int64 TimeDelta::InMilliseconds() const 
{
    return delta_ / Time::kMicrosecondsPerMillisecond;
}
//---------------------------------------------------------------------
u2int64 TimeDelta::InMillisecondsRoundedUp() const 
{
    return (delta_ + Time::kMicrosecondsPerMillisecond - 1) /
        Time::kMicrosecondsPerMillisecond;
}
//---------------------------------------------------------------------
u2int64 TimeDelta::InMicroseconds() const 
{
    return delta_;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
Time Time::FromTimeT(time_t tt) 
{
    if (tt == 0)
        return Time();  // Preserve 0 so we can tell it doesn't exist.
    return Time((tt * kMicrosecondsPerSecond) + kTimeTToMicrosecondsOffset);
}
//---------------------------------------------------------------------
time_t Time::ToTimeT() const 
{
    if (us_ == 0)
        return 0;  // Preserve 0 so we can tell it doesn't exist.
    return (us_ - kTimeTToMicrosecondsOffset) / kMicrosecondsPerSecond;
}
//---------------------------------------------------------------------
Time Time::FromDoubleT(double dt) 
{
    if (dt == 0)
        return Time();  // Preserve 0 so we can tell it doesn't exist.
    return Time(static_cast<u2int64>((dt *
        static_cast<double>(kMicrosecondsPerSecond)) +
        kTimeTToMicrosecondsOffset));
}
//---------------------------------------------------------------------
double Time::ToDoubleT() const 
{
    if (us_ == 0)
        return 0;  // Preserve 0 so we can tell it doesn't exist.
    return (static_cast<double>(us_ - kTimeTToMicrosecondsOffset) /
        static_cast<double>(kMicrosecondsPerSecond));
}
//---------------------------------------------------------------------
Time Time::UnixEpoch() 
{
    Time time;
    time.us_ = kTimeTToMicrosecondsOffset;
    return time;
}
//---------------------------------------------------------------------
Time Time::LocalMidnight() const 
{
    Exploded exploded;
    LocalExplode(&exploded);
    exploded.hour = 0;
    exploded.minute = 0;
    exploded.second = 0;
    exploded.millisecond = 0;
    return FromLocalExploded(exploded);
}
//---------------------------------------------------------------------
inline bool is_in_range(int value, int lo, int hi) 
{
    return lo <= value && value <= hi;
}
//---------------------------------------------------------------------
bool Time::Exploded::HasValidValues() const 
{
    return is_in_range(month, 1, 12) &&
        is_in_range(day_of_week, 0, 6) &&
        is_in_range(day_of_month, 1, 31) &&
        is_in_range(hour, 0, 23) &&
        is_in_range(minute, 0, 59) &&
        is_in_range(second, 0, 60) &&
        is_in_range(millisecond, 0, 999);
}