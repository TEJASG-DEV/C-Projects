#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    encInfo.src_image_fname = NULL;
    encInfo.secret_fname = NULL;
    encInfo.stego_image_fname = NULL;
    //uint img_size;    
    
    Status s;
    OperationType oper;
    
    if (argc > 2) {
        oper = check_operation_type(argv);
        if ( oper == e_encode ) {
            if (argc > 3) {
              printf("Encode selected.\n");
              s = read_and_validate_encode_args(argv, &encInfo);
              if (s == e_success) {
                do_encoding(&encInfo);
              } else {
                // pass
              }
            } else {
              printf("Usage : encode -[e or d] <filename>.bmp <filename>.txt <optional>.bmp .\n");
            }
        } else if ( oper == e_decode ) {
            printf("Decode selected.\n");
            s = read_and_validate_decode_args(argv, &encInfo);
            if (s == e_success) {
                do_decoding(&encInfo);
            } else {
                // pass
            }
        } else {
            printf("This is not supported.\n");
        }
    } else {
        printf("Usage : encode -[e or d] <filename>.bmp <filename>.txt <optional>.bmp .\n");
    }
    
    // close all the file pointers associated with the encodeInfo structure.
    // The if statements make it less error prone.
    if (encInfo.fptr_src_image) {
        fclose(encInfo.fptr_src_image);
        encInfo.fptr_src_image = NULL;
    } else {
        // pass
    }
    
    if (encInfo.fptr_secret) {
        fclose(encInfo.fptr_secret);
        encInfo.fptr_secret = NULL;
    } else {
        // pass
    }
    
    if (encInfo.fptr_stego_image) {
        fclose(encInfo.fptr_stego_image);
        encInfo.fptr_stego_image = NULL;
    } else {
        // pass
    }
    

    return 0;
}
