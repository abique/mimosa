#ifndef MIMOSA_RUNTIME_TIME_HH
# define MIMOSA_RUNTIME_TIME_HH

# include <melon/melon.h>

namespace mimosa
{
  namespace runtime
  {
    typedef ::melon_time_t Time;
    inline Time time() { return ::melon_time(); }
    inline Time nanoseconds(int64_t ns) { return ns * MELON_NANOSECOND; }
    inline Time microseconds(int64_t us) { return us * MELON_MICROSECOND; }
    inline Time milliseconds(int64_t ms) { return ms * MELON_MILLISECOND; }
    inline Time seconds(int64_t s) { return s * MELON_SECOND; }
    inline Time minutes(int64_t m) { return m * MELON_MINUTE; }
    inline Time hours(int64_t h) { return h * MELON_HOUR; }
    inline Time days(int64_t d) { return d * MELON_DAY; }
    inline void sleep(Time duration) { ::melon_usleep(duration / 1000); }
  }
}

#endif /* !MIMOSA_TIME_HH */
