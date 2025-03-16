#ifndef __DECODE_H__
#define __DECODE_H__

// The below include guard is necessary.
/*
    The program needs both encode.h and decode.h files. they may be included in any order.
    so typedefs twice will give fatal errors... so use include guards...
*/
#ifndef ENCODE_H
typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname; // "Beautiful.bmp" <- name of the file
    FILE *fptr_src_image; // To refer to the actual file.
    size_t image_capacity;
    //uint bits_per_pixel; // no need
    //char image_data[MAX_IMAGE_BUF_SIZE]; //no need

    /* Secret File Info */
    char *secret_fname; // .txt secret file.
    FILE *fptr_secret; // the actual secret text file pointer.
    File_Type ft;  // will treat this as an integer... 4 bytes.
    //char extn_secret_file[MAX_FILE_SUFFIX]; // no need
    //char secret_data[MAX_SECRET_BUF_SIZE]; // no need
    size_t size_secret_file;

    /* Stego Image Info */ // These are for the output file... default name: steg.bmp
    char *stego_image_fname; // output file name
    FILE *fptr_stego_image; // output file

} EncodeInfo; // .c .sh .txt files
#endif

// Some function declarations

Status read_and_validate_decode_args(char *argv[], EncodeInfo *encInfo);

File_Type get_secret_file_type(EncodeInfo *encInfo);

Status get_secret_file_size(EncodeInfo *encInfo);

Status do_decoding(EncodeInfo *encInfo);

Status decode_byte_from_bytes(unsigned char *ch, FILE *fptr_steg_image);

#endif
