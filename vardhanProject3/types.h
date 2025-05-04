#ifndef TAGS_H
#define TAGS_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    mp3_success, //for success
    mp3_failure  //for failure
} Status;

typedef enum //enum for check operation type
{
    mp3_view, //for viewing
    mp3_edit, //for editing
    mp3_unsupported, //for unsupported
    mp3_help
} OperationType;

#endif