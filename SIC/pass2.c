/**
 * pass1.c	
 * Description: Pass 1 of a two pass assembler
 * Author: Arjun Rao (1MS14IS018)
 * Course: System Software Laboratory
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "hashmap.h"

char* strdup(const char* s)
{
    char* p = malloc(strlen(s)+1);
    if (p) strcpy(p, s);
    return p;
}


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
	char tempstr[6];
	trec[0] = 'T';
	trec[1] = '\0';
	sprintf(tempstr,"%06xYY",loc);
	strcat(trec,tempstr);
}

void writeTextRecord(char *trec,FILE* fobj)
{
	char tempstr[6];
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
	char buff[255],*token,temp[8];
	char hrec[19],trec[69],erec[7];
	//declare location counter
	unsigned int loc = 0,hexno = 0;
	unsigned int lineno = 0;
	//open source file
	fsrc = fopen(intermfile, "r");
	if (fsrc == NULL)
   	{
		printf("Could not open %s.\n", intermfile);
		unload();
		return 1;
   	}	

	//setup output files
	fobj = fopen("OBJCODE.txt", "w");
	
	if (fobj == NULL)
   	{
		printf("Could not write outputs \n");
		unload();
		return 1;
   	}
	
	while(fgets(buff, 255, fsrc) != NULL)
	{
		lineno++;
		//read a line
		char *str,cbuff[255],line[3][255],tempstr[6],finst[6],instruction[6];
		strcpy(cbuff,buff);
		str = strdup (cbuff);
		//handle comment line
		if(buff[0] == '.') continue;

		int i=0;
		//EXTRACT LABEL,MNEMONIC, OPERAND
		while ((token = strsep(&str, "\t")) && i<3){
			strcpy(line[i],token);		
			for(int j=0,k=strlen(line[i]);j<k;j++)
			{
				if(line[i][j]=='\n'){
					line[i][j]='\0';
					break;
				}
			}
			i++;
		} 
		
		//Check if first Mnemonic is start
		if (strcmp(line[1],"START") == 0)
		{
			loc = strtol(line[2], NULL, 16);
			//print header record
			//get END address
			hexno = strtol(checksym("END"),NULL,16);
			
			//prepare Header record
			hrec[0]='H';
			for(int i=1;i<7;i++)
			{
				hrec[i]=line[0][i-1]!='\0'?line[0][i-1]:' ';
			}
			hrec[7]='\0';
			sprintf(tempstr,"%06x",loc);
			strcat(hrec,tempstr);
			sprintf(tempstr,"%06x",hexno-loc);
			strcat(hrec,tempstr);

			//write header record
			fprintf(fobj,"%s\n",hrec);
			printf("%s\n",hrec);

			//initialise text record
			initTextRecord(trec,loc);
			sprintf(erec,"E%06x",loc); //address of first executable instruction for E Record
			continue;
		}
		else if (strcmp(line[1],"END")==0){
			loc = strtol(line[0], NULL, 16);
			printf("%x\t%s\n",loc,line[1]);
			//write last text record
			writeTextRecord(trec,fobj);
			fprintf(fobj,"%s\n",erec);
			printf("%s\n",erec);
			continue;
		}
		else if (checkop(line[1]) == NULL)
		{
			printf("\nERROR in line %d\n",lineno);
			break;
		}
		//printf(floc,"%s,%s,%s\n",line[0],line[1],line[2]);
		
		
		//print out location counter and write to file
		loc = strtol(line[0], NULL, 16);
		printf("%x\t",loc);
		//fprintf(fobj,"%x\t",loc);
		
		
		 
		//write to file		
		printf("%s\t%s\t",line[1],line[2]);
		//fprintf(fobj,"%s\t%s\n",line[1],line[2]);
		

		//handle object code generation
		if(strcmp(checkop(line[1]),"*")!=0)
		{
			strcpy(instruction,checkop(line[1]));
			if(checksym(line[2]) || strstr(line[2],",X")!=NULL)
			{
				char * ptr = strstr(line[2],",X");
				if(ptr!=NULL)
				{
					*ptr = '\0';
					//Uses index register so or hexno with 2^15 (0x8000)
					hexno = strtol(checksym(line[2]),NULL,16);
					hexno = hexno ^ 0x8000;
					
				}
				else
					hexno = strtol(checksym(line[2]),NULL,16);
				sprintf(tempstr,"%x",hexno);
			}
			else {
				strcpy(tempstr,"0000");
			}
			strcat(instruction,tempstr);
			if(strlen(trec)+strlen(instruction)> 69)
			{
				writeTextRecord(trec,fobj);
				initTextRecord(trec,loc);
				
			}
			strcat(trec,instruction);
			printf("%s\n",instruction);
		}		
		else if(strcmp(checkop(line[1]),"*")==0)
		{
			
			//Handle BYTE
			if(strcmp(line[1],"BYTE")==0)
			{
				char byt[3];
				if(line[2][0] == 'C')
				{
					//loc +=  strlen(line[2])-3;
					for(int index = 1;line[2][index]!='\0';index++)
					{
						if(line[2][index]!='\''){
							printf("%02x", line[2][index]);
							sprintf(byt,"%02x", line[2][index]);
							if(strlen(trec)+2> 69)
							{
								writeTextRecord(trec,fobj);
								initTextRecord(trec,loc);
							}		
							strcat(trec,byt);
						}
					}
					printf("\n");
					
					
					
					
				}
				else if(line[2][0] == 'X')
				{
					//int hexdigits = strlen(line[2])-3;
					//loc += (hexdigits%2==0?hexdigits/2:(hexdigits+1)/2);'
					for(int index = 1;line[2][index]!='\0';index++)
					{
						if(line[2][index]!='\''){
							printf("%c", line[2][index]);		
							sprintf(byt,"%c", line[2][index]);
							if(strlen(trec)+2> 69)
							{
								writeTextRecord(trec,fobj);
								initTextRecord(trec,loc);
							}		
							strcat(trec,byt);
						}
					}
					printf("\n");
				}
				else
				{
					printf("\nERROR in line %d\n",lineno);
				}
			}
			else if(strcmp(line[1],"RESB")==0)
			{
				unsigned int tloc = loc+ strtol(line[2], NULL, 10);
				printf("\n");
				writeTextRecord(trec,fobj);
				initTextRecord(trec,tloc);
			}
			else if(strcmp(line[1],"RESW")==0)
			{
				unsigned int tloc = loc+ strtol(line[2], NULL, 10)*3;
				printf("\n");
				writeTextRecord(trec,fobj);
				initTextRecord(trec,tloc);
				
			}
			else if(strcmp(line[1],"WORD")==0)
			{
				//loc += 3;
				hexno = strtol(line[2], NULL, 16);
				sprintf(instruction,"%06x",hexno);
				if(strlen(trec)+strlen(instruction)> 69)
				{
					writeTextRecord(trec,fobj);
					initTextRecord(trec,loc);
					
				}
				strcat(trec,instruction);
				printf("%s\n",instruction);
			}
			
		}
		else
		{
			loc+=3;
		}


	
	}
	fclose(fobj);
	return 0;
	
}







int main(int argc, char* argv[])
{
		
	
	if(argc != 4)
	{
		printf("Usage: ./pass2 optab symtab intermfile\n");
		return 1;	
	}	
	printf("Parsing optab file: %s\n",argv[1]);

	if (!parse_optab(argv[1])) return 1;

	printf("Reading from SYMTAB.txt and LOCCTR.txt\n");
	printf("Started Pass2\n");	
	loadsym(argv[2]);
	if (pass2(argv[3])) return 1;
	printf("Completed Pass2- See OBJCODE.txt\n");
	unload();
	return 0;	
}
