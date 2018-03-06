#pragma once

#include "fft.hh"

template <typename fp_type, int len>
void generatePerfectSawSpectrum(fp_type *spectrum);

template <typename fp_type, int len>
void generatePerfectSawSignal(fp_type *signal);

template <typename fp_type, int len>
void generatePerfectSquareSpectrum(fp_type *spectrum);

template <typename fp_type, int len>
void generatePerfectSquareSignal(fp_type *signal);

#include "scwf-gen.hxx"
