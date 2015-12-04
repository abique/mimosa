#ifndef MIMOSA_ENDIAN_HH
# define MIMOSA_ENDIAN_HH

# ifdef __FreeBSD__
#  include <sys/endian.h>
# elif defined(__MACH__)
#  include <machine/endian.h>
# else
#  include <endian.h>
# endif

#endif /* !MIMOSA_ENDIAN_HH */
