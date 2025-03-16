#ifndef __TAG_HEADER_H__
#define __TAG_HEADER_H__

typedef struct tag_header {
    char id3[4];
    char ver[2];
    char flags;
    unsigned char size_as_bytes[4];
    int calc_size;
} Header;

int read_header(Header *hdr, FILE *mp3);

#endif
