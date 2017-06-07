#pragma once

#include <cstdint>
#include <ctime>
#include <stdexcept>

#include <unistd.h>
#include <sys/time.h>

#ifdef __MACH__
# include <mach/mach.h>
# include <mach/clock.h>
#endif

#ifndef CLOCK_MONOTONIC_COARSE
# define CLOCK_MONOTONIC_COARSE CLOCK_MONOTONIC
#endif

#ifndef CLOCK_REALTIME_COARSE
# define CLOCK_REALTIME_COARSE CLOCK_REALTIME
#endif

namespace mimosa
{
  // signed to ease time substraction
  typedef ::int64_t Time;

  const Time nanosecond  = 1;
  const Time microsecond = 1000 * nanosecond;
  const Time millisecond = 1000 * microsecond;
  const Time second      = 1000 * millisecond;
  const Time minute      = 60 * second;
  const Time hour        = 60 * minute;
  const Time day         = 24 * hour;

#ifdef __MACH__
  inline Time realTime()
  {
    ::clock_serv_t cal_clock;
    ::mach_timespec tp;

    ::host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cal_clock);
    ::clock_get_time(cal_clock, &tp);
    return tp.tv_nsec * nanosecond + tp.tv_sec * second;
  }

  inline Time monotonicTime()
  {
    ::clock_serv_t sys_clock;
    ::mach_timespec tp;

    ::host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &sys_clock);
    ::clock_get_time(sys_clock, &tp);
    return tp.tv_nsec * nanosecond + tp.tv_sec * second;
  }

  inline Time realTimeCoarse()
  {
    return realTime();
  }

  inline Time monotonicTimeCoarse()
  {
    return monotonicTime();
  }
#else
  inline Time realTime()
  {
    ::timespec tp;
    int ret = ::clock_gettime(CLOCK_REALTIME, &tp);
    if (ret)
      throw std::runtime_error("clock_gettime");
    return tp.tv_nsec * nanosecond + tp.tv_sec * second;
  }

  inline Time monotonicTime()
  {
    ::timespec tp;
    int ret = ::clock_gettime(CLOCK_MONOTONIC, &tp);
    if (ret)
      throw std::runtime_error("clock_gettime");
    return tp.tv_nsec * nanosecond + tp.tv_sec * second;
  }

  inline Time realTimeCoarse()
  {
    ::timespec tp;
    int ret = ::clock_gettime(CLOCK_REALTIME_COARSE, &tp);
    if (ret)
      throw std::runtime_error("clock_gettime");
    return tp.tv_nsec * nanosecond + tp.tv_sec * second;
  }

  inline Time monotonicTimeCoarse()
  {
    ::timespec tp;
    int ret = ::clock_gettime(CLOCK_MONOTONIC_COARSE, &tp);
    if (ret)
      throw std::runtime_error("clock_gettime");
    return tp.tv_nsec * nanosecond + tp.tv_sec * second;
  }
#endif

  inline Time time()
  {
    return monotonicTimeCoarse();
  }

  inline ::timespec toTimeSpec(Time time)
  {
    ::timespec tp;
    tp.tv_sec  = time / second;
    tp.tv_nsec = time % second;
    return tp;
  }

  inline ::timeval toTimeVal(Time time)
  {
    ::timeval tv;
    tv.tv_sec  = time / second;
    tv.tv_usec = (time % second) / microsecond;
    return tv;
  }

  inline void sleep(Time duration) { ::usleep(duration / microsecond); }
}
