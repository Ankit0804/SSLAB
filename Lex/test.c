/**
 * pass2.c	
 * Description: Pass 2 of a two pass assembler
 * Author: Arjun Rao (1MS14IS018)
 * Course: System Software Laboratory
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "hashmap.h"



int parse_optab (char *fname)
{
	//load file name given
	bool loaded = load(fname);
	if (!loaded)
	{
        	printf("Could not load %s.\n", fname);
	        return 0;
	}
	return 1;
}

void initTextRecord(char *trec, unsigned int loc)
{
	char tempstr[10];
	trec[0] = 'T';
	trec[1] = '\0';
	sprintf(tempstr,"%06xYY",loc);
	strcat(trec,tempstr);
}

void writeTextRecord(char *trec,FILE* fobj)
{
	char tempstr[10];
	unsigned int tempLen = 0;
	for(int i=9;trec[i]!='\0';i++)
	{
		tempLen++;
	}
	tempLen = (tempLen%2==0)?(tempLen*4)/8:(tempLen+1)*4/8;
	sprintf(tempstr,"%02x",tempLen);
	trec[7] = tempstr[0];
	trec[8] = tempstr[1];
	if(tempLen == 0) return;
	fprintf(fobj,"%s\n",trec);
	printf("%s\n",trec);
}



int pass2 (char *intermfile)
{
	FILE *fsrc,*fobj;
	char buff[257],*token,temp[8];
	char hrec[20],trec[70],erec[8];
	//declare location counter
	unsigned int loc = 0,hexno = 0;
	unsigned int lineno = 0;
	//open source file
	fsrc = fopen(intermfile, "r");
	if (fsrc == NULL);
}
   	