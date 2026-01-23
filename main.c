#include <stdio.h>
#include "LeerWav.h"

int main(void)
{
    archivowav wav;

    if (LeerWav("audio.wav", &wav) != 0) {
        printf("Error leyendo WAV\n");
        return 1;
    }

    printf("Canales: %d\n", wav.channels);
    printf("Sample rate: %d\n", wav.sample_rate);
    printf("Bits por muestra: %d\n", wav.bits);
    printf("Bytes de audio: %u\n", wav.data_size);

    if (wav.bits == 16) {
        int16_t *s = (int16_t *)wav.samples;
        printf("Primera muestra: %d\n", s[0]);
    }

    float *out = malloc(num_samples * sizeof(float));

    pcm_flotante(wav.samples,out,wav.data_size,wav.bits);
        
    for (int i = 0; i < 4; i++) {
        printf("%f\n", out[i]);
    }

    LiberarWav(&wav);
    return 0;
}
