#include "fft.h"
#include <stdlib.h>
#include <string.h>
#include "wav.h"

FFTContext* fft_creacion(int N){
    FFTContext *ctx = malloc(sizeof(FFTContext));

    ctx->N = N;

    ctx->buffer_tiempo = fftwf_malloc(sizeof(float)*N);
    ctx->buffer_frecuencia = fftwf_malloc(sizeof(fftwf_complex)*(N/2 + 1));

    ctx->ventana = malloc(sizeof(float)*N);   // <<< ESTA LÍNEA ES CLAVE

    ctx->hop = N/2;        // típico
    ctx->num_samples = 0; // se define luego
    ctx->sample_rate = 44100; // por defecto

    ctx->plan_fft = fftwf_plan_dft_r2c_1d(
        N,
        ctx->buffer_tiempo,
        ctx->buffer_frecuencia,
        FFTW_ESTIMATE
    );

    ctx->plan_ifft = fftwf_plan_dft_c2r_1d(
        N,
        ctx->buffer_frecuencia,
        ctx->buffer_tiempo,
        FFTW_ESTIMATE
    );

    return ctx;
}

void fft_calcular(FFTContext *ctx, float *in){

    float *frame = malloc(ctx->N * sizeof(float));
    float *norm = calloc(ctx->num_samples, sizeof(float)); //buffer para normalizar la ventana

    fftwf_complex *X = fftwf_malloc(sizeof(fftwf_complex) * (ctx->N/2 + 1)); //almacena la parte positiva de la FFT

    fftwf_plan plan_fft  = fftwf_plan_dft_r2c_1d(ctx->N, frame, X, FFTW_ESTIMATE); 
    fftwf_plan plan_ifft = fftwf_plan_dft_c2r_1d(ctx->N, X, frame, FFTW_ESTIMATE);
    /*el plan es una estructura de datos que contiene toda la informacion 
    necesaria para ejecutar la transformada rapida de fourier*/
    
    
    for (int posicion = 0; posicion + ctx->N <= ctx->num_samples; posicion += ctx->hop) {

        memcpy(frame, &in[posicion], ctx->N * sizeof(float));
        /*copio en frame la cantidad de bits que hay desde la posicion 
        del audio "in" hasta N floats (copio de a N y avanzo de a N/2)*/


        //ventana

        for (int n = 0; n < ctx->N; n++) {
            frame[n] *= ctx->ventana[n];
            /*recorro el frame y multiplico cada muestra por el valor 
              que le corresponde de la funcion matematica ventana  */
        }

        //fft
        fftwf_execute(plan_fft); 

        //ifft
        fftwf_execute(plan_ifft); /*Para cada transformada de fourier hay una inversión que reconstruye la funcion original*/
        
        //la libreria fftw no normaliza la señal y la ifft multiplica N veces la amplitud 
        for (int n = 0; n < ctx->N; n++)
            frame[n] /= ctx->N; // divido por N para volver a la amplitud original

        //overlap-add
        for (int n = 0; n < ctx->N; n++) {
            norm[posicion + n] += ctx->ventana[n];
            in[posicion + n] += frame[n];
        }
        /*recorro el buffer de salida desde la posicion inicial hasta 
        el fin del frame y le copio la muestra del frame procesado*/
    
    }

    //Normalizar
    for (int i = 0; i < ctx->num_samples; i++) {
        if (norm[i] > 1e-6f)
        in[i] /= norm[i];
        /*normalizamos porque el overlapp-add suma las ventanas*/
    }



    fftwf_free(X);
    free(frame);
    free(norm);


//////////////////////////////


    for(int i=0;i<ctx->N;i++)
        ctx->buffer_tiempo[i] = in[i];
    fftwf_execute(ctx->plan_fft);
}

void fft_inversa(FFTContext *ctx, float *out){
    fftwf_execute(ctx->plan_ifft);
    for(int i=0;i<ctx->N;i++)
        out[i] = ctx->buffer_tiempo[i] / ctx->N;
}

void fft_liberar(FFTContext *ctx){
    fftwf_destroy_plan(ctx->plan_fft);
    fftwf_destroy_plan(ctx->plan_ifft);

    fftwf_free(ctx->buffer_tiempo);
    fftwf_free(ctx->buffer_frecuencia);

    free(ctx->ventana);

    free(ctx);
}
