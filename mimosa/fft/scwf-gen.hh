#pragma once

#include "fft.hh"

template <typename fp_type, int len>
void generateIdealSawSpectrum(fp_type *spectrum);

template <typename fp_type, int len>
void generateIdealSawSignal(fp_type *signal);

template <typename fp_type, int len>
void generateIdealSquareSpectrum(fp_type *spectrum);

template <typename fp_type, int len>
void generateIdealSquareSignal(fp_type *signal);

#include "scwf-gen.hxx"
