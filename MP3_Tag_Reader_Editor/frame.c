#include <stdlib.h>
#include <string.h>
#include "frame.h"
#include "common.h"

// some functions necessary for this file only.
static char *search(const char *start, const char *end, const char *query)
{    
    char *p = (char *)start;
    int flag = 1;
    while (p <= (end - 4)) { // no access past buffer.
        
        for (int i = 0; i < 4; i++) {
            if ( p[i] == query[i] ) {
                // pass
            } else {
                flag = 0;
            }
        }

        if (flag) {
            return p;
        } else {
            flag = 1;
        }

        p++;
    }

    return NULL; // hopefully never happens.
}

static int encode_size(int size_int, char (*bytes)[4])
{
    for (int i = 3; i >= 0; i--) {
        if (i > 0) {
            (*bytes)[4 - 1 - i] = size_int / (0x1 << (7 * i));
        } else {
            (*bytes)[4 - 1 - i] = size_int;
        }
        //printf("%02hhX\n", (*bytes)[4 - 1 - i]);
        size_int %= (0x1 << (7 * i));
    }

    return 0;
}

// functions visible for others
int extract_frames(const char *read_buf, int buf_size, const char *name, Frame *fr_p)
{
    char *id = NULL;
    //id = strstr(read_buf, name); This is buggy. strstr works only upto '\0'. the super string has tags after '\0'.
    //           start        end                 query
    id = search(read_buf, (read_buf + buf_size), name);
    //printf("ID = %p, %c%c%c%c\n", id, id[0], id[1], id[2], id[3]);
    memcpy(fr_p->frame_id,id, 4);
    fr_p->frame_id[4] = '\0';
    
    id += 4;
    memcpy(&fr_p->size, id, 4);
    fr_p->size = calculate_size( (unsigned char *) &fr_p->size);

    id += 4;
    memcpy(&fr_p->enc, id, 1);
    
    id += 1;
    memcpy(&fr_p->flags, id, 2);

    id += 2;
    fr_p->frame_string = calloc(fr_p->size, 1);
    memcpy(fr_p->frame_string, id, fr_p->size - 1);
    
    return 0;
}

/*
    Note: A TON of error handling is needed.
*/

int edit_frame_and_write_tag(unsigned char *full_tag, size_t tag_size, const char *frame_id, const char *change_string, FILE *song_file)
{
    unsigned char *frame_begin = NULL, *frame_end = NULL;

    char ID_bytes[4] = {0}, encode_size_bytes[4] = { 0 }, extra_bytes[3] = { 0 };
    int str_size = 0;

    rewind(song_file);
    frame_begin = (unsigned char *)search((char *)full_tag, (char *)(full_tag + tag_size), frame_id);
    memcpy(ID_bytes, frame_begin, 4);
    memcpy(encode_size_bytes, (frame_begin + 4), 4);
    memcpy(extra_bytes, (frame_begin + 8), 3);

    str_size = calculate_size((unsigned char *)encode_size_bytes);
    frame_end = (frame_begin + 8 + 3 + str_size - 1);// ID + size_bytes + 3extra + str_len - 1. str_len is extra.

    encode_size((strlen(change_string) + 1), &encode_size_bytes);
    // Write the tag in full...but before that, consider reading the necessary after frame data..
    fwrite(full_tag, (frame_begin - full_tag), 1, song_file); // before edit frame, all data written
    // write all the edited bytes including tag.
    // ID
    fwrite(ID_bytes, 4 * sizeof(char), 1, song_file);
    // Size
    fwrite(encode_size_bytes, 4 * sizeof(char), 1, song_file);
    // 3 extra bytes
    fwrite(extra_bytes, 3 * sizeof(char), 1, song_file);
    // finally the string
    fwrite(change_string, strlen(change_string), 1, song_file); // the string is written
    // Now write the rest of the tag.
    // you may go with a while loop or, with a fwrite call.
    fwrite(frame_end, (full_tag + tag_size) - frame_end, 1, song_file);
    fflush(song_file);

    return 0;
}
