#ifndef FLOTANTE_H
#define FLOTANTE_H

#include <stdint.h>

void pcm_flotante(const void *pcm, float *out, int numerodesamples, int bitsporsample);
void flotante_pcm(const float *flotante, void *out, int numerodesamples, int bitsporsample);
#endif
