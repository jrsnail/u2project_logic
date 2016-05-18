#ifndef __U2Time_H__
#define __U2Time_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN



// Time represents an absolute point in time, internally represented as
// microseconds (s/1,000,000) since a platform-dependent epoch.  Each
// platform's epoch, along with other system-dependent clock interface
// routines, is defined in time_PLATFORM.cc.
//
// TimeDelta represents a duration of time, internally represented in
// microseconds.
//
// TimeTicks represents an abstract time that is always incrementing for use
// in measuring time durations. It is internally represented in microseconds.
// It can not be converted to a human-readable time, but is guaranteed not to
// decrease (if the user changes the computer clock, Time::Now() may actually
// decrease or jump).
//
// These classes are represented as only a 64-bit value, so they can be
// efficiently passed by value.
class _U2Export TimeDelta : public Object, public GeneralAllocatedObject
{
public:
    TimeDelta() 
        : delta_(0) 
    {
    }

    // Converts units of time to TimeDeltas.
    static inline TimeDelta FromDays(u2int64 days);
    static inline TimeDelta FromHours(u2int64 hours);
    static inline TimeDelta FromMinutes(u2int64 minutes);
    static inline TimeDelta FromSeconds(u2int64 secs);
    static inline TimeDelta FromMilliseconds(u2int64 ms);
    static inline TimeDelta FromMicroseconds(u2int64 us);

    // Converts an integer value representing TimeDelta to a class. This is used
    // when deserializing a |TimeDelta| structure, using a value known to be
    // compatible. It is not provided as a constructor because the integer type
    // may be unclear from the perspective of a caller.
    static TimeDelta FromInternalValue(u2int64 delta) 
    {
        return TimeDelta(delta);
    }

    // Returns the internal numeric value of the TimeDelta object. Please don't
    // use this and do arithmetic on it, as it is more error prone than using the
    // provided operators.
    // For serializing, use FromInternalValue to reconstitute.
    u2int64 ToInternalValue() const 
    {
        return delta_;
    }

    // Returns the time delta in some unit. The F versions return a floating
    // point value, the "regular" versions return a rounded-down value.
    //
    // InMillisecondsRoundedUp() instead returns an integer that is rounded up
    // to the next full millisecond.
    inline u2int32 InDays() const;
    inline u2int32 InHours() const;
    inline u2int32 InMinutes() const;
    inline double InSecondsF() const;
    inline u2int64 InSeconds() const;
    inline double InMillisecondsF() const;
    inline u2int64 InMilliseconds() const;
    inline u2int64 InMillisecondsRoundedUp() const;
    inline u2int64 InMicroseconds() const;

    TimeDelta& operator=(TimeDelta other) 
    {
        delta_ = other.delta_;
        return *this;
    }

    // Computations with other deltas.
    TimeDelta operator+(TimeDelta other) const 
    {
        return TimeDelta(delta_ + other.delta_);
    }
    TimeDelta operator-(TimeDelta other) const 
    {
        return TimeDelta(delta_ - other.delta_);
    }
    TimeDelta& operator+=(TimeDelta other) 
    {
        delta_ += other.delta_;
        return *this;
    }
    TimeDelta& operator-=(TimeDelta other) 
    {
        delta_ -= other.delta_;
        return *this;
    }
    TimeDelta operator-() const 
    {
        return TimeDelta(-delta_);
    }

    // Computations with ints, note that we only allow multiplicative operations
    // with ints, and additive operations with other deltas.
    TimeDelta operator*(u2int64 a) const 
    {
        return TimeDelta(delta_ * a);
    }
    TimeDelta operator/(u2int64 a) const 
    {
        return TimeDelta(delta_ / a);
    }
    TimeDelta& operator*=(u2int64 a) 
    {
        delta_ *= a;
        return *this;
    }
    TimeDelta& operator/=(u2int64 a) 
    {
        delta_ /= a;
        return *this;
    }
    u2int64 operator/(TimeDelta a) const 
    {
        return delta_ / a.delta_;
    }

    // Defined below because it depends on the definition of the other classes.
    Time operator+(Time t) const;
    TimeTicks operator+(TimeTicks t) const;

    // Comparison operators.
    bool operator==(TimeDelta other) const 
    {
        return delta_ == other.delta_;
    }
    bool operator!=(TimeDelta other) const 
    {
        return delta_ != other.delta_;
    }
    bool operator<(TimeDelta other) const 
    {
        return delta_ < other.delta_;
    }
    bool operator<=(TimeDelta other) const 
    {
        return delta_ <= other.delta_;
    }
    bool operator>(TimeDelta other) const 
    {
        return delta_ > other.delta_;
    }
    bool operator>=(TimeDelta other) const 
    {
        return delta_ >= other.delta_;
    }

private:
    // Constructs a delta given the duration in microseconds. This is private
    // to avoid confusion by callers with an integer constructor. Use
    // FromSeconds, FromMilliseconds, etc. instead.
    explicit TimeDelta(u2int64 delta_us) 
        : delta_(delta_us) 
    {
    }

