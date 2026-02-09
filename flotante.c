#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

void pcm_flotante(const void *pcm, float *out, int numerodesamples, int bitsporsample, int sample_rate){ {
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

void flotante_pcm(const float *flotante, void *out, int numerodesamples, int bitsporsample){ 
  uint32_t i;

    if (bitsporsample == 16) {
        int16_t *p = out; //se usa un puntero a out para indicar el tamaño del elemento y habilitar aritmetica de punteros. es un "casteo"
        for (i = 0; i < numerodesamples; i++) {
            float x = fmaxf(-1.0f, fminf(1.0f, flotante[i])); //-1.0f < flotante[i] < 1.0f 
                                                              // es como un compresor entre 1 y -1
            p[i] = (int16_t)lrintf(x * 32767.0f); //lrintf es una mejor manera de redondear que castear a (int)
            //p tiene que ser entero porque es el pcm.
        }
    }
    else if (bitsporsample == 24) {
            uint8_t *p = out; /* como no hay uint24_t en c 
            se almacena de a 1 byte para una alineacion mas sencilla, 
            ya que un puntero de 16 y otro de 8 seria extremadamente feo y desprolijo*/
            for (i = 0; i < numerodesamples; i++) {
            float x = fmaxf(-1.0f, fminf(1.0f, flotante[i]));
            int32_t v = (int32_t)lrintf(x * 8388607.0f); 
            /*si se puede castear con un registro de 32 
            ya que hay 8 bits de mas que simplemente no se usan*/ 

            p[0] =  v        & 0xFF;
            p[1] = (v >> 8)  & 0xFF;
            p[2] = (v >> 16) & 0xFF;
            /* multiplicar por 0xFF es dejar los primeros 8 bits
               ya que 0xFF = ... 00000000 11111111 
               Esto se hace para almacenar de a 8 bits ya que v es un entero de 32 bits*/

            p += 3; /*pasa 3 bytes
            p + n  =  p + n * sizeof(*p)  */
            }
        }
    else if (bitsporsample == 32) {
        int32_t *p = out;
        for (i = 0; i < numerodesamples; i++) {
            float x = fmaxf(-1.0f, fminf(1.0f, flotante[i]));
            p[i] = (int32_t)lrintf(x * 2147483647.0f);
        }
    }
}