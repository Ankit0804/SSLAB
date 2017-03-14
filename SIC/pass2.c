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




int pass2 (char *intermfile)
{
	FILE *fsrc,*fobj;
	char buff[255],*token,temp[8];
	
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
		char *str,cbuff[255],line[3][255],instruction[6];
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
		if (strcmp(line[0],"START") == 0)
		{
			loc = strtol(line[1], NULL, 16);
			printf("START\t%x\n",loc);
			//fprintf(fobj,"START\t%x\n",loc);
			continue;
		}
		else if (strcmp(line[1],"END")==0){ }
		else if (checkop(line[1]) == NULL)
		{
			printf("\nERROR in line %d\n",lineno);
			break;
		}
		//printf(floc,"%s,%s,%s\n",line[0],line[1],line[2]);
		
		
		//print out location counter and write to file
		loc = strtol(line[0], NULL, 16);
		printf("%x\t",loc);
		fprintf(fobj,"%x\t",loc);
		
		
		 
		//write to file		
		printf("%s\t%s\t",line[1],line[2]);
		//fprintf(fobj,"%s\t%s\n",line[1],line[2]);
		
		//handle object code generation
		if(strcmp(checkop(line[1]),"*")!=0 && checksym(line[2]))
		{
			strcpy(instruction,checkop(line[1]));
			strcat(instruction,checksym(line[2]));
			printf("%s\n",instruction);
		}		
		else if(strcmp(checkop(line[1]),"*")==0)
		{
			
			//Handle BYTE
			if(strcmp(line[1],"BYTE")==0)
			{
				
				if(line[2][0] == 'C')
				{
					//loc +=  strlen(line[2])-3;
					for(int index = 1;line[2][index]!='\0';index++)
					{
						if(line[2][index]!='\'')
							printf("%02x", line[2][index]);		
					}
					printf("\n");
				}
				else if(line[2][0] == 'X')
				{
					//int hexdigits = strlen(line[2])-3;
					//loc += (hexdigits%2==0?hexdigits/2:(hexdigits+1)/2);'
					for(int index = 1;line[2][index]!='\0';index++)
					{
						if(line[2][index]!='\'')
							printf("%c", line[2][index]);		
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
				//loc += strtol(line[2], NULL, 10);
				printf("\n");
			}
			else if(strcmp(line[1],"RESW")==0)
			{
				//loc += strtol(line[2], NULL, 10)*3;
				printf("\n");
			}
			else if(strcmp(line[1],"WORD")==0)
			{
				//loc += 3;
				hexno = strtol(line[2], NULL, 16);
				sprintf(instruction,"%x",hexno);
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
		
	
	if(argc != 2)
	{
		printf("Usage: ./pass2 optab\n");
		return 1;	
	}	
	printf("Parsing optab file: %s\n",argv[1]);

	if (!parse_optab(argv[1])) return 1;

	printf("Reading from SYMTAB.txt and LOCCTR.txt\n");
	printf("Started Pass2\n");	
	loadsym("SYMTAB.txt");
	if (pass2("LOCCTR.txt")) return 1;
	printf("Completed Pass2\n");
	unload();
	return 0;	
}
