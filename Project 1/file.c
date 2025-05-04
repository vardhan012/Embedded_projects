
#include "file.h"
void saveContactsToFile(AddressBook *addressBook) 
{
    FILE* fp=fopen("contacts.csv","w");//open file in the right mode
    if(fp==NULL)//check the file is available are not
    {
     printf("Unable to creat file\n");
     exit(1);
    }
    fprintf(fp,"%s %d\n","#Total contacts: ",addressBook->contactCount);//print total contactcount into the file
    for(int i=0;i<addressBook->contactCount;i++)//run the loop until total contactcount
    {
     fprintf(fp,"%s,",addressBook->contacts[i].name);//copy the structure data to file
     fprintf(fp,"%s,",addressBook->contacts[i].phone);//copy the structure data to file
     fprintf(fp,"%s\n",addressBook->contacts[i].email);//copy the structure data to file
    }
     fclose(fp);//file closed.
}

void loadContactsFromFile(AddressBook *addressBook) 
{
    FILE* fp=fopen("contacts.csv","r");//open the file in read mode
    if(fp==NULL)//check file is present are not
    {
        printf(" file not fount\n");
        exit(1);
    }
    int j=0,i,count=0;//variable declaration
    char str[150];//buffer
    fgets(str,sizeof(str),fp);//for skiping the first line in file
    while(fgets(str,sizeof(str),fp))//run the loop upto 'EOF',copy data to the buffer
    {
       str[strlen(str)-1]='\0';
       char* data=strtok(str,",");//store the data to char address based on delimits
       strcpy(addressBook->contacts[count].name,data);//storing data in structure arry variables
       data=strtok(NULL,",");
       strcpy(addressBook->contacts[count].phone,data);//storing data in structure arry variables
       data=strtok(NULL,",");
       strcpy(addressBook->contacts[count].email,data);//storing data in structure arry variables
       count++;//incleasing index value
    }
    fclose(fp);//file closing
    addressBook->contactCount=count;//assign index count to contactCount
}