    friend TimeDelta operator*(u2int64 a, TimeDelta td);

private:
    friend class Time;
    friend class TimeTicks;
    // Delta in microseconds.
    u2int64 delta_;
};

inline TimeDelta operator*(u2int64 a, TimeDelta td) 
{
    return TimeDelta(a * td.delta_);
}


class _U2Export Time
{
public:
    static const u2int64 kMillisecondsPerSecond = 1000;
    static const u2int64 kMicrosecondsPerMillisecond = 1000;
    static const u2int64 kMicrosecondsPerSecond = kMicrosecondsPerMillisecond * kMillisecondsPerSecond;
    static const u2int64 kMicrosecondsPerMinute = kMicrosecondsPerSecond * 60;
    static const u2int64 kMicrosecondsPerHour = kMicrosecondsPerMinute * 60;
    static const u2int64 kMicrosecondsPerDay = kMicrosecondsPerHour * 24;
    static const u2int64 kMicrosecondsPerWeek = kMicrosecondsPerDay * 7;
    static const u2int64 kNanosecondsPerMicrosecond = 1000;
    static const u2int64 kNanosecondsPerSecond = kNanosecondsPerMicrosecond * kMicrosecondsPerSecond;

    // Represents an exploded time that can be formatted nicely. This is kind of
    // like the Win32 SYSTEMTIME structure or the Unix "struct tm" with a few
    // additions and changes to prevent errors.
    struct _U2Export Exploded
    {
        u2int32 year;          // Four digit year "2007"
        u2int32 month;         // 1-based month (values 1 = January, etc.)
        u2int32 day_of_week;   // 0-based day of week (0 = Sunday, etc.)
        u2int32 day_of_month;  // 1-based day of month (1-31)
        u2int32 hour;          // Hour within the current day (0-23)
        u2int32 minute;        // Minute within the current hour (0-59)
        u2int32 second;        // Second within the current minute (0-59 plus leap seconds which may take it up to 60).
        u2int32 millisecond;   // Milliseconds within the current second (0-999)

        // A cursory test for whether the data members are within their
        // respective ranges. A 'true' return value does not guarantee the
        // Exploded value can be successfully converted to a Time value.
        bool HasValidValues() const;
    };

    // Contains the NULL time. Use Time::Now() to get the current time.
    explicit Time() 
        : us_(0) 
    {
    }

    // Returns true if the time object has not been initialized.
    bool is_null() const 
    {
        return us_ == 0;
    }

    // Returns the time for epoch in Unix-like system (Jan 1, 1970).
    static Time UnixEpoch();

    // Returns the current time. Watch out, the system might adjust its clock
    // in which case time will actually go backwards. We don't guarantee that
    // times are increasing, or that two calls to Now() won't be the same.
    static Time Now();

    // Returns the current time. Same as Now() except that this function always
    // uses system time so that there are no discrepancies between the returned
    // time and system time even on virtual environments including our test bot.
    // For timing sensitive unittests, this function should be used.
    static Time NowFromSystemTime();

    // Converts to/from time_t in UTC and a Time class.
    // TODO(brettw) this should be removed once everybody starts using the |Time|
    // class.
    static Time FromTimeT(time_t tt);
    time_t ToTimeT() const;

    // Converts time to/from a double which is the number of seconds since epoch
    // (Jan 1, 1970).  Webkit uses this format to represent time.
    // Because WebKit initializes double time value to 0 to indicate "not
    // initialized", we map it to empty Time object that also means "not
    // initialized".
    static Time FromDoubleT(double dt);
    double ToDoubleT() const;


    // Converts an exploded structure representing either the local time or UTC
    // into a Time class.
    static Time FromUTCExploded(const Exploded& exploded) 
    {
        return FromExploded(false, exploded);
    }
    static Time FromLocalExploded(const Exploded& exploded) 
    {
        return FromExploded(true, exploded);
    }

    // Converts an integer value representing Time to a class. This is used
    // when deserializing a |Time| structure, using a value known to be
    // compatible. It is not provided as a constructor because the integer type
    // may be unclear from the perspective of a caller.
    static Time FromInternalValue(u2int64 us) 
    {
        return Time(us);
    }

    // For serializing, use FromInternalValue to reconstitute. Please don't use
    // this and do arithmetic on it, as it is more error prone than using the
    // provided operators.
    u2int64 ToInternalValue() const 
    {
        return us_;
    }

    // Fills the given exploded structure with either the local time or UTC from
    // this time structure (containing UTC).
    void UTCExplode(Exploded* exploded) const 
    {
        return Explode(false, exploded);
    }
    void LocalExplode(Exploded* exploded) const 
    {
        return Explode(true, exploded);
    }

    // Rounds this time down to the nearest day in local time. It will represent
    // midnight on that day.
    Time LocalMidnight() const;

