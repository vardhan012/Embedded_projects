/*
		<<<-Documentation->>>
	
	Name         :  B.Vardhan
	Date      	 :  11/04/25 
	Batch No.    :  24038_034
	Module       :  Advanced-C
	Description  :  LSB Byte Stegnography

*/

#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc,char* argv[])
{
	//Step-1:
    //Check wheather the user is passing minimum 4 arguments or not
    //TRUE => Go to the next step  ,  FALSE => Print ERROR message  
	if(argc >= 3)
	{
		//Step-2:
        //Call the check_operation_type function pass argv[1], check wheather it is e_encode or decode and return 
        // if, e_encode (or) e_decode => Go to next step ,else print ERROR message (i.e. Insufficient arguments)
		int a = check_operation_type(argv);
		EncodeInfo encInfo;
		//Step-3: Declare a variable with the EncodeInfo
		if(a == e_encode)
		{
			printf("Encoded was selected\n");
            //Step-4: Call the read_and_validate_encode_args function and validate the arguments passed
			int result = read_and_validate_encode_args(argv,&encInfo);  
			//Step-5: Return success or failure 
            //TRUE => Go to next step  ,  FALSE => Terminate the total program
			if(result == e_success)
			{
                //Step-6: Call do_encoding function(&encInfo) 
				do_encoding(&encInfo);  
			}
			else{
				exit(0); //Total program got terminated
			}
		}
		decodeInfo decInfo;
		if(a == e_decode)
		{
			printf("Selected decode\n");
			int result1 = read_and_validate_decode_args(argv,&decInfo);// Call read and validate function 
			if(result1 == e_success)
			{
				do_decoding(&decInfo); // Call do_decoding function(&decInfo)
			}
			else{
				exit(0); //Terminate the total progarm

			}
		}
	}
	else{
		printf("Insufficient arguments\n"); //print Insufficient arguments
	}
    return 0;
}

OperationType check_operation_type(char *argv[])
{
	//Step-1: Check the arg is -E (or) not , if yes return e_encode
	if(strcmp(argv[1],"-E") == 0)
	{
		return e_encode; //return e_encode
	}
	//Step-2: Check the arg is -D (or) not , if yes return the e_decode
	if(strcmp(argv[1],"-D") == 0)
	{
		return e_decode; //return e_decode
	}
    else 
	{
		printf("Enter only -E  encoding (or) -D for  decoding\n"); //print message to give only -E and -D from the user
		return e_unsupported; // return e_unsupported
	}
}
