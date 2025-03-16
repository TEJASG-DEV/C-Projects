#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tag_header.h" // will be made sure to carry only relavant declarations & definitions.
#include "frame.h"

// Padding not taken into account. just for the sake of convinience of reading.

void print_string(const char *str, int size_str)
{
    for (int i = 0; i < size_str; i++) {
        putchar(str[i]);
    }
    putchar('\n');
}

int main(int argc, char *argv[])
{
    Header h;
    FILE *mp3_file = NULL;// fopen("Daavudi.mp3", "r"); // Daavudi.mp3 sample.mp3
    //FILE *tag = fopen("tag.t", "w");
    
    char *all_frames = NULL;
    unsigned char *tag = NULL;
    unsigned char *music_data = NULL;
    char *change_string = NULL;
    int edit_option = -1, year_len = 0, year_int = 0; // -a/-A/-c ... TIT2, TPE1, ...
    // The frames we want to read.
    char *relavant_frames[] = {
                                "TIT2",
                                "TPE1",
                                "TALB",
                                "TYER",
                                "TCON",
                                "COMM"
                            };
    Frame frames[6] = { 0 };
    //char string_buffer[100] = { 0 };
    
    //printf("Init pos : %lu\n", ftell(mp3_file));
    
    int res = -1;
    size_t file_size = 0;
/*
    printf("Header Info:\n");
    printf("%75s", "==============================\n");
    sprintf(string_buffer, "ID3V2 Identifier : %s\n", h.id3);
    printf("%75s", string_buffer);
    sprintf(string_buffer, "ID3V2 Version : Major : %hhd, Minor : %hhd\n", h.ver[0], h.ver[1]);
    printf("%75s", string_buffer);
    sprintf(string_buffer, "ID3V2 Flags : %hhX\n", h.flags);
    printf("%75s", string_buffer);
    sprintf(string_buffer, "ID3V2 Tag Size : %d\n", h.calc_size);
    printf("%75s", string_buffer);
    
    printf("%75s", "==============================\n");
*/  
    // Get the frame infos.. i.e, write the frames.
    // first read all tag bytes

    if (argc >= 2) {
        // the check for -v needs to be done.
        if ( ( res = strcmp(argv[1], "-v") ) == 0 ) {

            mp3_file = fopen(argv[2], "r");
            if (mp3_file == NULL) {
                printf("File doesn't exist. Terminating...\n");
                return -1;
            } else {
                read_header(&h, mp3_file);
                if ( (res = strcmp(h.id3, "ID3")) == 0 ) {
                    // pass
                } else {
                    printf("Invalid mp3 file. Terminating...\n");
                    goto end;
                }
            }
    
            all_frames = (char *)malloc(h.calc_size);
        
            fseek(mp3_file, 10, SEEK_SET); // go to the 10'th byte where your tag is stored.
            fread(all_frames, h.calc_size, 1, mp3_file);
                
            for (int i = 0; i < 6; i++) {
                extract_frames(all_frames, h.calc_size, relavant_frames[i], (frames + i));
            }
        
            puts("====================================================");
            printf("%35s", "Music Metadata: \n");
            puts("====================================================");
            printf("%-25s: %-25s\n", "Title", frames[0].frame_string);
            printf("%-25s: %-25s\n", "Artist", frames[1].frame_string);
            printf("%-25s: %-25s\n", "Album", frames[2].frame_string);
            printf("%-25s: %-25s\n", "Year", frames[3].frame_string);
            printf("%-25s: %-25s\n", "Music", frames[4].frame_string);
            printf("%-25s: %-25s\n", "Comment", frames[5].frame_string);
            puts("====================================================");
        
            for (int i = 0; i < 6; i++) {
                free(frames[i].frame_string);
                frames[i].frame_string = NULL;
            }
        } else if (( res = strcmp(argv[1], "-e") ) == 0 ) {
           
           if (!argv[2]) { // argv[2] is NULL.
               printf("No Edit Option Passed. Terminating...\n");
               goto end;
           } else {
               // pass
           }

           if ( (res = strcmp(argv[2], "-t")) == 0 ) {
               edit_option = 0;
           } else if ( (res = strcmp(argv[2], "-a")) == 0 ) {
               edit_option = 1;
           } else if ( (res = strcmp(argv[2], "-A")) == 0 ) {
               edit_option = 2;
           } else if ( (res = strcmp(argv[2], "-y")) == 0 ) {
               edit_option = 3;
           } else if ( (res = strcmp(argv[2], "-m")) == 0 ) {
               edit_option = 4;
           } else if ( (res = strcmp(argv[2], "-c")) == 0 ) {
               edit_option = 5;
           } else {
               printf("Invalid Edit Option Selected. Terminating...\n");
               goto end;
           }

           if (!argv[3]) {
               change_string = "Default edit text.";
           } else {
               // pass -> the user has passed in something.
               change_string = argv[3];
               if ( edit_option == 3 ) { // 3 levels Deep! >:-(
                   if ( ( ( year_len = strlen(argv[3]) ) == 4 ) && ( ( year_int = atoi(argv[3]) ) != 0 ) ) {
                       // pass
                   } else {
                       printf("Invalid Year Passed. Terminating...\n");
                       goto end;
                   }
               } else {
                   // pass
               }
           }

           mp3_file = fopen(argv[4], "r+");
           if (mp3_file == NULL) {
               printf("File doesn't exist.\n");
               return -1;
           } else {
               read_header(&h, mp3_file);               
               if ( (res = strcmp(h.id3, "ID3")) == 0 ) {
                   // pass
               } else {
                   printf("Invalid mp3 file. Terminating...\n");
                   goto end;
               }

           }
           
           // get the full size of mp3 file.
           fseek(mp3_file, 0, SEEK_END);
           file_size = ftell(mp3_file);
           --file_size; // Extra EOF removal.
           rewind(mp3_file);

           // Read the tag
           tag = (unsigned char *)malloc( (10 + h.calc_size) * sizeof(char));
           fread(tag, ( ( 10 + h.calc_size ) * sizeof(char) ), 1, mp3_file);

           // Read the music part.
           music_data = (unsigned char *)malloc(file_size - h.calc_size - 10); // file size - (header + remaining tag.)
           fread(music_data, ( (file_size - h.calc_size - 10) * sizeof(char) ), 1, mp3_file);

           // Edit the tag and write it.
           rewind(mp3_file);
           edit_frame_and_write_tag(tag, ( 10 + h.calc_size ), relavant_frames[edit_option], change_string, mp3_file);
           
           // finally write the music file.
           fwrite(music_data, (file_size - h.calc_size - 10) * sizeof(char), 1, mp3_file);
           fflush(mp3_file);
           printf("Edited and written Successfully.\n");

        } else if ( (res = strcmp(argv[1], "--help")) == 0 ) {
            puts("====================================================");
            printf("%30s", "HELP PAGE\n");
            puts("====================================================");
            puts("1. -v -> to view mp3 tag contents.");
            puts("2. -e -> to edit mp3 tag contents.");
            puts("\t\t2.1. -t -> to edit song title.");
            puts("\t\t2.2. -a -> to edit artist name.");
            puts("\t\t2.3. -A -> to edit album name.");
            puts("\t\t2.4. -y -> to edit year.");
            puts("\t\t2.5. -m -> to edit content.");
            puts("\t\t2.6. -c -> to edit comment.");
            puts("====================================================");
        } else {
            printf("=====================USAGE :========================\nView: ./a.out -v <mp3_file_name>\n");
            printf("Edit: ./a.out -e -t/-a/-A/-m/-y/-c <to_change_text> <mp3_file_name>\n");
            printf("Help: ./a.out --help\n");
            puts("====================================================");
        }

    } else {
        printf("=====================USAGE :========================\nView: ./a.out -v <mp3_file_name>\n");
        printf("Edit: ./a.out -e -t/-a/-A/-m/-y/-c <to_change_text> <mp3_file_name>\n");
        printf("Help: ./a.out --help\n");
        puts("====================================================");
    }

    //fwrite(h.id3, sizeof(char [3]), 1, tag);
    //fwrite(h.ver, sizeof(char [2]), 1, tag);
    //fwrite(&h.flags, sizeof(char), 1, tag);
    //fwrite(&h.calc_size, sizeof(int), 1, tag); // This was wrong!
    //fwrite(all_frames, h.calc_size, 1, tag);
    //fflush(tag);

end:
    if (mp3_file) {
        fclose(mp3_file);
        mp3_file = NULL;
    } else {
        // pass
    }
    //fclose(tag);
    if (all_frames) {
        free(all_frames);
        all_frames = NULL;
    } else {
        // pass
    }
    
    if (music_data) {
        free(music_data);
        music_data = NULL;
    } else {
        // pass
    }

    if (tag) {
        free(tag);
        tag = NULL;
    } else {
        // pass
    }

    return 0;
}
