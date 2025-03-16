#ifndef __FRAME_H__
#define __FRAME_H__
#include <stdio.h>

typedef struct mp3_tag_frame {
    char frame_id[5];
    int size;
    char enc; // Encoding type.
    short flags; // we got nothing to do with this.
    char *frame_string; // The string within the tag. Heap allocated
} Frame;


int extract_frames(const char *read_buf, int buf_size, const char *name, Frame *fr_p);


int edit_frame_and_write_tag(unsigned char *full_tag, size_t tag_size, const char *frame_id, const char *change_string, FILE *song_file);

#endif
