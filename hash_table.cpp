// ============================================
// The HashTable class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


/*
Student Name: Vincentius Kosasih
Date: April 3rd, 2023
=======================
ECE 2035 Project 2-1:
=======================
*/


/****************************************************************************
 * Include the Public Interface
 *
 * By including the public interface at the top of the file, the compiler can
 * enforce that the function declarations in the the header are not in
 * conflict with the definitions in the file. This is not a guarantee of
 * correctness, but it is better than nothing!
 ***************************************************************************/
#include "hash_table.h"

/****************************************************************************
 * Include other private dependencies
 *
 * These other modules are used in the implementation of the hash table module,
 * but are not required by users of the hash table.
 ***************************************************************************/
#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For printf

/****************************************************************************
 * Hidden Definitions
 *
 * These definitions are not available outside of this file. However, because
 * the are forward declared in hash_table.h, the type names are
 * available everywhere and user code can hold pointers to these structs.
 ***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable
{
    /** The array of pointers to the head of a singly linked list, whose nodes
        are HashTableEntry objects */
    HashTableEntry **buckets;

    /** The hash function pointer */
    HashFunction hash;

    /** The number of buckets in the hash table */
    unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry
{
    /** The key for the hash table entry */
    unsigned int key;

    /** The value associated with this hash table entry */
    void *value;

    /**
     * A pointer pointing to the next hash table entry
     * NULL means there is no next entry (i.e. this is the tail)
     */
    HashTableEntry *next;
};

/****************************************************************************
 * Private Functions
 *
 * These functions are not available outside of this file, since they are not
 * declared in hash_table.h.
 ***************************************************************************/
/**
 * createHashTableEntry
 *
 * Helper function that creates a hash table entry by allocating memory for it on
 * the heap. It initializes the entry with key and value, initialize pointer to
 * the next entry as NULL, and return the pointer to this hash table entry.
 *
 * @param key The key corresponds to the hash table entry
 * @param value The value stored in the hash table entry
 * @return The pointer to the hash table entry
 */
static HashTableEntry *createHashTableEntry(unsigned int key, void *value)
{
    HashTableEntry *newEntry = (HashTableEntry *)malloc(sizeof(HashTableEntry)); // allocate memory
    if (newEntry == NULL) {
        return NULL; // memory allocation error
    }
    newEntry->key = key;
    newEntry->value = value;
    newEntry->next = NULL;
    return newEntry;
}

/**
 * findItem
 *
 * Helper function that checks whether there exists the hash table entry that
 * contains a specific key.
 *
 * @param hashTable The pointer to the hash table
 * @param key The key corresponds to the hash table entry
 * @return The pointer to the hash table entry, or NULL if key does not exist
 */
static HashTableEntry *findItem(HashTable *hashTable, unsigned int key)
{
    if (hashTable == NULL) {
        return NULL; // if given table is null
    }
    unsigned int ind = (hashTable->hash)(key); // find index / bucket
    HashTableEntry *tempEntry = hashTable->buckets[ind]; // gets entry

    while (tempEntry != NULL) {
        if (tempEntry->key == key) {
            return tempEntry; // if key matches, return entry
        }
        tempEntry = tempEntry->next; // iterate through linked list
    }

    return NULL; // no entry found
}

/****************************************************************************
 * Public Interface Functions
 *
 * These functions implement the public interface as specified in the header
 * file, and make use of the private functions and hidden definitions in the
 * above sections.
 ****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable *createHashTable(HashFunction hashFunction, unsigned int numBuckets)
{
    // The hash table has to contain at least one bucket. Exit gracefully if
    // this condition is not met.
    if (numBuckets == 0)
    {
        printf("Hash table has to contain at least 1 bucket...\n");
        exit(1);
    }

    // Allocate memory for the new HashTable struct on heap.
    HashTable *newTable = (HashTable *)malloc(sizeof(HashTable));

    // Initialize the components of the new HashTable struct.
    newTable->hash = hashFunction;
    newTable->num_buckets = numBuckets;
    newTable->buckets = (HashTableEntry **)malloc(numBuckets * sizeof(HashTableEntry *));

    // As the new buckets are empty, init each bucket as NULL.
    unsigned int i;
    for (i = 0; i < numBuckets; ++i)
    {
        newTable->buckets[i] = NULL;
    }

    // Return the new HashTable struct.
    return newTable;
}
/**
 * destroyHashTable
 *
 * Destroys and frees all aspects of given Hash Table.
 *
 * @param hashTable The pointer to the hash table
 */
