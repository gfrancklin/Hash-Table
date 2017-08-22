/*******************************************************************************
* File Name          : hashAssignment.cpp
* Description        : Implemenation of a hash skelleton
*
* Author:              PROG8130 / Gabriel F. Yano
* Date:                August 08, 2017
******************************************************************************
*/

#include "stdafx.h"
#include <windows.h>
#include <strsafe.h>

unsigned int putIntoHashTable(char *ptrInputData, unsigned int bufferLength);    // function to add to hash table
unsigned int getFromHashTable(char *ptrOutputData, unsigned int bufferLength);   // function to retrieve data from hash table

#define INPUT_BUFFER_SIZE		200					// local buffer used for adding to hash table
#define HASH_SIZE				4					// size of hash table to be used

struct myHashStruct {
	char * ptrBuffer = NULL;                       // pointer to data stored in hash
	struct myHashStruct *ptrNextHashData = NULL;  // pointer to next item in this hash bucket (or NULL if no more)
};

struct myHashStruct * myHash[HASH_SIZE];             // create an empty hash table structure

int main() {
	char inputBuffer[INPUT_BUFFER_SIZE];
	// initialize the hash table to empty one
	for (int i = 0; i < HASH_SIZE; i++)
	{
		if ((myHash[i] = (struct myHashStruct *)calloc(1, sizeof(struct myHashStruct))) == NULL) {
			printf("calloc failed!\n");
			exit(-1);
		}
	}
	
	while (1) {
		printf("enter data to be added to hash table or exit when done\n");

		// get strings from the console and place in hash until nothing entered
		scanf_s("%s", inputBuffer, INPUT_BUFFER_SIZE);

		// stop adding data into hash table when "exit" is entered
		if (strcmp(inputBuffer, "exit") == 0)
			break;

		if (putIntoHashTable(inputBuffer, strlen(inputBuffer)) == HASH_SIZE) {
			printf("Error putting into hash table\n");
		}
	}
	// check if data is in hash table
	while (1) {
		unsigned int hashIndexNumber = 0;

		printf("Enter data to find, done when complete\n");

		// get strings from the console and check if in hash table
		scanf_s("%s", inputBuffer, INPUT_BUFFER_SIZE);

		// stop adding data into hash table when "done" is entered
		if (strcmp(inputBuffer, "done") == 0)
			break;

		if ((hashIndexNumber = getFromHashTable(inputBuffer, strlen(inputBuffer))) == HASH_SIZE) {
			printf("%s not found in hash table\n", inputBuffer);
		}
	}
	return 0;
}

// FUNCTION      : myHashFunction
// DESCRIPTION   :
//   Calculate a hash value to use in storing the data into the hash table using
//	 the algorithm in the slides
// PARAMETERS    :
//   ptrInputBuffer - a pointer to the buffer to be put into the hash table
//
// RETURNS       :
//   Hash value calculated
unsigned int myHashFunction(char *ptrInputBuffer) {
	unsigned int hash = 0;
	int c;

	// I'm going to use the sdbm algorithm showed on the slides
	while (c = *ptrInputBuffer++) {		//while there is a character different then NULL
		/* Then the magic happens */
		hash = c + (hash << 6) + (hash << 16) - hash;
	}
	return hash % HASH_SIZE;		//prevent the value to be bigger than the table
}

// FUNCTION      : putIntoHashTable
// DESCRIPTION   :
//   Put the supplied data into a hash table
// PARAMETERS    :
//   ptrInputBuffer - a pointer to the buffer to be put into the hash table
//   bufferLength   - how many characters are in the buffer being put into the hash table
// RETURNS       :
//   Hash value used ( HASH_SIZE on failure)
unsigned int putIntoHashTable(char * ptrInputData, unsigned int bufferLength){
	unsigned int hashValue = myHashFunction(ptrInputData);
	
	/* Check if the spot is blank */
	if (myHash[hashValue]->ptrBuffer == NULL) {
		/* If it's blank then allocate a memory for the buffer to be stored */
		if ((myHash[hashValue]->ptrBuffer = (char *)malloc(bufferLength)) == NULL) {
			return HASH_SIZE;
		}
		/* Store input buffer to the hash buffer allocated */
		sprintf(myHash[hashValue]->ptrBuffer, "%s", ptrInputData);
		myHash[hashValue]->ptrNextHashData = NULL;
	/* If it's not blank then we have to initialize a linked list */
	} else {
		struct myHashStruct * tempPtr = myHash[hashValue];	//tempPtr will be the head of the linked list
		while (tempPtr->ptrNextHashData != NULL) {			
			tempPtr = tempPtr->ptrNextHashData;				//go to the end of the list
		}

		/* Allocate memory to store the new value in the linked list */
		struct myHashStruct * newElement = (struct myHashStruct *)malloc(sizeof(struct myHashStruct));
		if (newElement != NULL) {
			/* Link the end of the list with the new Element */
			tempPtr->ptrNextHashData = newElement;

			/* Now allocate memory for the buffer */
			if ((tempPtr->ptrNextHashData->ptrBuffer = (char *)malloc(bufferLength)) == NULL) {
				return HASH_SIZE;
			}
			/* Store input buffer to the hash buffer allocated */
			sprintf(tempPtr->ptrNextHashData->ptrBuffer, "%s", ptrInputData);
			tempPtr->ptrNextHashData->ptrNextHashData = NULL;
		}
		else 
			return HASH_SIZE;	//in case of not enough memory to allocate
	}
	return hashValue;
}

// FUNCTION      : getFromHashTable
// DESCRIPTION   :
//   Read as much data as there is room for from the hash table
// PARAMETERS    :
//   ptrOutputBuffer - a pointer to the buffer to place the data read from hash table
//   bufferLength    - maxiumum number of characters that can be read
//
// RETURNS       :
//   Hash value used ( HASH_SIZE on failure)
unsigned int getFromHashTable(char *ptrOutputData, unsigned int bufferLength){
	unsigned int hashValue = myHashFunction(ptrOutputData);

	/* Check if the element of the table is empty */
	if (myHash[hashValue]->ptrBuffer == NULL) {
		return HASH_SIZE;
	/* if not then Compare strings */
	} else if (strcmp(ptrOutputData, (myHash[hashValue]->ptrBuffer)) == 0) {
		printf("%s found in hash table at %u\n", (myHash[hashValue]->ptrBuffer), hashValue);
	}
	/* Then check if there are more words in the same hash value */
	if (myHash[hashValue]->ptrNextHashData != NULL) {
		/* if so then create and auxiliar pointer */
		struct myHashStruct * tempPtr = myHash[hashValue]->ptrNextHashData;
		do {
			/* iterate the list comparing each string in the hash to check if matches and 
			   printing if it matches */
			if (strcmp(ptrOutputData, (tempPtr->ptrBuffer)) == 0) {
				printf("%s found in hash table at %u\n", (tempPtr->ptrBuffer), hashValue);
			}
			tempPtr = tempPtr->ptrNextHashData;
		} while (tempPtr->ptrNextHashData != NULL);	//while the list is over
	} 
	return hashValue;
}   