#ifndef MIMOSA_RUNTIME_TIME_HH
# define MIMOSA_RUNTIME_TIME_HH

# include <melon/melon.h>

namespace mimosa
{
  namespace runtime
  {
    typedef ::melon_time_t Time;
    inline Time time() { return ::melon_time(); }
    inline Time nanoseconds(int64_t ns) { return ns; }
    inline Time microseconds(int64_t us) { return us * 1000; }
    inline Time milliseconds(int64_t ms) { return ms * 1000 * 1000; }
    inline Time seconds(int64_t s) { return s * 1000 * 1000 * 1000; }
    inline Time minutes(int64_t m) { return m * 60 * 1000 * 1000 * 1000; }
    inline Time hours(int64_t h) { return h * 60 * 60 * 1000 * 1000 * 1000; }
    inline Time days(int64_t d) { return d * 24 * 60 * 60 * 1000 * 1000 * 1000; }
    inline void sleep(Time duration) { ::melon_usleep(duration / 1000); }
  }
}

#endif /* !MIMOSA_TIME_HH */
