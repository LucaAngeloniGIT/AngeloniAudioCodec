#include <stdlib.h>
#include <string.h>
#include "fftw3.h"

void pasa_bajos(float *in,float *out,int num_samples,int N,int hop,const float *ventana, int sample_rate, float frecuencia_corte){
    float *frame = malloc(N * sizeof(float));
    float *norm = calloc(num_samples, sizeof(float)); //buffer para normalizar la ventana

    fftwf_complex *X = fftwf_malloc(sizeof(fftwf_complex) * (N/2 + 1)); //almacena la parte positiva de la FFT

    fftwf_plan plan_fft  = fftwf_plan_dft_r2c_1d(N, frame, X, FFTW_ESTIMATE); 
    fftwf_plan plan_ifft = fftwf_plan_dft_c2r_1d(N, X, frame, FFTW_ESTIMATE);
    /*el plan es una estructura de datos que contiene toda la informacion 
    necesaria para ejecutar la transformada rapida de fourier*/
    
    
    for (int posicion = 0; posicion + N <= num_samples; posicion += hop) {

        memcpy(frame, &in[posicion], N * sizeof(float));
        /*copio en frame la cantidad de bits que hay desde la posicion 
        del audio "in" hasta N floats (copio de a N y avanzo de a N/2)*/


        //ventana

        for (int n = 0; n < N; n++) {
            frame[n] *= ventana[n];
            /*recorro el frame y multiplico cada muestra por el valor 
              que le corresponde de la funcion matematica ventana  */
        }

        //fft
        fftwf_execute(plan_fft); 

        //Procesamiento en el dominio de la frecuencia 

                        /*Efecto pasa bajos*/
            int k_corte = (int)(frecuencia_corte * N / sample_rate);

            for (int k = k_corte; k < N/2 + 1; k++) {
                X[k][0] = 0.0f; // parte real
                X[k][1] = 0.0f; // parte imaginaria
            }

        //       

        //ifft
        fftwf_execute(plan_ifft); /*Para cada transformada de fourier hay una inversión que reconstruye la funcion original*/
        
        //la libreria fftw no normaliza la señal y la ifft multiplica N veces la amplitud 
        for (int n = 0; n < N; n++)
            frame[n] /= N; // divido por N para volver a la amplitud original

        //overlap-add
        for (int n = 0; n < N; n++) {
            norm[posicion + n] += ventana[n];
            out[posicion + n] += frame[n];
        }
        /*recorro el buffer de salida desde la posicion inicial hasta 
        el fin del frame y le copio la muestra del frame procesado*/
    
    }

    //Normalizar
    for (int i = 0; i < num_samples; i++) {
        if (norm[i] > 1e-6f)
        out[i] /= norm[i];
        /*normalizamos porque el overlapp-add suma las ventanas*/
    }


    fftwf_destroy_plan(plan_fft);
    fftwf_destroy_plan(plan_ifft);
    fftwf_free(X);
    free(frame);
    free(norm);
}

