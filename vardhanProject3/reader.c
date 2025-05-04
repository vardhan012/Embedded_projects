#include "reader.h"

Status openfiles(Treader* reader) //function definition for opening the
{
   reader->mp3_fptr=fopen(reader->mp3_file_name,"r");
   if(reader->mp3_fptr==NULL) //if it is returning null address
   {
    printf("ERROR: FILE IS NOT FOUND\n");
    return mp3_failure;
   }
   return mp3_success;
}

Status read_and_validate(Treader* reader,char* argv[]) //function definition for read and validate
{  
    if((strstr(argv[2],".mp3"))!=NULL) // strstr to check the .mp3 file or not
    {
        
        reader->mp3_file_name=argv[2]; //copy the file name to the mp3_sile_name
       return mp3_success; //returning mp3_success
    }
    else{
        printf("ERROR: you should pass the file name\n");
    }
    return mp3_failure; //returning mp3_failure
}

Status read_data_mp3(Treader* reader)
{
    if(openfiles(reader)!=mp3_success) //function call to open the files if it is returning the mp3_failure print error and return mp3_failure
    {
        return mp3_failure;
    }
    printf("INFO: Files opened successfully\n\n");
     
    char*p[6]={"TIT2","TPE1","TALB","TYER","TCON","COMM"}; // declare the array of strings
    for(int i=0;i<6;i++) //loop to copy the data into the array of pointer
    {
        reader->ptr[i]=p[i]; 
    }
    fseek(reader->mp3_fptr,10,SEEK_SET); //fseek to shift the pointer variable
    printf("--------------------------------------------------------------------\n");
    printf("MP3 Tag Reader And Editor for ID3 2.3 version\n");
    printf("--------------------------------------------------------------------\n");
    for(int i=0;i<6;i++) // to read the data from the mp3 file
    {
    if(check_tag(reader,reader->mp3_fptr)!=mp3_success) //function call for the heck tags whether it is present or not
    {
        printf("ERROR: there are no tags in it\n"); // it not return mp3_success then print error and return mp3_failure
        return mp3_failure;
    }
    if(check_title_size(reader,reader->mp3_fptr)!=mp3_success) // function call for check the title size
    {
        printf("ERROR: uable to get size\n");  // it not return mp3_success then print error and return mp3_failure
        return mp3_failure;
    }
    fseek(reader->mp3_fptr,3,SEEK_CUR); //to skip the three bytes
    char buffer[reader->title_size];
    fread(buffer,reader->title_size-1,1,reader->mp3_fptr); //fread to read the size bytes from the mp3 file and store it in the buffer
    buffer[reader->title_size-1]='\0';
    switch(i) //switch case to print particular tag data
    {
        case 0:
        printf("TILTLE     : %s\n",buffer);
        break;
        case 1:
        printf("ARTIST     : %s\n",buffer);
        break;
        case 2:
        printf("ALBHUM     : %s\n",buffer);
        break;
        case 3:
        printf("YEAR       : %s\n",buffer);
        break;
        case 4:
        printf("GENRE      : %s\n",buffer);
        break;
        case 5:
        printf("LANGUAGE   : %s\n",buffer);
        break;
        default:
        break;
    }
}
printf("--------------------------------------------------------------------\n");
return mp3_success;
}
Status check_tag(Treader* reader,FILE* fptr) //function definition for check tag function
{ char buffer[4];
fread(buffer,4,1,fptr); //fread to read 4 bytes from the file
 for(int i=0;i<6;i++) //loop to check from my array of pointers
 {
    if(strcmp(reader->ptr[i],buffer)==0)  //to compre the string from my array of pointers
    {
        return mp3_success;
    }
 }
 return mp3_failure;
}
Status check_title_size(Treader* reader,FILE* fptr) //check title size function definition
{
    uint size=0;
    char buffer[4];
    fread(buffer,4,1,fptr); // read four bytes from the file pointer by fread function
   // size = (buffer[0] << 24) |(buffer[1] << 16) |(buffer[2] <<8) |(buffer[3]); //converting number from big endian to littlt endian from buffer
   for(int i=0;i<2;i++) //loop converting number from big endian to littlt endian from buffer
   {
    char temp=buffer[i];
    buffer[i]=buffer[4-i-1];
    buffer[4-i-1]=temp;
   }
   size=buffer[0]|buffer[1]|buffer[2]|buffer[3];
    reader->title_size=size; //store the number after converting it
    return mp3_success;
}