    Time& operator=(Time other) 
    {
        us_ = other.us_;
        return *this;
    }

    // Compute the difference between two times.
    TimeDelta operator-(Time other) const 
    {
        return TimeDelta(us_ - other.us_);
    }

    // Modify by some time delta.
    Time& operator+=(TimeDelta delta) 
    {
        us_ += delta.delta_;
        return *this;
    }
    Time& operator-=(TimeDelta delta) 
    {
        us_ -= delta.delta_;
        return *this;
    }

    // Return a new time modified by some delta.
    Time operator+(TimeDelta delta) const 
    {
        return Time(us_ + delta.delta_);
    }
    Time operator-(TimeDelta delta) const 
    {
        return Time(us_ - delta.delta_);
    }

    // Comparison operators
    bool operator==(Time other) const 
    {
        return us_ == other.us_;
    }
    bool operator!=(Time other) const 
    {
        return us_ != other.us_;
    }
    bool operator<(Time other) const 
    {
        return us_ < other.us_;
    }
    bool operator<=(Time other) const 
    {
        return us_ <= other.us_;
    }
    bool operator>(Time other) const 
    {
        return us_ > other.us_;
    }
    bool operator>=(Time other) const 
    {
        return us_ >= other.us_;
    }

private:
    friend class TimeDelta;

    explicit Time(u2int64 us) : us_(us)
    {
    }

    // Explodes the given time to either local time |is_local = true| or UTC
    // |is_local = false|.
    void Explode(bool is_local, Exploded* exploded) const;

    // Unexplodes a given time assuming the source is either local time
    // |is_local = true| or UTC |is_local = false|.
    static Time FromExploded(bool is_local, const Exploded& exploded);

private:
    // The representation of Jan 1, 1970 UTC in microseconds since the
    // platform-dependent epoch.
    static const u2int64 kTimeTToMicrosecondsOffset;

    // Time in microseconds in UTC.
    u2int64 us_;
};


class _U2Export TimeTicks 
{
public:
    TimeTicks() 
        : ticks_(0) 
    {
    }

    // Platform-dependent tick count representing "right now."
    // The resolution of this clock is ~1-15ms.  Resolution varies depending
    // on hardware/operating system configuration.
    static TimeTicks Now();

    // Returns true if this object has not been initialized.
    bool is_null() const 
    {
        return ticks_ == 0;
    }

    // Converts an integer value representing TimeTicks to a class. This is used
    // when deserializing a |TimeTicks| structure, using a value known to be
    // compatible. It is not provided as a constructor because the integer type
    // may be unclear from the perspective of a caller.
    static TimeTicks FromInternalValue(u2int64 ticks) 
    {
        return TimeTicks(ticks);
    }

    // Returns the internal numeric value of the TimeTicks object.
    // For serializing, use FromInternalValue to reconstitute.
    u2int64 ToInternalValue() const 
    {
        return ticks_;
    }

    TimeTicks& operator=(TimeTicks other) 
    {
        ticks_ = other.ticks_;
        return *this;
    }

    // Compute the difference between two times.
    TimeDelta operator-(TimeTicks other) const 
    {
        return TimeDelta(ticks_ - other.ticks_);
    }

    // Modify by some time delta.
    TimeTicks& operator+=(TimeDelta delta) 
    {
        ticks_ += delta.delta_;
        return *this;
    }
    TimeTicks& operator-=(TimeDelta delta) 
    {
        ticks_ -= delta.delta_;
        return *this;
    }

    // Return a new TimeTicks modified by some delta.
    TimeTicks operator+(TimeDelta delta) const 
    {
        return TimeTicks(ticks_ + delta.delta_);
    }
    TimeTicks operator-(TimeDelta delta) const 
    {
        return TimeTicks(ticks_ - delta.delta_);
    }

    // Comparison operators
    bool operator==(TimeTicks other) const 
    {
        return ticks_ == other.ticks_;
    }
    bool operator!=(TimeTicks other) const 
    {
        return ticks_ != other.ticks_;
    }
    bool operator<(TimeTicks other) const 
    {
        return ticks_ < other.ticks_;
    }
    bool operator<=(TimeTicks other) const 
    {
        return ticks_ <= other.ticks_;
    }
    bool operator>(TimeTicks other) const 
    {
        return ticks_ > other.ticks_;
    }
    bool operator>=(TimeTicks other) const 
    {
        return ticks_ >= other.ticks_;
    }

protected:
    friend class TimeDelta;

    // Please use Now() to create a new object. This is for internal use
    // and testing. Ticks is in microseconds.
    explicit TimeTicks(u2int64 ticks) 
        : ticks_(ticks)
    {
    }

protected:
    // Tick count in microseconds.
    u2int64 ticks_;

};

inline TimeTicks TimeDelta::operator+(TimeTicks t) const 
{
    return TimeTicks(t.ticks_ + delta_);
}



/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2Time_H__
