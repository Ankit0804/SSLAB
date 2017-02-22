/**
 * hashmap.c	
 * Description: Implements a hashmap for storing opcodes
 * Author: Arjun Rao (1MS14IS018)
 * Course: System Software Laboratory
 */
 

#include <stdbool.h>
#include <stdlib.h>

#include "hashmap.h"


/**
 * Create a structure for the hash table
 */
 
typedef struct node 
{
  char code[LENGTH+1]; //stores mnemonic
  char opcode[OPLENGTH+1]; //stores opcode
  struct node* link;
} node;

typedef struct hash_t
{
  unsigned int size;
  node **data;
} hash_t;

hash_t *table,*symtab;

/**
 * Returns opcode if mnemonic is in hashmap else NULL.
 */
const char* check(const char* code)
{
    char tempw[LENGTH+1];
    strcpy(tempw,code);
    for(int i=0;tempw[i]!='\0';i++) tempw[i] = toupper(tempw[i]);
    unsigned int hashval = hash(tempw);
    if(table->data[hashval]==NULL) return false;
    else
    {
        node *temp = table->data[hashval];
        while(temp!=NULL)
        {
            if(strcmp(tempw,temp->code)==0) return temp->opcode;
            else
            {
                temp = temp->link;
            }
        }   
    }
    return NULL;
}

node *insert(node *pointer, char * mnemonic, char * opcode)
{
    node *temp;
    if(pointer == NULL) // first node;
    {
        pointer = malloc(sizeof(node));
        strcpy(pointer->code,mnemonic);
	strcpy(pointer->opcode,opcode);
        pointer->link = NULL;
    }
    else //insert in the first position;    
    {
        temp = malloc(sizeof(node));
        strcpy(pointer->code,mnemonic);
	strcpy(pointer->opcode,opcode);
        temp->link = pointer;
        pointer = temp;
    }
    temp = NULL;
    return pointer;
}


/**
 * Loads hashmap into memory.  Returns true if successful else false.
 */
bool load(const char* optabfile)
{
   unsigned long hashval;
   char mncode[LENGTH+1],opcode[OPLENGTH+1];
   table = malloc(sizeof(hash_t));
   symtab = malloc(sizeof(hash_t));
   table->size=0;
   symtab->size=0;
   FILE* fp = fopen(optabfile, "r");
   if (fp == NULL)
    {
        printf("Could not open %s.\n", optabfile);
        unload();
        return 1;
    }
   
   // Allocate memory for the actual data
   table->data = malloc ( HASHMAX * sizeof(node *));
   for(int i =0;i<HASHMAX;i++) table->data[i] = NULL;
   while(fscanf(fp,"%s",mncode)!=EOF)
   {
        //read a mnemonic and opcode, now insert into hashtable;
	if(fscanf(fp,"%s\n",opcode)!=EOF)
	{
		//Get hashvalue
		hashval = hash(mncode);
		//insert code at hashval in table
		table->data[hashval] = insert(table->data[hashval],mncode,opcode);
		table->size++;
	}
   }
   fclose(fp);
      
   return true;
}

/**
 * Returns number of codes in hashmap if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return table->size;
}

/**
 * Unloads hashmap from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    node *temp,*temp2;
    if(table !=NULL)
    {
        if(table->data!=NULL)
        {
            for(int i=0;i<HASHMAX;i++)
            {
                if(table->data[i]!=NULL)
                {
                    temp= table->data[i];
                    while(temp!=NULL)
                    {
                        temp2 = temp;
                        temp= temp->link;
                        temp2->link = NULL;
                        free(temp2);
                        //temp2=NULL;
                    }
                   table->data[i]=NULL;
                    //table->data[i]=NULL;
                }
                                
            }
            free(table->data);
            
        }
        free(table);
    }
    return true;
}


unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash%HASHMAX;
}

