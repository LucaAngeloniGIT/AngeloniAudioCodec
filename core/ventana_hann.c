#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void ventana_hann(float *w, int N) {
    for (int n = 0; n < N; n++) {
        w[n] = 0.5f * (1.0f - cosf(2.0f * M_PI * n / (N - 1)));
        /* -w[n] es la funcion ventana de hann
           -cosf es mejor que cos porque trabaja con float
           -(N-1) es para no incluir el último valor del periodo en la funcion coseno*/
    }
}