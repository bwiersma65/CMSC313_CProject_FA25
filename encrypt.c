/*----------------------------------------------------------------------------*/
/* Define libraries to be included */
#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include <string.h>
#include <ctype.h>
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Define Structures*/
typedef struct  eNode{
    char name[100];           /*contains name*/
    struct eNode* next; /*next is used to navigate through structures.*/
} eNode;
eNode* firste, * currente, * newe; /*pointers used to create original linked list*/
eNode *encryptfirst, *encryptcurrent, *encryptnew; /*pointers you can use to create your encrypted list*/
/* firste is used to point to first record in the original linked list
encryptfirst must point to the first record in your encrypted list
*/

/* Define Functions (prototypes)*/
void listAll(eNode*);
void shiftEncrypt(eNode* first, int shiftAmt);
char shift(char name, int shiftAmt);
void deleteList(eNode* first);

/*----------------------------------------------------------------------------*/
/* Main Function */

//int main(int argc, char **argv)
//argc is the number of arguments used to run your program
//argv is an array of char* arguments
//argv[0] is the name of the executable
//argv[1] is the first argument that you pass in (if you passed in any).
int main(int argc, char **argv) 
{
    char name[100];
    char shift[10];
    firste = NULL;
    int counter = 9;
    int shiftAmt;
    encryptfirst = NULL;
    
    FILE *datafile = fopen(argv[1], "r");/* open file for reading*/
    if(datafile == NULL) {printf("can't open\n");}
    else
    {
	//get shiftAmt
	fgets(shift,9,datafile);
	//convert to int
	shiftAmt = atoi(shift);
        //get first name
        fgets(name, 99, datafile);
        //strcspn returns the index where the second string is in the first string
        //replaces it with a 0 for a cstring (terminating 0)
        name[strcspn(name, "\n")] = 0;
	    int length = strlen(name);
        newe = malloc(sizeof(eNode)); //allocate a new node
        //just in case there wasn't a newline, put in a terminating 0
	    name[length] = '\0';
        strcpy(newe->name,name); //copy name into node
        newe->next = NULL;
        firste = newe;
        while (fgets(name,99,datafile)) //pick up the next 9 names
        {
            name[strcspn(name, "\n")] = 0;
	        length = strlen(name);
	        name[length]= '\0';
            newe = malloc(sizeof(eNode)); //allocate a new node
            strcpy(newe->name, name); //copy name into node
            if (firste != NULL) {
                newe->next = firste; // add new Node to beginning
            }
            firste = newe; //beginning of list updated
            counter--;
        }
        fclose(datafile);  /* close file - good practice to close files after use*/
    }
    
    printf("Names\n");
    listAll(firste);
    printf("\nEncrypting ...\n");
    shiftEncrypt(firste,shiftAmt);
    printf("\nEncrypted Names\n");
    listAll(encryptfirst);
    printf("\nOriginal Names\n");
    listAll(firste);

    //deallocate the original linked list
    deleteList(firste);
    //deallocate the encrypted linked list
    deleteList(encryptfirst);
}

//This function is to deallocate a linked list
void deleteList(eNode* first)
{
    eNode* next;
    currente = first;
    while(currente != NULL)
    {
	next = currente->next;
	free(currente);
	currente = next;
    }
    first = NULL;
}

/*This function takes as a parameter the address (pointer) to the first element in a linked list and 
  displays the name for all the nodes in the linked list.
  **Because the nodes for original names are added to the linked list at the beginning of the list,
  those names will be printed in reverse order. The encrypted names may be in either order depending
  on how you added nodes to your encrypted list*/

void listAll(eNode* firstelement)
{
    while (firstelement != NULL) {
        printf("%s\n", firstelement->name);
        firstelement = firstelement->next;
    }
}


/*shiftEncrypt(eNode *original, int shift). This function takes as a parameter the address (pointer) 
 *  to the first element in a linked list.
 * 	The name in the node will be encrypted and placed into a new linked list.
 * 	For each node in the original linked list
 * 		i. 	Create a new node in your encrypted linked list
 * 		ii. 	The name of the encrypted node will be a version of the orignal name by shifting every
         alphabetic letter in the name by the indicated number of positions in the alphabet. Call shift() for 
         each character in the original name
 */
void shiftEncrypt(eNode* firstelement, int shiftAmt)
{
    // set current unencrypted node to parameter
    eNode* currentNode = firstelement;

    // create first new node to store encrypted name
    encryptcurrent = malloc(sizeof(eNode));
    // initialize name field with blank string
    strcpy(encryptcurrent->name, "");
    // initialize next field with NULL
    encryptcurrent->next = NULL;


    // loop until every unencrypted node has been processed
    while (currentNode != NULL) {
        char encryptedResult[100];
        int nameIndex = 0;
        // Traverse c-string name of current unencrypted node and shift each character by shiftAmt
        // Store result in c-string
        for (int i=0; i < strlen(currentNode->name); i++) {
            char encryptedChar = shift(currentNode->name[i], shiftAmt);
            encryptedResult[nameIndex] = encryptedChar;
            nameIndex++;
            encryptedResult[nameIndex] = '\0';
        }
        // Copy result into encrypted node's name field
        strcpy(encryptcurrent->name, encryptedResult);
        // Traverse to next unencrypted node in linked list
        currentNode = currentNode->next;

        if (currentNode == NULL) break;

        // create next node to store encrypted name
        encryptnew = malloc(sizeof(eNode));
        // initialize name field with blank string
        strcpy(encryptnew->name, "");
        // initialize next field with NULL
        encryptnew->next = NULL;

        // Append new node to beginning of linked list of encrypted nodes
        encryptnew->next = encryptcurrent;
        // Set current to new node
        encryptcurrent = encryptnew;
    }
    // After breaking loop, encryptcurrent will be the firstmost node in encrypted linked list
    encryptfirst = encryptcurrent;
    // Clear variables
    currentNode = NULL;
    encryptcurrent = NULL;
    encryptnew = NULL;
}


/*  char shift(char, int). This function is a utility function called by 
    shiftEncrypt, that takes in a character and the shift amount and returns the 
    encrypted character based on the following:
    1. The character is shifted by the shiftAmt. If a character gets shifted past the beginning 
        or end of the alphabet, wrap around to the other end to continue.
    2. Lower case letters will be shifted to a lowercase letter and uppercase letters will be shifted
        to an uppercase letter. For example, a shift value of 6 would shift a 'e' character to a 'k', 
        a 'w' will get shifted to a 'c', a 'E' character to a 'K', and a 'W' will get shifted to a 'C'
	3. All non-alphabetic characters will not be shifted. They will remain the same.
*/
char shift(char name, int shiftAmt) {
    char encryptedName;
    // character is uppercase alphabetic
    if ((65 <= name) && (name <= 90)) {
        if ((name+shiftAmt) > 90) {
            encryptedName = ((name+shiftAmt)-90)+64;
        }
        else {
            encryptedName = name + shiftAmt;
        }
    }
    else if ((97 <= name) && (name <= 122)) {
        if ((name+shiftAmt) > 122) {
            encryptedName = ((name+shiftAmt)-122)+96;
        }
        else {
            encryptedName = name + shiftAmt;
        }
    }
    else {
        encryptedName = name;
    }
    return encryptedName;
}
    
/*----------------------------------------------------------------------------*/

/* END OF PROGRAM */
   