#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

void pcm_to_float(const void *pcm, float *out, int numerodesamples, int bitsporsample); //a esta verga le decimos "promesa" porque si no no compila, declara la funcion que necesita ser declarada antes del main pero que necesita variables del main. :)

float AmplitudMax(const float *samples, int num_samples){ //importante crear la funcion para que devuelva float, si no redondea el entero.
    float pico = 0.0f;
    for (int i=0;i<num_samples; i++){
        if (fabs(samples[i])>pico){ //fabs saca el valor absoluto
            pico = fabs(samples[i]);
        }
    }
    return pico;
}

int main() {
    FILE *f = fopen("audio.wav", "rb");

    char riff[4];
    uint32_t size;
    char wave[4];

    fread(riff, 1, 4, f);
    fread(&size, 4, 1, f);
    fread(wave, 1, 4, f);

    char fmt[4];
    uint32_t fmt_size;
    uint16_t format, channels, bits;
    uint32_t sample_rate, byte_rate;
    uint16_t block_align;

    fread(fmt, 1, 4, f);           // "fmt "
    fread(&fmt_size, 4, 1, f);
    fread(&format, 2, 1, f);       // 1 = PCM
    fread(&channels, 2, 1, f);
    fread(&sample_rate, 4, 1, f);
    fread(&byte_rate, 4, 1, f);
    fread(&block_align, 2, 1, f);
    fread(&bits, 2, 1, f);

    char data_id[4];
    uint32_t data_size;

    fread(data_id, 1, 4, f);       // "data"
    fread(&data_size, 4, 1, f);

    void *samples = malloc(data_size); // void es para un puntero a un tipo de variable que todavia no se sabe cual es
    fread(samples, 1, data_size, f);

    //free(samples);

    int num_samples = data_size / (bits / 8); //dividimos el audio en bytes

    float *out = malloc(num_samples * sizeof(float));

    pcm_to_float(samples,out,num_samples,bits);

    float valormax = AmplitudMax(out, num_samples);

    printf("Numero de samples: %i\n",num_samples);
    printf("valor maximo del wav: %f\n",valormax);

    free(samples);
    free(out);
    fclose(f);
    return 0;
}


void pcm_to_float(const void *pcm, float *out, int numerodesamples, int bitsporsample) {
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


