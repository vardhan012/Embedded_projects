#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("Width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("Height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	//Step-1: Check argv[2] is .bmp or not, if FALSE return e_failure, if TRUE store argv[2] into the structure member
    int len = strlen(argv[2]);
    if(strcmp(&argv[2][len-4],".bmp") == 0)
    {
        encInfo->src_image_fname = argv[2];
    } 
    else{
        printf("ERROR::Please enter the '.bmp' file\n");
        return e_failure;
    }
	//Step-2: Check argv[3] is .txt or not, if FALSE return e_failure, if TRUE store argv[3] into the structure member
    int len1 = strlen(argv[3]);
    if((strstr(&argv[3][len1-4],".txt"))||(strstr(&argv[3][len1-2],".c"))||(strstr(&argv[3][len1-3],".sh"))!=NULL)
    {
        encInfo->secret_fname = argv[3];
    }
    else{
        printf("ERROR::Please Enter the '.txt' file\n");
        return e_failure;
    }
	//Step-3: Check argv[4] is NULL or not, if it is NULL then store the default.bmp into the structure member 
		// FALSE => Check whether the argv[4] is having the .bmp or not, if not return e_failure
		// TRUE => Store argv[4] into structure member
    if(argv[4]==NULL)
    {
        encInfo->stego_image_fname = "default.bmp";
    }
    else{
        int len3  = strlen(argv[4]);
        if(strcmp(&argv[4][len3-4],".bmp") == 0)
        {
            encInfo->stego_image_fname = argv[4];
        }
        else{
            return e_failure;
        }
    }
	//Step-4: if All conditions are valid arguements, return e_success
    return e_success; 
}
long get_file_size(FILE *fptr_secret)
{
    fseek(fptr_secret,0,SEEK_END);
    return ftell(fptr_secret);
}
Status check_capacity(EncodeInfo *encInfo)
{
    //Step-1: Call the get_image_size_for_bmp(encInfo->fptr_src_image) and  store it  into image_capacity
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    //Step-2: Call the get_file_size(encInfo->fptr_secret_file) and store it into size_secret_file
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    //Step-3: Image capacity > (54+16+32+32+32+(size_secret_file_data * 8))
    int len = strlen(MAGIC_STRING);
    char* ptr = strchr(encInfo->secret_fname,'.');
    strcpy(encInfo->extn_secret_file,ptr);
    encInfo->len_of_extn_secret_file = strlen(encInfo->extn_secret_file);

    if(encInfo->image_capacity > (54+(len*8)+(sizeof(encInfo->len_of_extn_secret_file)*8)+(strlen(encInfo->extn_secret_file)*8)+(sizeof(encInfo->size_secret_file)*8)+(encInfo->size_secret_file * 8)))
    {
            //if TRUE, print success message and return e_success
            printf("Image capacity is greater than sceret file size\n");
            return e_success;
    }
    else{
            //if FALSE print error message and return  e_failure 
        printf("Image capacity is less than secret file size\n");
        return e_failure;
    }
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    //char imageBuffer[54]
    unsigned char imageBuffer[54];
    //Step-1: Rewind the fptr_src_image
    rewind(fptr_src_image);

    //Step-2: Read the 54 bytes from the src image
    fread(imageBuffer,sizeof(imageBuffer),1,fptr_src_image);

    //Step-3: Copy the 54 bytes to fptr_dest_image
    if(fwrite(imageBuffer,sizeof(imageBuffer),1,fptr_dest_image) == 1)
    {
        //Step-4: return e_success 
        return e_success;
    }
    else{
        return e_failure; // return e_failure
    }
}
Status encode_size_to_lsb(int size,char* imageBuffer)
{
    for(int i=0;i<32;i++) // run loop for 32 times
    {
        int num = (size&(1<<i))>>i;
        imageBuffer[i] = (imageBuffer[i]&(~(1)))|num;
    }
}
long encode_size_lsb(int size,char* imageBuffer)
{
    for(int i=0;i<64;i++) // run loop for 64 times
    {
        unsigned int num = (size&(1<<i))>>i;
        imageBuffer[i] = (size&(~(1<<0)))|num;
    }
}
Status encode_secret_file_extn_size(int size,EncodeInfo* encInfo)
{
    //char imageBuffer[32]
    char imageBuffer[32];

    //Step-1: Read the 32 bytes from src file and store into the imageBuffer
    fread(imageBuffer,32,1,encInfo->fptr_src_image);

    //Step-2: Call the encode size to lsb(size,imageBuffer)
    encode_size_to_lsb(size,imageBuffer);

    //Step-3: Write the 32 bytes to the destination image
    int m = fwrite(imageBuffer,sizeof(imageBuffer),1,encInfo->fptr_stego_image);
    if(m==1)
    {
        return e_success;     //return e_success
    }
    else{
        return e_failure;    // return e_failure
    }
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    //Step-1: Call the encode_data_image(file_extn,sizeof the extn,file pointers)
    if(encode_data_to_image(file_extn,encInfo->len_of_extn_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
    {
        return e_success;   //return e_success
    }
    else{
        return e_failure;   // return e_failure
    }
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    //char imageBuffer[32]
    char imageBuffer[32];

    //Step-1: Read the 32 bytes from src file and store it into the imageBuffer
    fread(imageBuffer,32,1,encInfo->fptr_src_image);

    //Step-2: Call the encode size to lsb(size,imageBuffer)
    encode_size_to_lsb(file_size,imageBuffer);

    //step-3: Write the 32 bytes to the destination image
    int n = fwrite(imageBuffer,32,1,encInfo->fptr_stego_image);
    if(n == 1)
    {
        return e_success;             //return e_success
    }  
    else{
        return e_failure;             //return e_failure
    }                      
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    //char data[encInfo->size_secret_file]
    char data[encInfo->size_secret_file];
    //Step-1: Rewind(fptr_secret)
    rewind(encInfo->fptr_secret);
    //Step-2: Read the contents of the address secret.txt
    fread(data,encInfo->size_secret_file,1,encInfo->fptr_secret);
    //Step-3: Call the encode_data_image(data,encInfo->size_secret_file,file pointers)
    if(encode_data_to_image(data,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
    {
        return e_success;          //return e_success
    }
    else{
        return e_failure;          //return e_failure
    }
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    //Step-1: Read remaining the data character by character into the dest image upto the EOF
    char ch[1];
    while(fread(ch,1,1,fptr_src) > 0)
    {
        fwrite(ch,1,1,fptr_dest);
    }
    return e_success;             //return e_success
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    //Step-1: find the size of magic string
    char magic[]=MAGIC_STRING;
    int size = strlen(MAGIC_STRING);

    //Step-2: Call the function encode_data_to_image(magic_string,size,file pointers)
    if(encode_data_to_image(magic,size,encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)    //if it is returning e_success condition is true
    {
        return e_success; //return e_success
    }
    else{
        return e_failure; //retrun e_failure
    }
}
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    //char imageBuffer[8]
    char imageBuffer[8];
    //Step-1: Read 8 bytes from src image store into imageBuffer
    //Step-2: Call the encode_byte_to_lsb(data[i],imageBuffer)
    int n;
    for(int i=0;i<size;i++) //run loop upto size no. of times
    {
        fread(imageBuffer,8,1,fptr_src_image);
        encode_byte_to_lsb(data[i],imageBuffer);
        n = fwrite(imageBuffer,8,1,fptr_stego_image);
    }
    if(n == 1)
    {
        return e_success;
    }
    else{
        return e_failure;
    }
}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    //Write logic for encoding the data
    for(int i=0;i<8;i++) //run loop for 8 times
    {
        unsigned int num = (data&(1<<i))>>i;
        image_buffer[i] = (image_buffer[i]&(~(1)))|num;
    }
    //printf("This byte is successfully completed\n");
}
Status do_encoding(EncodeInfo *encInfo)
{
	//Step-1: Call the openfiles(encInfo)
    int res1 = open_files(encInfo);
    //Step-2: Check success or failure, if it is success goto the next step, if it is failure return e_failure
    if(res1 == e_success)
    {
        //Step-3: Call the check_capacity (encInfo) , if it is TRUE print success message go to next step, FALSE return e_failure
        if(check_capacity(encInfo) == e_success)
        {
            printf("Check capacity was success\n");
                //Step-4: Call copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_dest_image);
                //TRUE => print the success message go to the next step , FALSE => return e_failure
            if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
            {
                printf("Copying bmp header function was success\n");
                //Step-5: Call the encode_magic_string(MAGIC_STRING,encInfo) 
                //TRUE => print the success message go to next step, FALSE => print the error message return e_failure
                if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
                {
                    printf("Encode magic string was success\n");
                    //Step-6: Call the function Status encode_secret_file_extn_size(extn of the size,encInfo);
                            //TRUE => print the success message go to the next step , FALSE => print the error message , return e_failure
                    if(encode_secret_file_extn_size(encInfo->len_of_extn_secret_file,encInfo) == e_success)
                    {
                        printf("Encode secret file extensionn size was success\n");
                        //Step-7: Call the function encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);
                        //TRUE => print success msg goto the next step , FALSE => return e_failure
                        if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_success)
                        {
                            printf("Encode secret file extension was success\n");
                            //Step-8: Call the function encode_secret_file_size(encInfo->file_size, EncodeInfo *encInfo);
                                        //TRUE => print success message go to the next step , FALSE => return e_failure
                            if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
                            {
                                printf("Encode secret file size was success\n");
                                //step-9: call the function encode_secret_file_data(encInfo);
                                            //true => print success message go to the next step , FALSE => return e_failure
                                if(encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf("Encode secret file data was success\n");
                                    //Step-10: Call the function copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);
                                                //TRUE => print success message go to the next step , FALSE => retrun e_failure
                                    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
                                    {
                                        return e_success; //return e_success
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else{
        return e_failure;
    }    //Step-11: Close all the files         
}
