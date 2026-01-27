#include <stdio.h>
#include "wav.h"
#include "wav.c"
#include "flotante.h"
#include "flotante.c"
#include "volumen.c"
#include "volumen.h"
#include <stdlib.h>


int main(void)
{
    archivowav wav; //creo mi objeto wav con la estructura archivowav

    if (LeerWav("audio.wav", &wav) != 0) { //cargo el objeto wav con la data del archivo segun la estructura archivowav
        printf("Error leyendo WAV\n");
        return 1;
    }

    printf("Canales: %d\n", wav.channels);
    printf("Sample rate: %d\n", wav.sample_rate);
    printf("Bits por muestra: %d\n", wav.bits);
    printf("Bytes de audio: %u\n", wav.data_size);

    float *audio_flotante = malloc(wav.num_frames * wav.channels * sizeof(float)); //se reserva la cantidad de muestras multiplicado por los canales.
    void *audio_PCM = malloc(wav.data_size); //reservo memoria para el pcm de salida

    pcm_flotante(wav.samples,audio_flotante,(wav.num_frames * wav.channels),wav.bits);
    volumen_flotante(audio_flotante,(wav.num_frames * wav.channels),2.0f);
    flotante_pcm(audio_flotante,audio_PCM,(wav.num_frames * wav.channels),wav.bits);

    free(wav.samples); //libero la memoria del wav original
    wav.samples = audio_PCM; //asigno el nuevo pcm al wav
    exportwav("audio_exportado.wav",&wav);

    free(audio_flotante);
    free(audio_PCM);
    LiberarWav(&wav);
    return 0;
}
