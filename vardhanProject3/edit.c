#include"edit.h"
#include<stdlib.h>


Status read_and_validate_args(Edit* edit,char* argv[]) //function definition for read and validate
{
    if(argv[4]==NULL)
    {
        printf("ERROR: Please pass the file name\n");
        return mp3_failure;
    }
   else if((strstr(argv[4],".mp3"))!=NULL) // strstr to check the .mp3 file or not
    {
        edit->source_name=argv[4]; //copy the file name to the mp3_sile_name 
     //returning mp3_success
    }
    else
    {
    printf("ERROR: Opening the file\n");
    return mp3_failure;
    } //returning mp3_failure
    if((strstr(argv[2],"-t")!=NULL)||(strstr(argv[2],"-a")!=NULL)||(strstr(argv[2],"-A")!=NULL)||(strstr(argv[2],"-m")!=NULL)||(strstr(argv[2],"-y")!=NULL)||(strstr(argv[2],"-c")!=NULL))// strstr to check the .mp3 file or not
    {
        strcpy(edit->tag,argv[2]); //copy the file name to the mp3_sile_name 
       
    }
    else
    {
    printf("ERROR: Opening the file\n");
    return mp3_failure;
    } 
    edit->data = malloc(strlen(argv[3]) + 1);
    if (edit->data == NULL) {
      printf("ERROR: Memory allocation failed\n");
      return mp3_failure;
      }
    strcpy(edit->data,argv[3]); //copy the data from the argv[3] and data
    edit->data_len=strlen(edit->data);
    return mp3_success;//returning mp3_success
}

Status openfile(Edit* edit) //function to open the files
{
    edit->mp3_fptr_src=fopen(edit->source_name,"r");
    if(edit->mp3_fptr_src==NULL) //if file pointer is null return mp3_failure
    {
        printf("ERROR: in opening the files\n");
        return mp3_failure;
    }
    edit->dest_filen="dest.mp3"; //default destination file
    edit->mp3_dest=fopen(edit->dest_filen,"w");
    if(edit->mp3_dest==NULL) //
    {

        printf("ERROR: in creating file\n");
        return mp3_failure;
    }
    return mp3_success;
}
Status read_data_mp3_enc(Edit* edit)
{
   // to read the data from the file and eddit the data
    if(openfile(edit)!=mp3_success)
    {
        printf("ERROR: in opening thye files\n");
        return mp3_failure;
    }
   
    char* ptr[6] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"}; 
    for (int i = 0; i < 6; i++) { // loop to copy the data from ptr[i]
        edit->ptr_edit[i] = ptr[i];
      }
    //-t,-a,-A,-m,-y,-c
    int index=0;
    if(strcmp(edit->tag,"-t")==0) //if else for the tag checking
    {
        index= 0;
    }
    else if(strcmp(edit->tag,"-a")==0)
    {
        index= 1;
    }
    else if(strcmp(edit->tag,"-A")==0)
    {
        index= 2;
    }
    else if(strcmp(edit->tag,"-y")==0)
    {
        index= 3;
    }
    else if(strcmp(edit->tag,"-m")==0)
    {
      
        index= 4;
    }
    else if(strcmp(edit->tag,"-c")==0)
    {
        index= 5;
    }
    else{
        printf("ERROR: Enter valid tag\n");
        return mp3_failure;
    }
  
    strcpy(edit->tag,edit->ptr_edit[index]);
    char buffer[10];
    fread(buffer,10,1,edit->mp3_fptr_src); //to skip the 10 bytes from the source file by reading
    fwrite(buffer,10,1,edit->mp3_dest);
    for(int i=0;i<6;i++) //loop read the data from the mp3 file by the tags
    {
        if(check_tag_mp3(edit,edit->mp3_fptr_src)==mp3_success) //function call to check the tag
        {
            get_size(edit,edit->mp3_fptr_src); //get size of the particular tag function call
           if(insert_edit_size(edit,edit->mp3_dest)!=mp3_success) //function call to rewrite the size the edited data
            {
                printf("ERROR: copying the data\n"); //if it is false return mp3_failure
                return mp3_failure;
            }
            char buffer[3];
            fread(buffer,3,1,edit->mp3_fptr_src); //to read the two flag byts from the and a single null byte from source file
            fwrite(buffer,3,1,edit->mp3_dest);   //to write the two flag byts from the and a single null byte into dest file
            if(insert_data(edit,edit->mp3_dest)!=mp3_success)
            {
                return mp3_failure;
            }
            char buffer_2[edit->size_data];
            fread(buffer_2,edit->size_data-1,1,edit->mp3_fptr_src); //to the read (skip) the size no of bytes

           break;
        }
        else
        {
           if(get_title_size(edit,edit->mp3_fptr_src)!=mp3_success) // to get title size from the source file
           {
            printf("ERROR: get file size\n"); //if it is false return mp3_failure
            return mp3_failure;
           }
           char buffer_1[3];
           fread(buffer_1,3,1,edit->mp3_fptr_src); //to read the two flag byts from the and a single null byte from source file
           fwrite(buffer_1,3,1,edit->mp3_dest); //to write the two flag byts from the and a single null byte into dest file
           char buffer_2[edit->title_card_size];
           fread(buffer_2,edit->title_card_size-1,1,edit->mp3_fptr_src); //to read the title card size bytes from the source file
           fwrite(buffer_2,edit->title_card_size-1,1,edit->mp3_dest);  //to write the title card size bytes into the destination file
        }
    }
    reamaining_data(edit->mp3_fptr_src,edit->mp3_dest);  //function call to copy the reamaing data into dest file from source file
    fclose(edit->mp3_fptr_src); //to close the file pointer
    fclose(edit->mp3_dest);
    rename(edit->dest_filen,edit->source_name); //rename function 
    return mp3_success;

}

