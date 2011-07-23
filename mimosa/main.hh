#ifndef MIMOSA_MAIN_HH
# define MIMOSA_MAIN_HH

# include "init.h"

# define MIMOSA_MAIN(Argc, Argv)                                \
                                                                \
  struct __mimosa_main_data                                     \
  {                                                             \
    int     argc;                                               \
    char ** argv;                                               \
    int     retval;                                             \
  };                                                            \
                                                                \
  int __mimosa_main(int argc, char ** argv);                    \
                                                                \
  void __mimosa_main_wrapper(__mimosa_main_data * data)         \
  {                                                             \
    data-> retval = __mimosa_main(data->argc, data->argv);      \
  }                                                             \
                                                                \
  int main(int argc, char ** argv)                              \
  {                                                             \
    ::mimosa::init(argc, argv);                                 \
    __mimosa_main_data data = { argc, argv, 0 };                \
    ::mimosa::Fiber::start(__mimosa_main_wrapper, &data);       \
    ::mimosa::wait();                                           \
    ::mimosa::deinit();                                         \
    return 0;                                                   \
  }                                                             \
                                                                \
  int __mimosa_main(int Argc, char ** Argv)

#endif /* !MIMOSA_MAIN_HH */
