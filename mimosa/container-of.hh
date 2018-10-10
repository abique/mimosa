#pragma once

#include <cstdint>

namespace mimosa
{
  /**
   * Returns the address of the object of type T containing the given member at the address u.
   */
  template <typename T, typename U>
  T *
  containerOf(U * u, U T::*Member)
  {
    union {
      T *       obj;
      uint8_t * obj2;
    };
    union {
      U *       mb;
      uint8_t * mb2;
    };

    obj         = nullptr;
    mb          = &(obj->*Member);
    auto offset = mb2 - obj2;
    mb          = u;
    obj2        = mb2 - offset;
    return obj;
  }
}

