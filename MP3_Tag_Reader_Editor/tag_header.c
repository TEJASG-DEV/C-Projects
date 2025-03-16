#include <stdio.h>
#include "tag_header.h"
#include "common.h"
// Definitions

int read_header(Header *hdr, FILE *mp3)
{
    // id3 is an array... name itself is base address.
    // file error handling is needed.
    fread(hdr->id3, sizeof(char [3]), 1, mp3);
    hdr->id3[3] = '\0';

    fread(hdr->ver, sizeof(char [2]), 1, mp3);
    fread(&hdr->flags, sizeof(char), 1, mp3);
    fread(hdr->size_as_bytes, sizeof(int), 1, mp3);
    
    //change_endian(&size_raw);

    hdr->calc_size = calculate_size((unsigned char *)hdr->size_as_bytes);
    rewind(mp3);

    return 0;
}
