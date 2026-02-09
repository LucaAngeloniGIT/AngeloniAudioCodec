#include <stdio.h>
#include "wav.h"
#include "flotante.h"
#include "volumen.h"
#include "ventana_hann.h"
#include "efectos.h"
#include <stdlib.h>

#define frame_size 1024
#define hop_size frame_size/2 //%50 overlap

int main(void)
{
    archivowav wav; //creo mi instancia wav con la estructura archivowav

    if (LeerWav("audio.wav", &wav) != 0) { //cargo el objeto wav con la data del archivo segun la estructura archivowav
        printf("Error leyendo WAV\n");
        return 1;
    }

    printf("Canales: %d\n", wav.channels);
    printf("Sample rate: %d\n", wav.sample_rate);
    printf("Bits por muestra: %d\n", wav.bits);
    printf("Bytes de audio: %u\n", wav.data_size);

    int num_samples = wav.num_frames * wav.channels;

    float *buffer_audio_flotante = malloc(num_samples * sizeof(float)); //se reserva la cantidad de muestras multiplicado por los canales.
    void *buffer_audio_pcm = malloc(wav.data_size); //reservo memoria para el pcm de salida

    pcm_flotante(wav.samples,buffer_audio_flotante,(num_samples),wav.bits);
    volumen_flotante(buffer_audio_flotante,(num_samples),2.0f);

    /* frames*/

    float *ventana = malloc(frame_size * sizeof(float));
    ventana_hann(ventana, frame_size);
    
    float *buffer_audio_frames = calloc(num_samples, sizeof(float)); //calloc inicia en 0 el buffer
    ecualizador(buffer_audio_flotante, buffer_audio_frames,num_samples, frame_size, hop_size, ventana, wav.sample_rate,5000.0f);
    
    free(ventana);

    /*Exportar devuelta a PCM*/

    flotante_pcm(buffer_audio_frames,buffer_audio_pcm,(num_samples),wav.bits);
    free(wav.samples); //libero la memoria del wav original
    wav.samples = buffer_audio_pcm; //asigno el nuevo pcm al wav
    exportwav("audio_exportado.wav",&wav);

    free(buffer_audio_flotante);
    free(buffer_audio_pcm);
    LiberarWav(&wav);
    return 0;
}
