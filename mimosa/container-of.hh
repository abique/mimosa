#ifndef MIMOSA_CONTAINER_OF_HH
# define MIMOSA_CONTAINER_OF_HH

# include <cstdint>

namespace mimosa
{
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

#endif /* !MIMOSA_CONTAINER_OF_HH */
