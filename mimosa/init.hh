#ifndef INIT_HH
# define INIT_HH

namespace mimosa
{
  void init(uint16_t nb_threads = 0);
  void wait();
  void deinit();
}

#endif /* !INIT_HH */
