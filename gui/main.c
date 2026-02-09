#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "fft.h"

#include "wav.h"
#include "flotante.h"
#include "volumen.h"
#include "ventana_hann.h"
#include "efectos.h"

#define N 1024

int main()
{
    InitWindow(800, 500, "Luca Angeloni Audio Codec - FFT Demo");
    SetTargetFPS(60);

    FFTContext *fft = fft_creacion(N); //creo instancia de puntero FFTContext con N muestras
    archivowav *wav; //creo mi instancia de puntero wav con la estructura archivowav 

    if (LeerWav("F:/desarrollos/AngeloniAudioCodec/data/audio.wav", &wav) != 0) { //cargo el objeto wav con la data del archivo segun la estructura archivowav
        printf("Error leyendo WAV\n");
        return 1;
    }

    fft->num_samples = wav->num_frames * wav->channels;

    float *buffer_audio_flotante = malloc(wav->num_samples * sizeof(float));
    pcm_flotante(wav->samples,buffer_audio_flotante,(wav->num_samples),wav->bits);
    

    while(!WindowShouldClose())
    {
        fft_calcular(fft, buffer_audio_flotante);

        BeginDrawing();
        ClearBackground(BLACK);

        for(int i=0;i<N/2;i++)
        {
            float mag = sqrtf(
                fft->buffer_frecuencia[i][0]*fft->buffer_frecuencia[i][0] +
                fft->buffer_frecuencia[i][1]*fft->buffer_frecuencia[i][1]
            );

            DrawLine(i, 450, i, 450 - mag*0.01f, GREEN);
        }

        EndDrawing();
    }

    //free(wav.samples);
    free(buffer_audio_flotante);
    LiberarWav(&wav);
    fft_liberar(fft);
    CloseWindow();
    return 0;
}