Status check_tag_mp3(Edit* edit,FILE* fptr) //function call to check the tag
{
 char buffer[4];
 fread(buffer,4,1,fptr); //to read the 4 bytes from the source file for tag
 fwrite(buffer,4,1,edit->mp3_dest);//to write the 4 bytes into dest file
 if(strcmp(edit->tag,buffer)==0) //to compare the tag 
 {
    return mp3_success; //it it is true return mp3_success or else mp3_failure
 }
 return mp3_failure;

}

Status insert_edit_size(Edit* edit,FILE* fptr) //to insert the data into big indian format
{
   uint num=edit->data_len+1;
   char buffer[4];
    buffer[0] = num & 0x000000FF;  // MSB
    buffer[1] = (num  & 0x0000FF00)>>8;
    buffer[2] = (num  & 0x00FF0000)>>16;
    buffer[3] = (num & 0xFF000000)>>24;          // LSB
    fwrite(buffer,4,1,fptr); //write the 4 bytes after converting into big indian
    return mp3_success;

}
Status insert_data(Edit* edit,FILE* fptr)
{
  //function definition for the insertion of data
    char buffer[edit->data_len];
    strcpy(buffer,edit->data); //copy the data from data to buffer
    fwrite(buffer,edit->data_len,1,fptr); //write copied data dest file
    return mp3_success;
}
Status get_title_size(Edit* edit,FILE* fptr)
{
    uint size=0;
    char buffer[4];
    fread(buffer,4,1,fptr); // read four bytes from the file pointer by fread function
    fwrite(buffer,4,1,edit->mp3_dest);
   for(int i=0;i<2;i++) //loop converting number from little endian to big endian from buffer
   {
    char temp=buffer[i];
    buffer[i]=buffer[4-i-1];
    buffer[4-i-1]=temp;
   }
   size=buffer[0]|buffer[1]|buffer[2]|buffer[3];
    edit->title_card_size=size; //store the number after converting it
 
    return mp3_success;

}
Status reamaining_data(FILE* fptr1,FILE* fptr2)
{
    //function definition for copying the remaining data
    char buffer[1];
    while(fread(buffer,1,1,fptr1)>0) //while loop to copy thwe data upto EOF
    {
        fwrite(buffer,1,1,fptr2);
    }
}
Status get_size(Edit* edit,FILE* fptr)
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
    edit->size_data=size; //store the number after converting it
    return mp3_success;
}
