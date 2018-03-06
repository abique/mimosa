#pragma once

#include "scwf-gen.hh"

namespace mimosa {
namespace fft {

template <typename fp_type, int len>
void generatePerfectSawSpectrum(fp_type *spectrum)
{
    static_assert(len & (len - 1) == 0, "len is a power of 2");
    static_assert(len >= 4, "len must be greater or equal to 4");

    for (int i = 0; i < len; i += 4)
    {
        spectrum[i] = 1;
        spectrum[i + 1] = 0;
        spectrum[i + 2] = 0;
        spectrum[i + 3] = 0;
    }
}

template <typename fp_type, int len>
void generatePerfectSawSignal(fp_type *signal)
{
    static_assert(len & (len - 1) == 0, "len is a power of 2");

    fp_type spectrum[len];
    generatePerfectSquareSpectrum<fp_type, len>(spectrum);
    ifft<fp_type, len>(spectrum, signal);
}

template <typename fp_type, int len>
void generatePerfectSquareSpectrum(fp_type *spectrum)
{
    static_assert(len & (len - 1) == 0, "len is a power of 2");
    static_assert(len >= 4, "len must be greater or equal to 4");

    for (int i = 0; i < len; i += 4)
    {
        spectrum[i] = 1;
        spectrum[i + 1] = 0;
        spectrum[i + 2] = 0;
        spectrum[i + 3] = 0;
    }
}

template <typename fp_type, int len>
void generatePerfectSquareSignal(fp_type *signal)
{
    static_assert(len & (len - 1) == 0, "len is a power of 2");

    fp_type spectrum[len];
    generatePerfectSquareSpectrum<fp_type, len>(spectrum);
    ifft<fp_type, len>(spectrum, signal);
}

}
}