void destroyHashTable(HashTable *hashTable)
{
    if (hashTable == NULL) {
        return; // hashtable does not exist
    }
    for (unsigned int i = 0; i < hashTable->num_buckets; i++) { // loop through buckets
        HashTableEntry *tempEntry = hashTable->buckets[i]; // set temp to head
        while (tempEntry != NULL) {
            HashTableEntry *nextEntry = tempEntry->next;
            free(tempEntry->value); // frees value pointer
            deleteItem(hashTable, tempEntry->key); // deletes AND FREES nodes
            tempEntry = nextEntry;
        }
    }
    free(hashTable->buckets); // free buckets
    free(hashTable); // free hash table
}
/**
 * insertItem
 *
 * Function used to insert an item/node into a passed in hashtable, if the key exists, replace the value.
 *
 * @param hashTable The pointer to the hash table
 * @param key The key corresponds to the hash table entry
 * @param value The value at which you would like to insert
 * @return previous value if key already exists, NULL if key does not exist and is sucessful
 */
void *insertItem(HashTable *hashTable, unsigned int key, void *value)
{
    if (hashTable == NULL) {
        return NULL; // hashtable does not exist
    }
    HashTableEntry *existingEntry = findItem(hashTable, key); // is key present?
    if (existingEntry != NULL) { // if key is found
        void *prevValue = existingEntry->value;
        existingEntry->value = value; // update the value of the existing entry
        return prevValue;
    } else { // key is not present
        unsigned int ind = (hashTable->hash)(key); // finds index
        HashTableEntry *newEntry = createHashTableEntry(key, value); // creates the entry
        if (newEntry == NULL) {
            return NULL; // memory allocation error
        }

        newEntry->next = hashTable->buckets[ind]; // sets the entry's next to the old head of the list
        hashTable->buckets[ind] = newEntry; // sets the head to the added entry
        //hashTable->count++;

        return NULL; // no old value to return
    }
}
/**
 * getItem
 *
 * Uses findItem to return a value from a key. Returns null if key is not found.
 *
 * @param hashTable The pointer to the hash table
 * @param key The key corresponds to the hash table entry
 * @return The value you are trying to pull
 */
void *getItem(HashTable *hashTable, unsigned int key)
{
    HashTableEntry *testEntry = findItem(hashTable, key); // is key present?
    if (testEntry == NULL) {
        return NULL; // key not found
    }

    return testEntry->value; // return the value of the entry, key is present
}
/**
 * removeItem
 *
 * Removes but does not delete / free entry. Returns old value or null if does not exist.
 *
 * @param hashTable The pointer to the hash table
 * @param key The key corresponds to the hash table entry
 * @return The old value of the node you are removing or NULL if not found
 */
void *removeItem(HashTable *hashTable, unsigned int key)
{
    if (hashTable == NULL) {
        return NULL; // hashtable does not exist
    }
    unsigned int ind = (hashTable->hash)(key); // finds index or bucket number
    HashTableEntry *currEntry = hashTable->buckets[ind]; // head entry
    HashTableEntry *prevEntry = NULL; // iterative purposes
    if (currEntry == NULL) {
        return NULL; // not found
    }

    if (currEntry->key == key) { // head is node
        hashTable->buckets[ind] = currEntry->next; // sets new head
        return currEntry->value; // returns old value
    }
    while (currEntry != NULL && (currEntry->key != key)) { // node not in head, search
        prevEntry = currEntry;
        currEntry = currEntry->next;
    }
    if (currEntry == NULL) { // key is absent
        return NULL;
    }
    prevEntry->next = currEntry->next; // unlinks node
    //free(currEntry);

    return currEntry->value; // returns old value
}
/**
 * deleteItem
 *
 * Function that utilizes remove and deletes node entirely.
 *
 * @param hashTable The pointer to the hash table
 * @param key The key corresponds to the hash table entry
 */
void deleteItem(HashTable *hashTable, unsigned int key)
{
    if (hashTable == NULL) {
        return; // hashtable does not exist
    }

    HashTableEntry *testEntry = findItem(hashTable, key);
    if (testEntry != NULL) {
        removeItem(hashTable, key);
        free(testEntry); // frees if key is present
    }
}
