#include <stdlib.h>
#include <string.h>
#include "frames.h"
#include "fftw3.h"

void procesar_frames(float *in,float *out,int num_samples,int N,int hop,const float *ventana){
    float *frame = malloc(N * sizeof(float));
    float *norm = calloc(num_samples, sizeof(float)); //buffer para normalizar la ventana

    fftwf_complex *X = fftwf_malloc(sizeof(fftwf_complex) * (N/2 + 1));

    fftwf_plan plan_fft  = fftwf_plan_dft_r2c_1d(N, frame, X, FFTW_ESTIMATE);
    fftwf_plan plan_ifft = fftwf_plan_dft_c2r_1d(N, X, frame, FFTW_ESTIMATE);
    
    
    
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




        fftwf_execute(plan_fft); //fft
        fftwf_execute(plan_ifft);//ifft
        
        //fftw no normaliza
        
        for (int n = 0; n < N; n++)
            frame[n] /= N;

        //overlap-add

        for (int n = 0; n < N; n++) {
            norm[posicion + n] += ventana[n];
            out[posicion + n] += frame[n];
            /*recorro el buffer de salida desde la posicion inicial hasta 
              el fin del frame y le copio la muestra del frame procesado*/
            out[n] /= norm[n];
        }
    }
    fftwf_destroy_plan(plan_fft);
    fftwf_destroy_plan(plan_ifft);
    fftwf_free(X);
    free(frame);
    free(norm);
}
