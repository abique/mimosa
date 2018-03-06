#pragma once

#include <cmath>

#include "normalize.hh"

namespace mimosa {
namespace fft {

template <typename fp_type, int len>
void normalizeEuclide(fp_type *spectrum)
{
    fp_type norm = 0;

    for (int i = 0; i < len; ++i)
        norm += spectrum[i] * spectrum[i];
    norm = std::sqrt(norm);

    for (int i = 0; i < len; ++i)
        spectrum[i] /= norm;
}

}
}
