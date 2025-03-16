#ifndef TYPES_H
#define TYPES_H

// My Definitions BEGIN

typedef enum {
  txt = 5, // for the file type number to be unique.
  c,
  sh
} File_Type;

// My Definitions END

// Emertxe Guy's Definitions Below:
/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;

#endif
