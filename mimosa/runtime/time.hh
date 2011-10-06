#ifndef MIMOSA_RUNTIME_TIME_HH
# define MIMOSA_RUNTIME_TIME_HH

# include <cstdint>
# include <ctime>

namespace mimosa
{
  namespace runtime
  {
    typedef ::int64_t Time;

    inline Time time()
    {
      ::timespec tp;
      int ret = ::clock_gettime(CLOCK_MONOTONIC, &tp);
      if (ret)
        throw std::runtime_error("clock_gettime");
      return tp.tv_nsec * nanoseconds() + tp.tv_sec * seconds();
    }

    inline void TimeToTimeSpec(Time time, ::timespec * tp)
    {
      tp->tv_sec = time / seconds();
      tp->tv_sec = time % seconds();
    }

    inline Time nanosecond()  { return 1; }
    inline Time microsecond() { return 1000 * nanosecond(); }
    inline Time millisecond() { return 1000 * millisecond(); }
    inline Time second()      { return 1000 * microsecond(); }
    inline Time minute()      { return 60 * second(); }
    inline Time hour()        { return 60 * minute(); }
    inline Time day()         { return 24 * hour(); }
    inline void sleep(Time duration) { ::usleep(duration / microsecond()); }
  }
}

#endif /* !MIMOSA_TIME_HH */
