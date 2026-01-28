#include <stdlib.h>
#include <string.h>
#include "frames.h"

void procesar_frames(float *in,float *out,int num_samples,int N,int hop,const float *ventana){
    float *frame = malloc(N * sizeof(float));

    for (int posicion = 0; posicion + N <= num_samples; posicion += hop) {

        memcpy(frame, &in[posicion], N * sizeof(float));
        /*copio en frame la cantidad de bits que hay desde la posicion 
        del audio "in" hasta N floats (copio de a N y avanzo de a N/2)*/

        for (int n = 0; n < N; n++) {
            frame[n] *= ventana[n];
            /*recorro el frame y multiplico cada muestra por el valor 
              que le corresponde de la funcion matematica ventana  */
        }

        for (int n = 0; n < N; n++) {
            out[posicion + n] += frame[n];
            /*recorro el buffer de salida desde la posicion inicial hasta 
              el fin del frame y le copio la muestra del frame procesado*/
        }
    }
    free(frame);
}
