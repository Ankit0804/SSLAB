#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char label[10], opcode[10], operand[10], maclabel[10];
	char label1[10], opcode1[10], operand1[10];
	char l1[10];
	int count;
	FILE *f1, *f2, *f3;

	f1 = fopen("input_macro.txt", "r");
	f2 = fopen("output_macro.txt", "w");
	f3 = fopen("macro.txt", "w");

	fscanf(f1, "%s%s%s", label, opcode, operand);
	while(strcmp(opcode, "END") != 0)
	{
		if(strcmp(opcode, "MACRO") == 0)
		{	
			strcpy(maclabel, label);
			while(strcmp(opcode, "MEND") != 0)
			{
				fscanf(f1, "%s%s%s", label, opcode, operand);
				if(strcmp(opcode, "MEND") != 0)
					fprintf(f3, "%s\t%s\t%s\n", label, opcode, operand);
			}
		}
		fscanf(f1, "%s%s%s", label, opcode, operand);
	}

	fclose(f1);
	fclose(f3);

	f1 = fopen("input_macro.txt", "r");
	
	while(fscanf(f1, "%s%s%s", label, opcode, operand) != EOF)
	{
		if(strcmp(opcode, "END") == 0) 
			break;
		else if(strcmp(opcode, maclabel) == 0)
		{
			strcpy(l1, label);
			f3 = fopen("macro.txt", "r");
			count = 1;
			while(fscanf(f3, "%s%s%s", label1, opcode1, operand1)!=EOF)
			{
				if(count == 1)
				{
					fprintf(f2, "%s\t%s\t%s\n", l1, opcode1, operand1);
					count = 0;
				}
				else
					fprintf(f2, "%s\t%s\t%s\n", label1, opcode1, operand1);
			}
			fclose(f3);
		}
		else if(strcmp(opcode, "MACRO") != 0 && strcmp(opcode, "MEND") != 0)
		{
			fprintf(f2, "%s\t%s\t%s\n", label, opcode, operand);	
		}
	}	

	fclose(f1);
	fclose(f2);

	return 0;
}
