#ifndef FFT_H
#define FFT_H

#include "fftw3.h"

typedef struct {
    int N;
    float *buffer_tiempo;
    fftwf_complex *buffer_frecuencia;
    fftwf_plan plan_fft;
    fftwf_plan plan_ifft;
    int num_samples;
    int hop;
    const float *ventana;
    int sample_rate;

} FFTContext;

FFTContext* fft_creacion(int N);
void fft_avanzar(FFTContext *ctx, float *in);
void fft_inversa(FFTContext *ctx, float *out);
void fft_liberar(FFTContext *ctx);

#endif
