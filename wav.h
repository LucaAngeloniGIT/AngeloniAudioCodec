#ifndef WAV_H
#define WAV_H

#include <stdint.h>

typedef struct {
    uint16_t format;
    uint16_t channels; //cantidad de señales de audio. 1=mono / 2=estereo
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits;
    uint32_t data_size;
    uint32_t num_frames; //muestras por canal
    void    *samples; //es void porque no sabemos cuanto pesa el audio /// y en PCM no en float
} archivowav;

int LeerWav(const char *nombrearchivo, archivowav *wav);
int exportwav(const char *nombrearchivo, archivowav *wav);
void LiberarWav(archivowav *wav);

#endif
