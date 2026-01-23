#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "LeerWav.h"

int LeerWav(const char *nombrearchivo, archivowav *wav) {//no compila esto solo porque no es un main()
    
    FILE *f = fopen(nombrearchivo, "rb");
    if (!f) return -1;

    char riff[4];
    uint32_t size;
    char wave[4];
    char fmt[4];
    uint32_t fmt_size;
    uint16_t format, channels, bits;
    uint32_t sample_rate, byte_rate;
    uint16_t block_align;

    //Encabezado
    //No se pasan a la estructura (osea no se guardan) porque no sirven, son solo validacion del formato

    fread(riff, 1, 4, f);
    fread(&size, 4, 1, f);
    fread(wave, 1, 4, f);
    fread(fmt, 1, 4, f);
    fread(&fmt_size, 4, 1, f);

    //Audio

    fread(&wav->format, 2, 1, f);       // 1 = PCM
    fread(&wav->channels, 2, 1, f);
    fread(&wav->sample_rate, 4, 1, f);
    fread(&wav->byte_rate, 4, 1, f);
    fread(&wav->block_align, 2, 1, f);
    fread(&wav->bits, 2, 1, f);

    char data_id[4];
    uint32_t data_size;

    fread(data_id, 1, 4, f);
    fread(&wav->data_size, 4, 1, f);

    wav->samples = malloc(wav->data_size); //le asignamos la memoria al puntero de la estructura 
    if (!wav->samples) {
        fclose(f);
        return -2;
    }

    fread(wav->samples, 1, wav->data_size, f); //me llevo los samples del audio a la estructura

    fclose(f);
    return 0;
}

void LiberarWav(archivowav *wav)
{
    if (wav->samples) {
        free(wav->samples);
        wav->samples = NULL;
    }
}