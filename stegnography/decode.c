#include <stdio.h>
#include "decode.h"
#include "types.h"
#include<string.h>
#include "common.h"
#include "encode.h"
#include<stdlib.h>

//Function definition for read and validate arguements
Status read_and_validate_decode_args(char *argv[], decodeInfo *decInfo)
{
    if(strstr(argv[2],".bmp") != NULL) 
    {
        decInfo->dest_image_fname = argv[2]; 
    }
    else
    {
        return e_failure;//return e_failure
    }
    if(argv[3]!= NULL) 
    {
        decInfo->dup = strtok(argv[3],"."); //check for Delimiters
    }
    else
    {
        decInfo->dup = "vardhan"; // create a file with any name
    }
   return e_success; // return e_success
}
Status open_file(decodeInfo *decInfo) // opening the files function
{
    decInfo->fptr_dest_image = fopen(decInfo->dest_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_dest_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->dest_image_fname);

    	return e_failure; //return e_failure
    }
    return e_success; //return e_success
}
Status decode_magic_string(decodeInfo *decInfo)
{
    // Decode the magic string from the encoded file
    char magic[2];
    if(decode_data_from_image(magic, 2,decInfo->fptr_dest_image)==e_failure)
    {
        return e_failure; //return e_failure
    }
    //printf("magic string encoded successfully\n");
    strcpy(decInfo->magic_string,magic); // using strcpy copy the magic string to the structure variable
    printf("magic_string decoded sucesfully\n");
}
Status decode_byte_from_lsb(char* data, char *decode_buffer)
{
    int bit=0;
    for(int i=0;i<8;i++) // run a loop up to 8 times 
    {
        int res = decode_buffer[i] & 1;
        res = res <<i;
        bit = res | bit;
    }
    *data = bit;
}
Status decode_data_from_image(char *data, int size, FILE *fptr_dest_image)
{
    char decode_buffer[8];
    for(int i=0;i<size;i++) //run loop for size no. fo times
    {
        fread(decode_buffer,8,1,fptr_dest_image);
        decode_byte_from_lsb(&data[i], decode_buffer);
    }
}
Status decode_secret_file_extn_size(FILE *fptr_dest_image,decodeInfo *decInfo)
{
    char extn_size_buffer[32];
    fread(extn_size_buffer,32,1,fptr_dest_image);
    int res =0;
    for(int i=0;i<32;i++) //run loop for 32 times
    {
        int bit = extn_size_buffer[i] &1;
        bit = bit << i;
        res = res | bit;
    }
    decInfo->ext_size = res;
    return e_success;  //return e_success
}
Status decode_secret_file_extn(FILE *fptr_dest_image, decodeInfo *decInfo)
{
    char ext_b_data[decInfo->ext_size];
    decode_data_from_image(ext_b_data, decInfo->ext_size,fptr_dest_image);
    strcpy(decInfo->ext_data,ext_b_data);
    return e_success;    //return e_success
    
}
Status decode_secret_file_size(FILE *fptr_dest_image, decodeInfo *decInfo)
{
    char file_size_buffer[32];
    fread(file_size_buffer,32,1,fptr_dest_image);
    int res =0;
    for(int i=0;i<32;i++) //run loop for 32 times
    {
        int bit = file_size_buffer[i] &1;
        bit = bit << i;
        res = res | bit;
    }
    decInfo->secret_fsize = res;
    return e_success; // return e_success
}
Status decode_secret_file_data(decodeInfo *decInfo)
{
    char secret_b_data[decInfo->secret_fsize];
    printf("RES :  %u\n",decInfo->secret_fsize);
    decode_data_from_image(secret_b_data, decInfo->secret_fsize,decInfo->fptr_dest_image);
    fwrite(secret_b_data,decInfo->secret_fsize,1,decInfo->fptr_secret_data);
    return e_success;//return e_success
}
Status do_decoding(decodeInfo *decInfo)
{
    if(open_file(decInfo) == e_failure)
    {
        printf("File doesnt open\n"); 
        return e_failure; // return e_failure
    }
    printf("File opened succesfully\n");
    fseek(decInfo->fptr_dest_image,54,SEEK_SET);
    if(decode_magic_string(decInfo)==e_failure)
    {
        printf("Magic string does not decoded\n");
        return e_failure;// return e_failure
    }

    printf("Magic string was decoded succesfuuly\n"); //print that magic string was decoded
    
    if(decode_secret_file_extn_size(decInfo->fptr_dest_image,decInfo)==e_failure)
    {
        printf("Decoding size extension was not complted\n");
        return e_failure;// return e_failure
    }
    printf("Decoding extension size was success\n"); // print extension size decoded


    if(decode_secret_file_extn(decInfo->fptr_dest_image,decInfo)==e_failure)
    {
        printf("Extension data was not decoded\n");
        return e_failure;// return e_failure
    }
    printf("Extension the data succesfully\n"); //print the data

    decInfo->secret_file_name = malloc(strlen(decInfo->dup) + strlen(decInfo->ext_data) + 1);
    strcpy(decInfo->secret_file_name, decInfo->dup);
    strcat(decInfo->secret_file_name, decInfo->ext_data);
    
    if(decode_secret_file_size(decInfo->fptr_dest_image,decInfo)==e_failure)
    {
        printf("Secret data was not decoded\n");
        return e_failure;// return e_failure
    }
    printf("Secret size decoded successfully\n"); //print secret data size


    decInfo->fptr_secret_data = fopen(decInfo->secret_file_name, "w");
    if (decInfo->fptr_dest_image == NULL) 
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->dest_image_fname);

    	return e_failure;// return e_failure
    }
    if(decode_secret_file_data(decInfo)==e_failure)
    {
        printf("Secret file data not decoded\n");
        return e_failure;// return e_failure
    }
    printf("Decoded succesfully\n");
    return e_success;// return e_success


}
