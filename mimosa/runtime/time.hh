#ifndef MIMOSA_RUNTIME_TIME_HH
# define MIMOSA_RUNTIME_TIME_HH

# include <melon/melon.h>

namespace mimosa
{
  namespace runtime
  {
    inline ::melon_time_t time() { return ::melon_time(); }
    inline ::melon_time_t nanoseconds(int64_t ns) { return ns; }
    inline ::melon_time_t microseconds(int64_t us) { return us * 1000; }
    inline ::melon_time_t milliseconds(int64_t ms) { return ms * 1000 * 1000; }
    inline ::melon_time_t seconds(int64_t s) { return s * 1000 * 1000 * 1000; }
    inline ::melon_time_t minutes(int64_t m) { return m * 60 * 1000 * 1000 * 1000; }
    inline ::melon_time_t hours(int64_t h) { return h * 60 * 60 * 1000 * 1000 * 1000; }
    inline ::melon_time_t days(int64_t d) { return d * 24 * 60 * 60 * 1000 * 1000 * 1000; }
    inline void sleep(melon_time_t duration) { ::melon_usleep(duration / 1000); }
  }
}

#endif /* !MIMOSA_TIME_HH */
