#ifndef READER_H
#define READER_H
#include <stdio.h>
#include "types.h"
#include <string.h>
#include <stdlib.h>

typedef struct
{
  char* ptr[6]; //to store the tags of mp3
char* mp3_file_name; //to store the file name of the .mp3
 FILE* mp3_fptr; //to store the file pointer of .mp3
 char* title; //to store the title data 
 uint title_size; //to store the  size of the title data

}Treader;
//to open the file of .mp3
Status openfiles(Treader* reader);
//read_and validate function to validate the given file is .mp3 or not
Status read_and_validate(Treader* reader,char* argv[]);

//Status check_size(Treader* reader,FILE *file);
//function for reading the data fro the .mp3 file
Status read_data_mp3(Treader* reader);
//check the tags whether it is prent in my file or not
Status check_tag(Treader* reader,FILE* fptr);
//to check the size of the data 
Status check_title_size(Treader* reader,FILE* fptr);
// to check the operation which we are going to do it is view or edit
OperationType check_operation(char* symbol);
#endif
