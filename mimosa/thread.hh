#pragma once

#include <functional>
#include <stdexcept>
#include <memory>

#include <pthread.h>

#include "time.hh"

namespace mimosa
{
  void setCurrentThreadName(const char *name) noexcept;
}
