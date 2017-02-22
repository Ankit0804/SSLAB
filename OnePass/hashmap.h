/**
 * hashmap.c	
 * Description: Declares a hashmap for storing opcodes
 * Author: Arjun Rao (1MS14IS018)
 * Course: System Software Laboratory
 */

#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// maximum length for a code
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 6
#define OPLENGTH 2
#define HASHMAX 4096
/**
 * Returns true if code is in hashmap else false.
 */
const char * check(const char* code);

/**
 * Loads hashmap into memory.  Returns true if successful else false.
 */
bool load(const char* optabfile);

/**
 * Returns number of codes in hashmap if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads hashmap from memory.  Returns true if successful else false.
 */
bool unload(void);

/**
 * Returns the hash value for a given string as unsigned long int
 * Using djb2 algorithm for computing the hash value
 */
unsigned long hash(char *str);




#endif // HASHMAP_H