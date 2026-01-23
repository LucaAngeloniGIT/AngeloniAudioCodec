#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void pcm_flotante(const void *pcm, float *out, int numerodesamples, int bitsporsample) {
    int i;

    if (bitsporsample == 8) {
        const uint8_t *p = (const uint8_t*)pcm;
        for (i = 0; i < numerodesamples; i++) {
            out[i] = ((int)p[i] - 128) / 128.0f;
        }
    }
    else if (bitsporsample == 16) {
        const int16_t *p = (const int16_t*)pcm;
        for (i = 0; i < numerodesamples; i++) {
            out[i] = p[i] / 32768.0f;
        }
    }
    else if (bitsporsample == 24) {
        const uint8_t *p = (const uint8_t*)pcm;
        for (i = 0; i < numerodesamples; i++) {
            int32_t v = p[0] | (p[1] << 8) | (p[2] << 16);
            if (v & 0x800000) v |= 0xFF000000;  // sign extend
            out[i] = v / 8388608.0f;
            p += 3;
        }
    }
    else if (bitsporsample == 32) {
        const int32_t *p = (const int32_t*)pcm;
        for (i = 0; i < numerodesamples; i++) {
            out[i] = p[i] / 2147483648.0f;
        }
    }
}