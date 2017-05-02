#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <stdlib.h>

using namespace std;


map <string, string> optab,symtab;
map <string, string>::iterator iter;

void parse_support()
{
    fstream file;
    string input1,input2;
    file.open("optab.txt",ios::in);

    while( file>>input1>>input2)
    {
        optab.insert(make_pair(input1,input2));
    }
    file.close();
    file.open("symtab.txt",ios::in);

    while( file>>input1>>input2)
    {
        symtab.insert(make_pair(input1,input2));
    }
    file.close();
}

void parse_src()
{
    int hexno = 0;
    int locctr = 0, lineno = 0;
    fstream input_file, output_file;
    string line;
    char *token;
    input_file.open("locctr.txt", ios::in );
    output_file.open("interm.txt", ios::out);

    while(getline(input_file,line))
    {
        char tokens[3][20]={0,0,0},hexloc[5],objcode[7]={0};
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
        
        // Check if Mnemonic is START or END
        if (strcmp(tokens[1],"START") == 0 || strcmp(tokens[1],"END") == 0)
        {
            output_file << line << endl;
            continue;
        }
     
        // Checks if MNEMONIC exists in optab
        else if ( optab.find(tokens[1]) == optab.end())
        {
            cout << "\nERROR in line " << lineno << endl;
            break;
        }
        output_file << line <<"\t";
        iter = optab.find(tokens[1]);

        if(iter->second != "*")
        {
           hexno = 0;
           strcpy(objcode,iter->second.c_str());
           
            char * ptr = strstr(tokens[2],",X");
            if(ptr!=NULL)
            {
                *ptr = '\0';
                //Uses index register so or hexno with 2^15 (0x8000)
               
                iter = symtab.find(tokens[2]);
                if (iter != symtab.end())
                {
                    hexno = strtol(iter->second.c_str(),NULL,16);
                    hexno = hexno ^ 0x8000;
                }
                sprintf(hexloc,"%04X\n",hexno);
                strcat(objcode,hexloc);
                output_file << objcode;
            } 
            else
            {              
                iter = symtab.find(tokens[2]);
                if (iter != symtab.end())
                {
                    hexno = strtol(iter->second.c_str(),NULL,16); 
                             
                }
                sprintf(hexloc,"%04X\n",hexno);
                strcat(objcode,hexloc);
                output_file << objcode;    
            }
            

        }
        else
        {
            //Check data stuff

            if(strcmp(tokens[1],"WORD")==0)
            {
                hexno = strtol(tokens[2],NULL,10);
                sprintf(objcode,"%06X\n",hexno);
                output_file<<objcode;
            }
            else if(strcmp(tokens[1],"BYTE")==0)
            {
                if(tokens[2][0] == 'C')
                {
                    for(int i = 2;tokens[2][i]!='\'';i++)
                    {
                        sprintf(objcode,"%s%02X",objcode,tokens[2][i]);
                    }
                    output_file<<objcode<<endl;
                }
                else if(tokens[2][0]=='X')
                {
                    for(int i = 2;tokens[2][i]!='\'';i++)
                    {
                        sprintf(objcode,"%s%c",objcode,tokens[2][i]);
                    }
                    output_file<<objcode<<endl;
                }
                else
                {
                    cout << "\nERROR in line " << lineno << endl;
                }
            }
            else
            {
                //RESW or RESB
                output_file<<endl;
            }   
        }
    }
}

int main()
{
    parse_support();
    parse_src();
}