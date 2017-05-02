#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <stdlib.h>

using namespace std;


map <string, string> optab;
map <string, string>::iterator iter;

void parse_optab()
{
    fstream file;
    string input1,input2;
    file.open("optab.txt",ios::in);

    while( file>>input1>>input2)
    {
        optab.insert(make_pair(input1,input2));
    }
}

void parse_src()
{
    //set up variable for locctr, lineno, symtab and locctr files;
    int locctr = 0, lineno = 0;
    fstream input_file, sym_file, output_file;
    string line;

    char *token;
    
    input_file.open("input.txt", ios::in );
    output_file.open("locctr.txt", ios::out);
    sym_file.open("symtab.txt", ios::out);

    while(getline(input_file,line))
    {
        char tokens[3][30], hexloc[5];
        lineno++;

        //check for comment line
        if(line[0] == '.')
        {
            output_file << line <<endl;
            continue;
        }
        int i=0;
        
        char* str = strdup(line.c_str());

        //extract tokens
        while((token = strsep(&str,"\t"))&& i<3)
        {
            tokens[i][0] = '\0';
            strcpy(tokens[i],token);
            i++;
        }

        //check for START

        if(strcmp(tokens[1],"START") == 0)
        {
            locctr = strtol(tokens[2],NULL,16);
            output_file <<"\t"<<line<<endl;
            continue;
        }

        else if (strcmp(tokens[1],"END")==0)
        {
            sprintf(hexloc, "%X", locctr);
            sym_file << tokens[1] <<"\t" << hexloc << endl;
            output_file << hexloc << "\t\t" << tokens[1]<< endl;
            continue;
        }

        // Checks if MNEMONIC exits in optab
        else if ( optab.find(tokens[1]) == optab.end())
        {
            cout << "\nERROR in line " << lineno << endl;
            break;
        }

        sprintf(hexloc, "%X", locctr);
        output_file<< hexloc <<"\t"<<tokens[1]<<"\t"<<tokens[2]<<"\n";
        if(strlen(tokens[0])!=0)
        {
            sym_file << tokens[0] <<"\t" << hexloc << endl;
        }

        //find object code for RESB and other
        iter = optab.find(tokens[1]);
        if(iter->second == "*")
        {
            if(strlen(tokens[0])==0){
                cout << "Error: No label defined in line " << lineno << endl;
				break;
            }
            if(strcmp(tokens[1],"WORD")==0)
            {
                locctr+=3;
            }
            else if(strcmp(tokens[1],"RESW")==0)
            {
                locctr += strtol(tokens[2], NULL, 10)*3;
            }
            else if(strcmp(tokens[1],"RESB")==0)
            {
                locctr += strtol(tokens[2], NULL, 10);
            }
            else if(strcmp(tokens[1],"BYTE")==0)
            {
                if(tokens[2][0] == 'C')
                {
                    locctr+= strlen(tokens[2])-3;
                }
                else if(tokens[2][0] == 'X')
                {
                    int hexdigits = strlen(tokens[2])- 3;
                    locctr += ( hexdigits % 2 == 0 ? hexdigits/2 :(hexdigits+1)/2 );
                }
                else
                {
                    cout << "\nERROR in line " << lineno << endl;
                    break;
                }

            }
        }
        else 
        {
					locctr += 3;
		}        
    }
    input_file.close();
    sym_file.close();
    output_file.close();
}

int main()
{
    parse_optab();
    parse_src();
}