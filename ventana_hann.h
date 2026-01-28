#ifndef VENTANA_H
#define VENTANA_H

void ventana_hann(float *w, int N);
void aplicar_ventana(float *frame, const float *w, int N);

#endif
