// This function is used in frame read and header read
static int calculate_size(unsigned char bytes[]) // to calculate the actual size from the decoded raw size.
{
    // say bytes are 00    00   02   01
    //                *     *    *    *
    //             2^21  2^14  2^7  2^0
    //               ||    ||   ||   ||
    //               0     0    256  1   => size = 257.
    int size = 0;

    for (int i = 3; i >= 0; i--) {
        //printf("%d\n", bytes[3 - i]);
        size += bytes[3 - i] * (0x1 << (7 * i));
    }
    //printf("Size = %d\n", size);
    return size;
}

