#pragma once

namespace mimosa {
namespace fft {

template <typename fp_type, int len>
void normalizeEuclide(fp_type *spectrum);

}
}

#include "normalize.hxx"
