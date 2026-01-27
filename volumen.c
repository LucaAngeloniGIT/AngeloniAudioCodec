#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

void volumen_flotante(float *out, int numerodesamples, float ganancia){
    for (int i = 0; i < numerodesamples; i++) {
    out[i] *= ganancia;
    out[i] = fmaxf(-1.0f, fminf(1.0f, out[i]));
    }
}
 