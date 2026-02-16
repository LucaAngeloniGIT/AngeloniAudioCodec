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
    InitAudioDevice();
    Music music = LoadMusicStream("F:/desarrollos/AngeloniAudioCodec/data/audio.wav");
    PlayMusicStream(music);
    SetTargetFPS(60);

    FFTContext *fftcontexto = fft_creacion(N); //creo instancia de puntero FFTContext con N muestras

    fftcontexto->ventana = malloc(sizeof(float)*N);

    archivowav wav; //creo mi instancia de puntero wav con la estructura archivowav 

    if (LeerWav("F:/desarrollos/AngeloniAudioCodec/data/audio.wav", &wav) != 0) { //cargo el objeto wav con la data del archivo segun la estructura archivowav
        printf("Error leyendo WAV\n");
        return 1;
    }

    fftcontexto->num_samples = wav.num_frames * wav.channels;
    fftcontexto->sample_rate = wav.sample_rate;
    float *buffer_audio_flotante = malloc(fftcontexto->num_samples * sizeof(float));
    pcm_flotante(wav.samples,buffer_audio_flotante,(fftcontexto->num_samples),wav.bits);
    
    ventana_hann(fftcontexto->ventana, N);

    printf("ventana[0] = %f\n", fftcontexto->ventana[0]);

int frame_index = 0;
int hop = N/2;

while(!WindowShouldClose())
{
    UpdateMusicStream(music);

    float tiempo = GetMusicTimePlayed(music);
    int frame_index = (int)(tiempo * fftcontexto->sample_rate);

    if(frame_index + N < fftcontexto->num_samples)
        fft_calcular(fftcontexto, buffer_audio_flotante + frame_index);

    BeginDrawing();
    ClearBackground(BLACK);

    for(int i=0;i<N/2;i++)
    {
        float mag = sqrtf(
            fftcontexto->buffer_frecuencia[i][0]*fftcontexto->buffer_frecuencia[i][0] +
            fftcontexto->buffer_frecuencia[i][1]*fftcontexto->buffer_frecuencia[i][1]
        );

        mag /= N;
        float db = 20*log10f(mag + 1e-6f);
        float y = 450 - (db + 60)*5;

        DrawRectangle(i*2, y, 2, 450-y, GREEN);
    }

    EndDrawing();
}

    //free(wav.samples);
    free(buffer_audio_flotante);
    LiberarWav(&wav);
    fft_liberar(fftcontexto);
    CloseWindow();
    return 0;
}
