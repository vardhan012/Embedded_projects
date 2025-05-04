/*
			***DOCUMENTATION***
	NAME				:	B.VARDHAN
	DATE OF SUBMISION 	:	16/03/25
	BATCH NO 			:	24038_34
	PROJECT				:	ADRESS_BOOK
*/

#include "contact.h"
#include "file.h"

int main()
{
  AddressBook addressbook;
  initialize(&addressbook);  
	printf("\t***>>>WELCOME TO ADDRESSBOOK<<<***");
  while(1)
   {
	printf("\n\x1b[33mPlease Select any  one option from Address Book Menu:\x1b[0m\n");
	printf(" 1. Create Contact\n");
	printf(" 2. Search Contact\n");
	printf(" 3. Edit Contact\n");
	printf(" 4. Delete Contact\n");
	printf(" 5. List All Contacts\n");
	printf(" 6. Save and Exit\n");
	printf(" Please Enter your Choice: ");
	int opt;
	scanf("%d",&opt);
	switch(opt)
    {
		case 1:
		       createContact(&addressbook);
			   break;
		case 2:
	           searchContact(&addressbook);
			   break;
		case 3:
		       editContact(&addressbook);
			   break;
		 case 4:
		 		deleteContact(&addressbook);
				break;
		case 5:
		       listContacts(&addressbook);
			   break;
		case 6:
		       saveAndExit(&addressbook);
		       printf("\t***>>>THANK YOU<<<***\n");
			   return 0;
		default:

		        printf("Please Select the  valid option from the AdressBook Menu\n");
	}
	
		
		       
  }
}