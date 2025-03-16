#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

// BEGIN My definitions
// END My definitions

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

// Emertxe Definitions

Status open_files(EncodeInfo *encInfo)
{
    // open the file associated with the string name within struct.
    // Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    if (encInfo->fptr_src_image != NULL) {
        // pass
    } else {
        printf("Failed to open bitmap image file.\n");
        return e_failure;
    }
    // Secret Text file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    if (encInfo->fptr_secret != NULL) {
        // pass
    } else {
        printf("Failed to open secret text file.\n");
        return e_failure;
    }
    // Default file for writing..
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    if (encInfo->fptr_stego_image != NULL) {
        // pass
    } else {
        printf("Failed to open %s file.\n", encInfo->stego_image_fname);
        return e_failure;
    }

    return e_success;
}

Status encode_byte_to_lsb(unsigned char data, char *image_buffer)
{
    unsigned char lsb = 0;
    for (int i = 7; i >= 0; i--) {
        lsb = ( data & (1 << i) ) >> i;
        if ( lsb == 0 ) {
            //printf("%hhX, %hhX\n", image_buffer[7 - i], lsb);
            lsb = 1;
            image_buffer[7 - i] &= (~lsb);
            //printf("->%hhX\n", image_buffer[7 - i]);
        } else {
            //printf("%hhX, %hhX\n", image_buffer[7 - i], lsb);
            image_buffer[7 - i] |= lsb;
            //printf("->%hhX\n", image_buffer[7 - i]);
        }
    }

    return e_success;
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char *buf = malloc( 8 * sizeof(char) ); // 8 bytes Hardcoded
    Status s = e_success;

    if (!buf) {
        printf("encode_data_to_image: C allocate buffer.\n");
        goto end;
    } else {
        // pass
    }

    for (int i = 0; i < size; i++) {
        // first read the data...
        // Error handling here too necessary
        fread(buf, 8, 1, fptr_src_image);        
        
        // then encode
        s = encode_byte_to_lsb(data[i], buf);
        if (s == e_failure) {
            printf("encode_data_to_image: encode_byte_to_lsb returned failure.\n");
            goto end;
        } else {
            // pass
        }
        
        // then write the encoded buffer to output steg file
        // Error handling necessary below...
        fwrite(buf, 8, 1, fptr_stego_image);
    }
    fflush(fptr_stego_image); // clear the buffer.

end:
    
    if (buf) {
        free(buf);
    } else {
        // pass
    }

    return s;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    int len_ms = strlen(magic_string); // The length of magic string
    // The warning for the below line of code iif not typecast.
    encode_data_to_image((char *)magic_string, len_ms, encInfo->fptr_src_image, encInfo->fptr_stego_image);
    // This needs error handling refinement.
    return e_success;
}

