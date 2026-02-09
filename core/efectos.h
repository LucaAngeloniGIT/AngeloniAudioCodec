#ifndef efectos_H
#define efectos_H

void pasa_bajos(float *in,float *out,int num_samples,int N,int hop,const float *ventana, int sample_rate,float frecuencia_corte);
void pasa_altos(float *in,float *out,int num_samples,int N,int hop,const float *ventana, int sample_rate, float frecuencia_corte);
void ecualizador(float *in,float *out,int num_samples,int N,int hop,const float *ventana, int sample_rate, float frecuencia_corte);

#endif