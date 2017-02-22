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


int parse_src (char *srcfilename)
{
	FILE *fsrc,*floc,*fsym;
	char buff[255],*token;
	
	//declare location counter
	unsigned int loc = 0;
	unsigned int lineno = 0;
	//open source file
	fsrc = fopen(srcfilename, "r");
	if (fsrc == NULL)
   	{
		printf("Could not open %s.\n", srcfilename);
		unload();
		return 1;
   	}	

	//setup output files
	floc = fopen("LOCCTR.txt", "w");
	fsym = fopen("SYMTAB.txt", "w");
	
	if (fsym == NULL || floc == NULL)
   	{
		printf("Could not write outputs \n");
		unload();
		return 1;
   	}
	
	while(fgets(buff, 255, fsrc) != NULL)
	{
		lineno++;
		//read a line
		char *str,cbuff[255],line[3][255];
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
			printf("START\t%x\n",loc);
			fprintf(floc,"START\t%x\n",loc);
			continue;
		}
		else if (strcmp(line[1],"END")==0){ }
		else if (check(line[1]) == NULL)
		{
			printf("\nERROR in line %d\n",lineno);
			break;
		}
		//printf(floc,"%s,%s,%s\n",line[0],line[1],line[2]);
		
		
		//print out location counter and write to file
		printf("%x\t",loc);
		fprintf(floc,"%x\t",loc);
		
		
		// symbol in label field
		if(strlen(line[0])!=0)
		{
			printf("%s\t",line[0]);
			fprintf(fsym,"%s\t%x\n",line[0],loc);	
		}
		//write to file		
		printf("%s\t%s\n",line[1],line[2]);
		fprintf(floc,"%s\t%s\n",line[1],line[2]);
		
		//handle location counter update
		if((strlen(line[0])!=0) && (strcmp(check(line[1]),"*")==0))
		{
			
			//Handle BYTE
			if(strcmp(line[1],"BYTE")==0)
			{
				
				if(line[2][0] == 'C')
				{
					loc +=  strlen(line[2])-3;
				}
				else if(line[2][0] == 'X')
				{
					int hexdigits = strlen(line[2])-3;
					loc += (hexdigits%2==0?hexdigits/2:(hexdigits+1)/2);
				}
				else
				{
					printf("\nERROR in line %d\n",lineno);
				}
			}
			else if(strcmp(line[1],"RESB")==0)
			{
				loc += strtol(line[2], NULL, 10);
			}
			else if(strcmp(line[1],"RESW")==0)
			{
				loc += strtol(line[2], NULL, 10)*3;
			}
			else if(strcmp(line[1],"WORD")==0)
			{
				loc += 3;
			}
			
		}
		else
		{
			loc+=3;
		}


	
	}
	fclose(fsrc);
	fclose(fsym);
	fclose(floc);
	return 0;
	
}

int main(int argc, char* argv[])
{
		
	
	if(argc != 3)
	{
		printf("Usage: ./pass1 optab srcfile\n");
		return 1;	
	}	
	printf("Parsing optab file: %s\n",argv[1]);

	if (!parse_optab(argv[1])) return 1;

	printf("Parsing source file: %s\n",argv[2]);

	if (parse_src(argv[2])) return 1;

	printf("Wrote SYMTAB.txt and LOCCTR.txt\n");
	unload();
	return 0;	
}
