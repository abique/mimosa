# include <sys/types.h>
# include <cstdint>

# ifdef __unix__
#  include <sys/uio.h>
# else
#  define IOV_MAX 128
struct iovec {
  void     *iov_base = nullptr;
  uint64_t  iov_len  = 0;
};
# endif
