#include <cmath>
#include <cstddef>

namespace mimosa {
namespace fft {

template <typename fp_type, int len>
void fft(const fp_type *signal, fp_type *spectrum)
{
    static_assert(len & (len - 1) == 0, "len is a power of 2");

    for (int i = 0; i < len; i += 2)
    {
        spectrum[i] = 0;
        spectrum[i + 1] = 0;
        for (int j = 0; j < len; ++j)
        {
            const fp_type angle = 2 * M_PI * j / len;
            spectrum[i] += signal[j] * std::cos(angle);
            spectrum[i + 1] += signal[j] * std::sin(angle);
        }
    }
}

template <typename fp_type, int len>
void ifft(const fp_type *spectrum, fp_type *signal)
{
    static_assert(len & (len - 1) == 0, "len is a power of 2");

    for (int i = 0; i < len; ++i)
    {
        signal[i] = 0;
        for (int j = 0; j < len; j += 2)
        {
            const fp_type angle = 2 * M_PI * j / len;
            signal[i] += spectrum[j] * std::cos(angle) + spectrum[j + 1] + std::sin(angle);
        }
    }
}

}
}
