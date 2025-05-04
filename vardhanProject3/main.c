/*
Name        : Vardhan
Date        : 29/04/2025
Project     : Mp3 Tag reader
Batch No      : 24038_034
*/

#include"reader.h"
#include <string.h>
#include "types.h"
#include "edit.h"

int main(int argc,char* argv[])
{
    if(argc>=3) //condition to check the arguements in my command line
    {
        Treader reader;
        if(check_operation(argv[1])==mp3_view) // function call to check the operation type 
        {
            if((read_and_validate(&reader,argv)) == mp3_success) //raed and validate function call if it is success only go to next step
            {
                if(read_data_mp3(&reader)==mp3_success) //read the data from mp3 file function call
                {
                  printf("SUCCESSFULLY COMPLETED\n");
                }
            }
        }
        else if(check_operation(argv[1])==mp3_edit) 
        {
            Edit edit; //creating structure variable
           if(read_and_validate_args(&edit,argv)==mp3_success) //function call fr read and validate
            {
                if(read_data_mp3_enc(&edit)==mp3_success) //function call for the read_data from mp3 file
                {
                    printf("EDITING COMPLETED\n");
                }
            }
        }
        else
        {
            printf("INFO: ./a.out -v to view the file\n");
            printf("INFO: ./a.out -E to Eidt the file\n");
    
        }
}
else if(argc==2)
{
    if(check_operation(argv[1])==mp3_help) //function call to check the operation type
    {
        printf("INFO: ./a.out -v to view the file\n");
        printf("INFO: ./a.out -E to Eidt the file\n");

    }
    else{
        printf("ERROR: pass valid arugeuements\n");
    } 
}
else{
    printf("ERROR: pass valid arguements\n");
    printf("INFO: for more info -> ./a.out --help\n");
}
}
OperationType check_operation(char* symbol) //check_operation definition
{
    if(strcmp(symbol,"-v")==0) // comparing the arguement of the comand line by using strcmp
    return mp3_view;
    else if(strcmp(symbol,"-e")==0) 
    return mp3_edit;
    else if(strstr(symbol,"--help")!=NULL) //for user user --help
    {
        return mp3_help;
    }
   else
   return mp3_unsupported; //returning unsupported
}
