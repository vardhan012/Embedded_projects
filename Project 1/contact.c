#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

void listContacts(AddressBook *addressBook) 
{
    int no=1;
    if(addressBook->contactCount!=0)
    {
      printf("|-------|---------------------|--------------|--------------------------|\n");
    printf("|\x1b[32m S.NO\x1b[0m  | \x1b[32m       NAME     \x1b[0m    | \x1b[32m   NUMBER \x1b[0m   |   \x1b[32m      EMAIL ID         \x1b[0m|\n");
    printf("|-------|---------------------|--------------|--------------------------|\n");
    }
    int i;
    for( i=0;i<addressBook->contactCount;i++) //running loop upto contactcount present in adressbook
    {
        
        printf("|  %-3d  | \x1b[33m %-17s \x1b[0m | \x1b[33m %-10s \x1b[0m | \x1b[33m %-22s  \x1b[0m|\n",no,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        printf("|-------|---------------------|--------------|--------------------------|\n");
        no++;
        
    }
    
    if(addressBook->contactCount==0) //when,there is no data present in adressbook
    {
      printf("\n\t:::Data Not Found in the Adressbook:::\n"); //printing no data found
    }
}

void initialize(AddressBook *addressBook)
 {
  //  addressBook->contactCount=0;
   // populateAddressBook(addressBook);

    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
 }
void saveAndExit(AddressBook *addressBook)
 {
    saveContactsToFile(addressBook); // Save contacts to file
   // exit(EXIT_SUCCESS); // Exit the program 
}


void createContact(AddressBook *addressBook)
{
    /* Define the logic to create a Contacts */

    // Read the name from the user
    int chances=0;
    int ind=addressBook->contactCount; //storing the  contactcount into a 'ind' variable
    readname(addressBook,&chances,ind); //reading the name
    if(chances<=limit)  //when,chanches are not exceeding the limit
    readnumber(addressBook,&chances,ind); //reading the mobile number
    if(chances<=limit) //when,chanches are not exceeding the limit
    reademail(addressBook,&chances,ind); //reading the e-mail
    if(chances<=limit)//when,chanches are not exceeding the limit
    {
      addressBook->contactCount++;//increamenting the contactCount
    }
    else
    {
        printf("\n\x1b[31m Your chances got compeleted! Please choose option from AddressBook Menu again.\x1b[0m\n");
        printf("\tThank You\n");
    }
}

int  searchContact(AddressBook *addressBook) 
{
    /* Define the logic for search */
    int chances=0; //Declarations of variables(chances,index...)
    int index=0;
    char name[30];
    char number[12];
    char email[30];
    int opt,duplicatecount=0;
    while(chances<=limit) //when, the chances are less then or equal to given limit(i.e.=4)
    {
    if(duplicatecount==0)//when, there is no duplicate contacts 
     {
       printf("\x1b[33mPlease choose the option that, how do you want to search that contact\x1b[0m:\n1.NAME\n2.MOBILE NUMBER\n3.EMAIL\n4.EXIT\n");
     }
    else
     {
      printf("\x1b[33More number of contacts are there with same name,Please select one option for searching:\x1b[0m\n1.NUMBER\n2.EMAIL ID\n3.EXIT\n");
     }//if,there is multiple no. of contacts with the same name means.
    printf("Enter your option: ");
    scanf("%d",&opt);
    if(duplicatecount!=0)//if the names are same, we need chech with the mobile number or e-mail.
    {
      if(opt==0)
      {
        opt=5;
      }
      else
      {
        opt=opt+1;
      }
    }
    switch(opt)
    {
        case 1:
              printf("Please Enter the Name: ");
              getchar();
              scanf("%[^\n]",name);
              duplicatecount=searchmultipulnames(addressBook,name,&index);//searching the more no of conntacts with smae name
              if(duplicatecount==0) //if, the given details are not matched with contacts in the adressbook
              {
                printf("\x1b[31mContact not found \n"); //printing that contact not found and let them try again.
                printf("Please Try Again\n");
                printf("You have only %d chances\x1b[0m\n",4-chances); //decrementing the chanches for user
                chances++; //inctrementing the chances count
              }
              else if(duplicatecount==1) //when, the contact found 
              {
                 return index; //return the index to the function call
              }
              else
              {
                index=duplicatecount;  //whwn, the more no. of contacts are found means
              }
              break; //break
        case 2:
               printf("Please Enter Mobile number: "); //Asking the user to preint the mobile number
               getchar();
               scanf("%s",number); //read the mobile number
               index=searchnumber(number,addressBook,duplicatecount);//searching the number whether it is present in adressbook or not
               if(index==0&&duplicatecount>1) //if the number is  not found
               {
                printf("\x1b[31mContact not found\n"); //printing that No contact found
                printf("Please Try again\n");
                printf("You have only %d chances\x1b[0m\n",4-chances); //decrementingthe chances for user
                chances++; //incrementing the chanches count
               }
               else
               {
               if(index)
                {
                  if(index==1)
                  {
                   index=searchnumber(number,addressBook,1); //return the index value of the number
                   
                  }
                  printf("\x1b[33mContact found and Details are:\x1b[0m\n");
                  printf("\n|------------------------------------------------------------------------|\n");
                  printf("| \x1b[34m S.NO\x1b[0m | \x1b[34m       NAME     \x1b[0m    | \x1b[34m   NUMBER \x1b[0m   |   \x1b[34m      EMAIL ID         \x1b[0m |\n");
                  printf("|------------------------------------------------------------------------|\n"); 
                  printcontact(index,addressBook,1); //printing that contact details
                  printf("Thank you\n");
                  return index;
                }
                else
                {
                 printf("\x1b[31mContact not found\n"); //when, the contact not found means
                 printf("Please Try again\n");
                 printf("You have only %d chances\x1b[0m\n",4-chances); //decrementing the chances for user
                 chances++; // increamenting the chances count
                 }
                }
                break; //termination
        case 3:
                printf("Please Enter E-mail id: "); //Asking the user to print e-mail
                getchar();
                scanf("%s",email); //read mail
                index=searchemail(email,addressBook,duplicatecount); //searching that mail present in the adressbook or not.
                if(duplicatecount>1&&index==0) //if the mail not found means
                {
                  printf("\x1b[31mContact not found\n"); //printing contact not found
                  printf("Please Try again\n");
                  printf("You have only %d chances\x1b[0m\n",4-chances); //decrementing the chances
                  chances++; //incrementing the chances count.
                }
                else
                {
                if(index)
                {
                  if(duplicatecount>1)
                  {
                    index=searchemail(email,addressBook,1); //return the index 
                  }
                  printf("\x1b[33Contact found and details are:\x1b[0m\n");
                  printf("\n|--------------------|------------------------------------|--------------|\n");
                  printf("| \x1b[34m S.NO\x1b[0m | \x1b[34m       NAME     \x1b[0m    | \x1b[34m   NUMBER \x1b[0m   |   \x1b[34m      EMAIL ID         \x1b[0m |\n");
                  printf("|----------------------|------------------------------------|-------------|\n");
                  printcontact(index,addressBook,1); //printing the contact details 
                  printf("Thank you\n");
                  return index; //returning the index
                }
                else
                {
                  printf("\x1b[31mContact not found\n"); //when, the contact not found means
                  printf("Please Try again\n");
                  printf("You have only %d chances\x1b[0m\n",4-chances);//decrementing the chances for the user
                  chances++; //incrementing the chances
                }
              }
                break;//terminate
        case 4:
               printf("Thank you\n");
               return 0;
        default:
                printf("\x1b[31mInvalid option, Please Choose the option again\n");
                printf("You have only %d chances\x1b[0m\n",4-chances); //showing the chanches to user
                chances++; //incrementing the chances
    }

}
if(chances>limit) //if all the chanaces are got over
{
    printf("\x1b[31mYour chances gotcompleted! Please choose the option from AddressBook Menu again\x1b[0m\n");
    return 0;
}
}

void editContact(AddressBook *addressBook)
{
    printf("\x1b[36mBefore Edit you need to search for the contact,which is available in the AdressBook or not\n");
    printf("Please follow  the below steps\x1b[0m\n");
   int index=searchContact(addressBook);  //Searching for thet contact
   while(index) //Enter the while loop when the contact is avialable in the adressbook
   {
    printf("\x1b[33mPlease select which option that you want to edit:\x1b[0m\n1.NAME\n2.MOBILE NUMBER\n3.EMAIL ID\n4.EXIT\n");
    int opt;
    printf("Please Enter the option: "); //asking for user for what you want to edit
    scanf("%d",&opt);
    int chances=0;
    switch(opt) 
    {
        case 1:
                readname(addressBook,&chances,index-1); //Call readname function againfor editing the name
                break;
        case 2:
                readnumber(addressBook,&chances,index-1);//Call readnumber function againfor editing the mobile number
                break;
        case 3:
                reademail(addressBook,&chances,index-1);//Call reademail function againfor editing the e-mail
                break;
        case 4:
               printf("Thank you\n");
               index=0;
               break;
               
        default:
                printf("\x1b[31mPlease Enter valid option\x1b[0m\n"); //Asking the userto enter the valid option
               
    }
   }

}

void deleteContact(AddressBook *addressBook)
{
    printf("\x1b[36mBefore deleting the contact, you need to search for that contact whearher it is available in AdressBook  or not\n");
    printf("Please follow below steps\x1b[0m\n");
   int index=searchContact(addressBook); //delete the contact, if there is contact is present in the addresssbook
   int size=addressBook->contactCount; //declare the size to storing contact count
   if(index) //delete the contact, when that contact is available
   {
      while(index<=size) //shift the contact to the last index
      {
        details res=addressBook->contacts[index-1];
        addressBook->contacts[index-1]=addressBook->contacts[index];
        addressBook->contacts[index]=res; //insted of using structure array,used to swap the persons usinf res variable
        index++;
      }
      addressBook->contactCount--;
      printf("\x1b[32mContact deleted successfully\x1b[0m\n");
   }
   else
   {
    printf("\x1b[31mContact not found, Please Try again\x1b[0m\n");//if, the contact not found means,print not found 
   }

}
int searchname(char* name,AddressBook* addressBook)
{
    for(int i=0;i<(addressBook->contactCount);i++)
    {
       if(strcmp(name,addressBook->contacts[i].name)==0) //ENter the loop, when the input name is present
       {
        while(1) //while loop will run until the right input given by the user
        {
         printf("\x1b[33mThere are More number of contacts are present with the same Name\x1b[0m\nPress \x1b[32m'Y'\x1b[0m for save, Press \x1b[32m'N'\x1b[0mfor Entering the other Name:\x1b[0m ");
         char ch;
         getchar();
         scanf("%c",&ch);
         if(ch=='N') //Read name when user enters 'N'
           return 1;
        if(ch=='Y')// store that name  //the user should give Y or N .
        {
          i=addressBook->contactCount+1; //stop the loop
          break;
        }
        else
        {
             printf("\n\x1b[31mPlease Enter valid input\x1b[0m\n");
        }
       }

      }
       
    }
    return 0;

}
int searchnumber(char* number ,AddressBook* addressBook,int dup)
{
  if(dup<=1) // if one contact is present with the same name itself
  {
   for(int i=0;i<(addressBook->contactCount);i++) //the loop runs until it go to the structure array size
    {
       if(strcmp(number,addressBook->contacts[i].phone)==0)//if number found means
       {
          return i+1; //return the non zero value
       }    
       
    }
  }
   else //if, The multiple no. of contacts are present with same name
   {
   for(int i=0;i<(addressBook->dupcount);i++) // loop running until to go to array size
    {
       if(strcmp(number,addressBook->duplicate[i].phone)==0)
       {
          return 1;
       }    
    }
    return 0; //return 0, if there is no number found.
  }

    return 0; //return 0, if there is no number found.
}
int searchemail(char* mail,AddressBook* addressBook,int dup)
{
  if(dup<=1) //if, only one contact is found with same name
  {
    for(int i=0;i<(addressBook->contactCount);i++) //run a loop until it goes structure array size
    {
       if(strcmp(mail,addressBook->contacts[i].email)==0)
       {
          return i+1; //return the non zero value, if e-mail found
       }   
    }
  }
  else 
  {
    for(int i=0;i<(addressBook->dupcount);i++)
    {
       if(strcmp(mail,addressBook->duplicate[i].email)==0) //comparing the e-mail
       {
          return 1; //if email found return 1
       }
       
    }
    return 0; //if email not found return 0
  }
    return 0;//if email not found return 0
}
void readname(AddressBook* addressBook,int* chances,int ind) //colecting adress of adressbook & chances
{
    char name[20]; //taking a temperory string storing the name
    int chancescount=1,count; //variable declaration
    while(chancescount<=limit)
    {
      count=0;
      printf("Please Enter name: ");
      getchar();
      scanf("%[^\n]",name);
      while(name[count]) //first letter of the name is !='\0'
      {
       if((name[count]>='a'&&name[count]<='z')||(name[count]>='A'&&name[count]<='Z')||name[count]==' '||name[count]=='.')
        {
           count++; //count should be increamented
        }
        else 
        {
            printf("\x1b[31m\nplease Enter valid Name(Ex:B.Vardhan (or) B Vardhan)\n");
            printf("You have only %d chances\x1b[0m\n",4-chancescount); //decreasing the chanchescount
            chancescount++; //increment of chances upto the condition gets false
            break;
        }
       }
     if(name[count]=='\0') 
     {
        if(searchname(name,addressBook)) //call the function for cheacking wheather the name is already exist or not
        { 
            printf("\x1b[31mThe Name you entered was already exist, Please Enter  the Name again\n");
            printf("You have only %d chances\x1b[0m\n",4-chancescount);
            chancescount++; //increament chancescount
        }
        else
        {
            strcpy(addressBook->contacts[ind].name,name);   //copy the name into the 1st index
            printf("\x1b[33mName added successfully\x1b[0m\n");
            break;
        }

     }
    }
     *chances=chancescount; //store the chancescount into chances
    
}
void readnumber(AddressBook* addressBook,int* chances,int index)
{
    if(*chances<=limit)
    {
      int chancescount=1;
     while(chancescount<=limit)//user having only 4 chanches
     {
      int count=0;
      getchar();
      printf("Please Enter Mobile Number: ");
      char number[20];
      scanf("%s",number);
        while(number[count]) //while loop runs until NULL
        {
            if(number[count]>='0'&&number[count]<='9'&&number[0]>5)//mobile num should contains integers only
            {
               count++;//if all conditions are true incrementing the count value by 1 for checking the next number
            } 
            else
            {
                break;//terminate from loop,if the condition not true
            }
        }
        if(count!=10) //the mobile number should contains 10 digts
        {
            printf("\n\x1b[31mPlease Enter valid Number\n");
            printf("NOTE: Number must be have 10 digits and first number should be greater than 5(EX:9866803972)\n");
            printf("You have only %d chances\x1b[0m\n",4-chancescount); //decrementing the chanches for user
            chancescount++; //incrementing the chanches count
        }
        else
        {
         if(searchnumber(number,addressBook,1))
         {
            printf("\x1b[31mMobile Number was already exist Please Enter the Number again\n");
            printf("You have only %d chances\x1b[0m\n",4-chancescount);
            chancescount++;
         }
         else
         {
            strcpy(addressBook->contacts[index].phone,number); //copy the number into the structure
            printf("\x1b[33mNumber added successfully\x1b[0m\n");
            break;
         }
        }
     }
     *chances=chancescount; //chanches were update here
   }
}
void reademail(AddressBook* addressBook,int* chances,int index)
{
     int chancescount=1;
    while(chancescount<=limit) //user have limited chances only
    {
      char mail[50];
      getchar();
      printf("Please Enter E-mail: ");
      scanf("%s",mail);
      int count=0;
      int a=0,b=0,m=0;
      while(mail[count]) //run while until NULL
      {
        if((mail[0]>='a'&&mail[0]<='z')) //first character should be lower case
        {
            if(((mail[count]>='a'&&mail[count]<='z')||(mail[count]=='@')||(mail[count]>='0'&&mail[count]<='9')||(mail[count]=='.'))&&a<=1&&b<=1)//(lowercase,ints,@,.)should give by the user
             {
                count++; //if above condition get true,increment count value by 1,for next charcter checking
                if(mail[count]=='.') //e-mail should contains '.'
                {
                  a++;
                }
                else if(mail[count]=='@')//e-mail should contains '@'
                {
                  b++;
                  m= count; //storing the '@' into index

                }
             }
            else
               {
                printf("\x1b[31mPlease Enter valid E-mail(Ex:xxxx5678@uuu.com)\n");
                printf("You have only %d chances\x1b[0m\n",4-chancescount);
                chancescount++;
                break;
               }
        }
        else
        {
            printf("\x1b[31mFirst charecter should be in the  lowercase Alphabet(Ex:xxxx5678@uuu.com)\n");
            printf("You have only %d chances\x1b[0m\n",4-chancescount);
            chancescount++;
            break;
        }
     }
     if(count>5&&a==1&&b==1)
     {
      count=count-4;
      if(strcmp(&mail[count],".com"))
      {
        printf("\x1b[31mPlease enter valid email(Ex:xxxx5678@kkk.com)\n");
        printf("You have only %d chances\x1b[0m\n",4-chancescount);//decrementing the chances for user for the invalid input
        chancescount++; //incrementing the chanches count
      }
     else
      {
        
        if(searchemail(mail,addressBook,1))
        {
            printf("\x1b[31mEmail already exist please try again\n");
            printf("You have only %d chances\x1b[0m\n",4-chancescount); //decrementing the chanches
            chancescount++; //increment chances count
        }
        else
        {
            strcpy((addressBook->contacts[index].email),mail); //copy mail into the structure
            printf("\x1b[33mEmail added successfully\x1b[0m\n");
            printf("\x1b[32mNew Contact created sucessfully, Thank you\x1b[0m\n");
            break;
        } 
      }
     }
     else{
        printf("\x1b[31mYour Email should be  containing only one '@',one '.'and atleast 7 characters after '@'(Ex:xxxx5678@jjj.com\x1b[0m\n");
        printf("You have only %d chances\x1b[0m\n",4-chancescount);
        chancescount++;
        }
    }
    *chances=chancescount; //update the chances
  

}
int searchmultipulnames(AddressBook* addressBook,char* name,int* index)
{
  int i,duplicatecount=0,n=0;
  for(i=0;i<addressBook->contactCount;i++) //upto the array size the loop runs
  {
     if(strcmp(name,addressBook->contacts[i].name)==0)//shearching if the contact is present with same name
     {
       if(n==0)
       {
        printf("Contact datails:\n"); 
       printf("\n|------------------------------------------------------------------------|\n");
       printf("|\x1b[34mS.NO\x1b[0m   | \x1b[34m       NAME     \x1b[0m    | \x1b[34m   NUMBER \x1b[0m   |   \x1b[34m      EMAIL ID         \x1b[0m |\n");
       printf("|------------------------------------------------------------------------|\n");  
       }    
       printcontact(i+1,addressBook,duplicatecount+1);//print contact
       *index=i+1;
       duplicatecount++;//increment duplicate count
       strcpy(addressBook->duplicate[n].name,addressBook->contacts[i].name);
       strcpy(addressBook->duplicate[n].phone,addressBook->contacts[i].phone);
       strcpy(addressBook->duplicate[n].email,addressBook->contacts[i].email);
       n++;
     }
   
  }
    addressBook->dupcount=n;//store the duplicate num count to dupcount
    return duplicatecount;
}
void printcontact(int index,AddressBook* addressBook,int n) //printing the contacts.
{
  printf("|  %-3d  | \x1b[33m %-17s \x1b[0m | \x1b[35m %-10s \x1b[0m | \x1b[32m %-22s   \x1b[0m|\n",n,addressBook->contacts[index-1].name,addressBook->contacts[index-1].phone,addressBook->contacts[index-1].email);
  printf("|------------------------------------------------------------------------|\n");

}