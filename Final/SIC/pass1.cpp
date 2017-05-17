#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;

map<string,string> optab,symtab;
map<string,string>::iterator iter;

void readoptab(string filename)
{
    string mnemonic,opcode;
    fstream f(filename,ios::in);
    while(f >> mnemonic >>opcode )
    {
        optab[mnemonic] = opcode;
    }
   f.close();
}


void writesymtab(string filename)
{
    fstream f(filename,ios::out);
    for(iter = symtab.begin();iter!=symtab.end();iter++)
    {
        f << iter->first <<"\t"<<iter->second<<"\n";
    }

    f.close();
}
void tokenize(string line,string &l,string &m,string &o)
{
    stringstream tokens(line);
    getline(tokens,l,'\t');
    getline(tokens,m,'\t');
    getline(tokens,o,'\n');
}
void pass1(string ifile,string ofile)
{
    fstream infile(ifile,ios::in);
    int lineno=0,loc = 0;
    fstream outfile(ofile,ios::out);
    string line,label,mnemonic,operand;
    
    // Read lines from input (label, mnemonic, operand) or comment
    while(getline(infile,line))
    {
        lineno++;
        //handle comments        
        if(line[0] == '.')
        {
            outfile<<line<<endl;
            continue;
        }
        tokenize(line,label,mnemonic,operand);
        
        //handle start
        if(mnemonic == "START")
        {
            stringstream temp(operand);
            temp>>hex>>loc;
            outfile<<"\t"<<line<<endl;
            continue;
        }
        //handle end
        if(mnemonic == "END")
        {
            stringstream temp;
            temp<<setfill('0')<<setw(4)<<uppercase<<hex<<loc;
            symtab[mnemonic]=temp.str();
            outfile<<temp.str()<<"\t"<<line<<endl;
            break;
        }
        //handle symbols
        if ( label.size() != 0 )
        {
            if((iter=symtab.find(label))!=symtab.end())
            {
                cout<<"Line:"<<lineno<<" - Error! Symbol "<<iter->first<<" redefined!\n";
                break;
            }
            //save location
            stringstream temp; 
            temp <<setfill('0')<<setw(4)<<uppercase<<hex<<loc;
            symtab[label] = temp.str();
        }
        outfile<<setfill('0')<<setw(4)<<uppercase<<hex<<loc<<"\t"<<line<<"\n";
        //handle location counter updates
        if((iter = optab.find(mnemonic))!=optab.end())
        {         
            if(iter->second != "*")
            {
                //normal instruction, increment locctr by 3 bytes;
                loc+=3;
            }
            else
            {
                // Handle resw, resb
                if(iter->first == "RESW")
                {
                    stringstream temp(operand);
                    int inc;
                    temp>>inc;
                    loc+=inc*3;
                }
                else if(iter->first == "RESB" )
                {
                    stringstream temp(operand);
                    int inc;
                    temp>>inc;
                    loc+=inc;
                }
                // Handle byte,word
                else if(iter->first == "WORD")
                {
                    loc+=3;
                }
                else if(iter->first == "BYTE")
                {
                    if(operand[0]=='C')
                        loc+= operand.size()-3;
                    else if(operand[0] == 'X')
                    {
                        int inc = operand.size()-3;
                        loc+= (inc%2==0?inc/2:(inc+1)/2);
                    }
                    else
                    {
                        cout<<"Error, Invalid Symbol on line: "<<lineno<<endl;
                    }
                }                
            }
        }
        else
        {
            cout<<"Error: Undefined Opcode on line " << lineno<<endl;
            break;
        }
    }

    infile.close();
    outfile.close();

}


int main()
{
    readoptab("optab.txt");
    pass1("input.txt","output.txt");
    writesymtab("symtab.txt");

}
