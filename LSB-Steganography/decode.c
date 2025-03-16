#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "decode.h"
#include "common.h"

// BEGIN All my definitions

static int heap_allocated_str = 0; // for the read_and_validate_decode_args heap string

// BEGIN Validation definitions:

static int is_valid_bmp(const char *f_name)
{
    int cmp_res = -1;
    char *found = NULL;
    found = strchr(f_name, '.');
    if (found) {    
        if ( ( ( cmp_res = strcmp( (found + 1) , "bmp" ) ) == 0 ) ) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

static int is_valid_txt(const char *f_name)
{
    int cmp_res = -1;
    char *found = NULL;
    found = strchr(f_name, '.');
    
    if (found) {
    
        if ( ( ( cmp_res = strcmp( (found + 1) , "txt" ) ) == 0 ) ) {
            return 1;
        } else {
            return 0;
        }
    
    } else {
        return 0;
    }

}

static int is_valid_c(const char *f_name)
{
    int cmp_res = -1;
    char *found = NULL;
    found = strchr(f_name, '.');
    if (found) {
        if ( ( ( cmp_res = strcmp( (found + 1) , "c" ) ) == 0 ) ) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

static int is_valid_sh(const char *f_name)
{
    int cmp_res = -1;
    char *found = NULL;
    found = strchr(f_name, '.');
    if (found) {
        if ( ( ( cmp_res = strcmp( (found + 1) , "sh" ) ) == 0 ) ) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

static int is_image_encoded(FILE *image_file)
{
    unsigned char magic_string_data[3] = { 0 };
    int image_offset = 0, cmp_res = -1;
    Status s;

    fseek(image_file, 10, SEEK_SET); // error handling can be added...
    // if empty bmp file then problems should arise
    fread(&image_offset, sizeof(int), 1, image_file); // error handling needed for all file ops.
    
    printf("Image offset = %d\n", image_offset);
    fseek(image_file, image_offset, SEEK_SET);
    
    for (int i = 0; i < 2; i++) {
        s = decode_byte_from_bytes( (magic_string_data + i), image_file );
        if (s == e_failure) {
            printf("is_image_encoded : Failed to decode.\n");
            return e_failure;
        } else {
            // pass
        }
    }
    printf("Magic string decoded = %s\n", magic_string_data);
    if ( !( cmp_res = strcmp((char *)magic_string_data, MAGIC_STRING) ) ) { // compare res == 0
        return 1;
    } else {
        return 0;
    }

}

void print_bits(unsigned char ch)
{
    for (int i = 7; i >= 0; i--) {
        printf("%d ", ( ( ch & (0x1 << i) ) >> i ));
    }
    puts("");
}

// END Validation definitions

Status decode_byte_from_bytes(unsigned char *ch, FILE *fptr_stego_image)
{
    char buf[8] = { 0 };
    int bit = 0;
    fread(buf, sizeof( char [8] ), 1, fptr_stego_image);

    for (int i = 0; i < 8; i++) {
        *ch <<= 1;
        //printf("buf[%d] = %hhd\n", i, buf[i] & 0x1);
        bit = buf[i] & 0x1;
        if (bit) {
            *ch |= 0x1;
        } else {
            *ch &= ~(0x1);
        }
        /*
        if (i < 7) {
            *ch <<= 1;
        } else {
            // pass
        }
        */
        //print_bits(*ch);
    }
    
    return e_success;
}

Status get_secret_file_size(EncodeInfo *encInfo)
{
    // file pointer exactly at the location you need..
    for (int i = 0; i < sizeof(size_t); i++) {
        // Error handling based on status codes...
        decode_byte_from_bytes( ( (unsigned char *)&(encInfo->size_secret_file) + i ), encInfo->fptr_stego_image); // potential segfault
    }
    encInfo->size_secret_file--;
    return e_success;
}

File_Type get_secret_file_type(EncodeInfo *encInfo)
{
    File_Type ft;
    for (int i = 0; i < sizeof(File_Type); i++) {
        // error handling needed.
        decode_byte_from_bytes( ( ( (unsigned char *)&ft ) + i ) , encInfo->fptr_stego_image );
    }
    return ft;
}

Status do_decoding(EncodeInfo *encInfo)
{
    get_secret_file_size(encInfo);
    // Status error handling needed.
    char *buf = malloc(encInfo->size_secret_file + 1);

    for (int i = 0; i < encInfo->size_secret_file; i++) {
        // error handling...
        decode_byte_from_bytes((unsigned char *)(buf + i), encInfo->fptr_stego_image);
    }

    printf("Decoded message: %s\n", buf);

    fwrite(buf, sizeof(char [encInfo->size_secret_file]), 1, encInfo->fptr_secret);

    if (heap_allocated_str) {
        free(encInfo->secret_fname);
    } else {
        // pass
    }

    if (buf) {
        free(buf);
    } else {
        // pass
    }

    return e_success;
}

Status read_and_validate_decode_args(char *argv[], EncodeInfo *encInfo)
{
    // Here the logic Will be very different than encode.
    // First you open the necessary files
    
    File_Type ft, arg_ft; // The encoded file type and the one inputted in argments..
    //Status s;

    char *default_file_name = NULL, *file_name_ext = NULL, *token = NULL;
    char *extensions[] = { ".txt" , ".c", ".sh" };
    int opt_len_argv = 0; // optional argument

    if (is_valid_bmp(argv[2])) {
        encInfo->fptr_stego_image = fopen(argv[2], "r");
        
        if (!encInfo->fptr_stego_image) { // NULL pointer check
            printf("The file %s doesn't exist and cannot be opened.\n", argv[2]);
            return e_failure;
        } else {
            // Verify weather image is encoded.
            printf("Verifying weather image is encoded...\n");
            if ( is_image_encoded(encInfo->fptr_stego_image) ) {
                // Encoded now to get file type
                printf("Input file is encoded... Getting file type.\n");
                ft = get_secret_file_type(encInfo);
                switch (ft) {
                    case txt:
                        printf("Encoded file is a text file.\n");
                        default_file_name = "Decoded.txt";
                        break;
                    case c:
                        printf("Encoded file is a c file.\n");
                        default_file_name = "Decoded.c";
                        break;
                    case sh:
                        printf("Encoded file is a sh file.\n");
                        default_file_name = "Decoded.sh";
                        break;
                    default:
                        printf("This file type doesn't exist : %d\n", ft);
                }
                
                encInfo->ft = ft;

                printf("Validating passed extension...\n");
                if (argv[3]) {
                    opt_len_argv = strlen(argv[3]);
                    
                    if ( is_valid_txt( argv[3] ) ) {
                        arg_ft = txt;
                        
                        if ( arg_ft == ft ) {
                            encInfo->secret_fname = argv[3];
                        } else {
                            file_name_ext = malloc( (sizeof(char) * opt_len_argv) + 4); // Allocate a large enough buffer.
                            token = strtok(argv[3], ".");
                            strcpy(file_name_ext, token);
                            strcat(file_name_ext, extensions[ ft - 5 ]);
                            encInfo->secret_fname = file_name_ext;
                        }

                    } else if ( is_valid_c( argv[3] ) ) {
                        arg_ft = c;
                        
                        if ( arg_ft == ft ) {
                            encInfo->secret_fname = argv[3];
                        } else {
                            file_name_ext = malloc( (sizeof(char) * opt_len_argv) + 2);
                            token = strtok(argv[3], ".");
                            strcpy(file_name_ext, token);
                            strcat(file_name_ext, extensions[ft - 5]);
                            encInfo->secret_fname = file_name_ext;
                        }

                    } else if ( is_valid_sh( argv[3] ) ) {
                        arg_ft = sh;
                        
                        if ( arg_ft == ft ) {
                            encInfo->secret_fname = argv[3];
                        } else {
                            file_name_ext = malloc( (sizeof(char) * opt_len_argv) + 3);
                            token = strtok(argv[3], ".");
                            strcpy(file_name_ext, token);
                            strcat(file_name_ext, extensions[ft - 5]);
                            encInfo->secret_fname = file_name_ext;
                        }

                    } else {
                        printf("Proper file name not specified. Default output considered.\n");
                        encInfo->secret_fname = default_file_name;
                    }

                } else {
                    printf("No optional arguments... Default output considered.\n");
                    encInfo->secret_fname = default_file_name;
                }
                // Debug mesg below...
                //printf("Secret file name = %s, Token = %s \n", encInfo->secret_fname, token);
                
                encInfo->fptr_secret = fopen(encInfo->secret_fname, "w");
                 
            } else {
                printf("The current image is not encoded.\n");
                return e_failure;
            }     
        }

    } else {
        printf("Invalid image file extension. Only \".bmp\" file supported.\n");
        return e_failure;
    }
    
    if ( file_name_ext ) {
        heap_allocated_str = 1; // set the global var
    } else {
        // pass
    }

    return e_success;
}

// END All my definitions