Status encode_secret_file_info(File_Type ft, EncodeInfo *encInfo)
{
    encode_data_to_image((char *)&ft, sizeof(ft), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    // This needs error handling refinement.
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo) //Note: The size your'e giving is size_t -> unsigned long...
{
    encode_data_to_image( (char *)&file_size, sizeof(file_size), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    // This needs error handling
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char ch = '\0';
    for (int i = 0; i < (encInfo->size_secret_file - 1); i++ ) { // 25 - 1 -> 24 chars... so 0 to 23.
        ch = fgetc(encInfo->fptr_secret);
        encode_data_to_image(&ch, 1, encInfo->fptr_src_image, encInfo->fptr_stego_image);
        // error handling needed.
        // Not the best way of using buffering.
    }

    return e_success;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    unsigned int header_size_offset = 0;
    char *buf = NULL; // I'll be using a heap allocated buffer to copy the header... as it's the most portable.
    // rewind the src pointer image.
    rewind(fptr_src_image);

    // Now Get the actual offset of the image raster data... (From beginning of file.)
    fseek(fptr_src_image, 10, SEEK_SET);
    fread(&header_size_offset, sizeof(unsigned int), 1, fptr_src_image);

    // rewind the file pointer again.
    rewind(fptr_src_image);

    buf = (char *)malloc(header_size_offset);
    if (!buf) {
        printf("Failed to allocate buffer.\n");
        return e_failure;
    } else {
        // pass
    }

    // More error handling needed below.. fread return value
    fread(buf, header_size_offset, 1, fptr_src_image);
    //printf("copy_bmp_header : Source img ptr at %lX\n", ftell(fptr_src_image)); // It will too get updated post reading/writing.

    // now write the buffer to the Steg.bmp file.
    // More error handling needed below.. fwrite return value 
    fwrite(buf, header_size_offset, 1, fptr_dest_image);
    fflush(fptr_dest_image); // force write buffered data to file.
    //printf("copy_bmp_header : Destination img ptr at %lX\n", ftell(fptr_dest_image)); // pointing post header bytes.

    free(buf);
    buf = NULL;

    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch = '\0';
    long cur_pos = ftell(fptr_src), end_pos = 0;
    fseek(fptr_src, 0, SEEK_END);
    end_pos = ftell(fptr_src);

    fseek(fptr_src, cur_pos, SEEK_SET);

    for ( ; cur_pos < end_pos; cur_pos++ ) {
        ch = fgetc(fptr_src);
        fputc(ch, fptr_dest);
    }

    fflush(fptr_dest);

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    Status s = open_files(encInfo);
    if (s == e_success) {

        encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
        printf("Image Capacity : %lu\n", encInfo->image_capacity);

        encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
        printf("Secret file size : %lu\n", encInfo->size_secret_file);
        
        s = check_capacity(encInfo);
        if (s == e_success) {
            // AOK. proceed with encoding.
            printf("Encoding...\n");

            // first copy the header...
            s = copy_bmp_header( encInfo->fptr_src_image, encInfo->fptr_stego_image );
            if (s == e_success) {  
                printf("Copied The header.\n");
            } else {
                printf("Failed to copy header.\n");
                return e_failure;
            }
            // then do the actual encode parsing then add the encoded parts to the file.
            // Encode the magic string
            s = encode_magic_string( MAGIC_STRING, encInfo );
            if (s == e_success) {
                printf("Encoded Magic String.\n");
            } else {
                printf("Failed to encode magic string.\n");
                return e_failure;
            }
            // Encode the text file info
            s = encode_secret_file_info( encInfo->ft, encInfo );
            if (s == e_success) {
                printf("Encoded Secret file info.\n");
            } else {
                printf("Failed to encode secret file info.\n");
                return e_failure;
            }

            // Encode the file content size i.e, size_secret_file
            s = encode_secret_file_size( encInfo->size_secret_file, encInfo ); // Type promotion is happening here
            if (s == e_success) {
                printf("Encoded Secret file size.\n");
            } else {
                printf("Failed to encode secret file size.\n");
                return e_failure;
            }
            
            // Encode the contents.
            s = encode_secret_file_data(encInfo);
            if (s == e_success) {
                printf("Encoded Secret Data Successfully.\n");
            } else {
                printf("Failed to encode secret data.\n");
            }
            
            // then copy whatever that isn't encoded...
            s = copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image);
            if (s == e_success) {
                printf("Copied Unencoded bytes Successfully.\n");
            } else {
                printf("Failed Copy of unencoded bytes.\n");
                return e_failure;
            }

        } else {
            printf("The secret file cannot be encoded. Out of capacity.\n");
            return e_failure;
        }


    } else {
        printf("do_encoding: Open files failed to open.\n");
        return e_failure;
    }

    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    // Capacity needed = Magic String -> 16 bytes
    //                 + FileTypeInfo -> 32 bytes min (My Idea)//, 64 bytes max (Sir's Idea)
    //                 + SecretSizeInfo -> 64 Bytes // since it's unsigned long
    //                 + Secret Data -> Secret file Size * 8 Bytes

    // If image size (in bytes > Capacity Needed) -> return True
    // Else Return False.
    
    size_t capacity_needed = 16 + 32 + 64 + ( (encInfo->size_secret_file) * 8);

    if ( encInfo->image_capacity >= capacity_needed ) {
        return e_success;
    } else {
        return e_failure;
    }
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;// 3 Bytes per pixel.  // 3 is for RGB... each 8 bits.
}

uint get_file_size(FILE *fptr_file)
{
    size_t size = 0;
    fseek(fptr_file, 0, SEEK_END);
    size = ftell(fptr_file); // This will return the full size in bytes including the EOF character.
    // The below line is important...
    rewind(fptr_file); // for future encoding it's best to start from the beginning.
    
    return size;
}

OperationType check_operation_type(char *argv[])
{
    int l_value = -1;
    if ( ( l_value = strcmp( argv[1], "-e") ) == 0 ) {
        return e_encode;
    } else if ( ( l_value = strcmp(argv[1], "-d")) == 0 ) {
        return e_decode;
    } else {
        return e_unsupported;
    }
}

Status read_and_validate_encode_args( char *argv[], EncodeInfo *encInfo)
{
    if (is_valid_bmp(argv[2])) { // Not Null
        encInfo->src_image_fname = argv[2];
    } else {
        printf("Invalid image file extension. Only \".bmp\" file supported.\n");
        return e_failure;
    }

    if ( is_valid_txt(argv[3]) ) {
        encInfo->secret_fname = argv[3];
        encInfo->ft = txt; // ft is file type.
    } else if ( is_valid_c( argv[3] )) {
        encInfo->secret_fname = argv[3];
        encInfo->ft = c;
    } else if ( is_valid_sh( argv[3] ) ) {
        encInfo->secret_fname = argv[3];
        encInfo->ft = sh;
    } else {
        printf("Invalid secret file extension. Only \".txt\" or \".c\" or \".sh\" files supported.\n");
        return e_failure;
    }
    
    if (argv[4]) {
        if (is_valid_bmp(argv[4])) {
            encInfo->stego_image_fname = argv[4];
        } else {
            printf("The requested steg file name is invalid. Going with default name.\n");
            encInfo->stego_image_fname = "Stegged.bmp";
        }
    } else {
        encInfo->stego_image_fname = "Stegged.bmp";
    }
    
    return e_success;
}
