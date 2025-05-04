#ifndef EDIT_H
#define EDIT_H
#include <stdio.h>
#include "types.h"
#include <string.h>
#include <stdlib.h>
typedef struct
{
  char* ptr_edit[6]; //to store the tags of mp3
char* source_name; //to store the file name of the .mp3
 char tag[5];
 char* dest_filen;
 char* data;
 uint data_len;
 FILE* mp3_fptr_src; //to store the file pointer of .mp3
 FILE* mp3_dest;
 char* title_card; //to store the title data 
 uint title_card_size; //to store the  size of the title data
 uint size_data;

}Edit;
//to open the file of .mp3
Status openfile(Edit* edit);
//read_and validate function to validate the given file is .mp3 or not
Status read_and_validate_args(Edit* edit,char* arg[]);

//Status check_size(Treader* reader,FILE *file);
//function for reading the data fro the .mp3 file
Status read_data_mp3_enc(Edit* edit);
//check the tags whether it is prent in my file or not
Status check_tag_mp3(Edit* edit,FILE* fptr);

Status insert_edit_size(Edit* edit,FILE* fptr);

Status insert_data(Edit* edit,FILE* fptr);
//to check the size of the data 
Status get_title_size(Edit* edit,FILE* fptr);
// to check the operation which we are going to do it is view or edit
Status reamaining_data(FILE* fptr1,FILE* fptr2);

Status get_size(Edit* edit,FILE* fptr);
#endif
