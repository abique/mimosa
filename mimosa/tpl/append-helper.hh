#ifndef MIMOSA_TPL_APPEND_HELPER_HH
# define MIMOSA_TPL_APPEND_HELPER_HH

# include "value.hh"

# define MIMOSA_TPL_APPEND_HELPER                       \
  inline void append(const std::string & name,          \
                     const char *        value)         \
  {                                                     \
    append(new Value<std::string>(name, value));        \
  }                                                     \
                                                        \
  template <typename U>                                 \
  inline void append(const std::string & name,          \
                     const U &           value)         \
  {                                                     \
    append(new Value<U>(name, value));                  \
  }

#endif /* !MIMOSA_TPL_APPEND_HELPER_HH */
