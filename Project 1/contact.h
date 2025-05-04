#ifndef CONTACT_H
#define CONTACT_H
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_CONTACTS 100
#define limit 4

typedef struct Contact{
    char name[50];
    char phone[11];
    char email[50];
} details;

typedef struct {
    details contacts[MAX_CONTACTS];
    int contactCount;
    details duplicate[MAX_CONTACTS];
    int dupcount;
} AddressBook;

int savecontacts(AddressBook *addressBook);
void createContact(AddressBook *addressBook);
int searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);
void saveAndExit(AddressBook* addressBook);
int searchname(char* name,AddressBook* sddressBook);
int searchemail(char* mail,AddressBook* addressBook,int dup);
int searchnumber(char* number ,AddressBook* addressBook,int dup);
void printcontact(int index,AddressBook* addressBook,int n);
void reademail(AddressBook* addressBook,int* chances,int index);
void readnumber(AddressBook* addressBook,int* chances,int index);
void readname(AddressBook* addressBook,int* chances,int index);
int searchmultipulnames(AddressBook* addressBook,char* name,int* index);
#endif