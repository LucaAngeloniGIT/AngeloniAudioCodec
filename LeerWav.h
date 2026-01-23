#ifndef WAV_H
#define WAV_H

#include <stdint.h>

typedef struct {
    uint16_t format;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits;
    uint32_t data_size;
    void    *samples; //es void porque no sabemos cuanto pesa el audio
} archivowav;

int LeerWav(const char *nombrearchivo, archivowav *wav);
void LiberarWav(archivowav *wav);

#endif
