#pragma once

namespace mimosa {
namespace fft {

template <typename fp_type, int len>
void fft(const fp_type *signal, fp_type *spectrum);

template <typename fp_type, int len>
void ifft(const fp_type *spectrum, fp_type *signal);

}
}

#include "fft.hxx"
