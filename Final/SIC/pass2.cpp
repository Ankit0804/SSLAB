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

void read_tables(string symfile,string opfile)
{
    string mnemonic,opcode;
    fstream f(opfile,ios::in);
    while(f >> mnemonic >>opcode )
    {
        optab[mnemonic] = opcode;
    }
    f.close();
    f.open(symfile,ios::in);
    while(f >> mnemonic >>opcode )
    {
        symtab[mnemonic] = opcode;
    }
    f.close();
}

void tokenize(string line,string &lo,string &la,string &m,string &o)
{
    stringstream tokens(line);
    getline(tokens,lo,'\t');
    getline(tokens,la,'\t');
    getline(tokens,m,'\t');
    getline(tokens,o,'\t');
}

string generate(string line);
void pass2a(string ifile,string ofile);
void pass2b(string ifile,string ofile){}

int main()
{
    read_tables("symtab.txt","optab.txt");
    pass2a("outpass1.txt","outpass2a.txt");
}

void pass2a(string ifile,string ofile)
{
    //to generate object codes, open in/out files, store lineno, tokens
    fstream infile(ifile,ios::in);
    int lineno=0,loc = 0;
    fstream outfile(ofile,ios::out);
    string line,objcode;
    
    while(getline(infile,line))
    {
        lineno++;
        //if comment, skip
        if(line[0] == '.')
        {
            outfile<<line<<endl;
            continue;        
        }        
        //generate object code for given line
        objcode = generate(line);    
        if(objcode == "ERROR"){ cout<<"ERROR at line"<<lineno<<endl;break;}
        outfile<<line<<"\t"<<objcode<<"\n";
    }

    infile.close();
    outfile.close();
}

string generate(string line)
{    
    bool index = false;
    string loc,lab,mne,op,objcode= "";    
    stringstream temp;
    tokenize(line,loc,lab,mne,op);    
    
    if(mne == "START" || mne == "END") return "";
    
    iter = optab.find(mne);
    
    if(iter->second != "*") //normal instruction - not data
    {
        objcode = iter->second;
        if(op.size()==0) // like rsub
        {
            temp<<objcode<<setfill('0')<<setw(4)<<hex<<0;            
        }
        else
        {   
            if(op.find(",X")!=string::npos)   
            {
                //it contains ,X - so lets get the original symbol
                op.replace(op.find(",X"),op.length(),"");  
                if(symtab.find(op)==symtab.end()){
                    return "ERROR";
                }
                index=true;
            }
            stringstream temp2(symtab.find(op)->second);
            int addr;
            temp2>>hex>>addr;
            if(index)addr^=0x8000;
            temp<<objcode<<uppercase<<hex<<addr;
        }
               

    }
    else //data instructions
    {
        //handle RESW && RESB
        if(mne == "RESW" || mne == "RESB") return "";
        
        if (mne == "BYTE")
        {            
            if(op[0] == 'C') index = true;
            op = op.substr(2);            
            if(index)
            {    //convert ascii value of each char to hex and store it
                for(int i = 0; i < op.size()-1; i++)
                    temp << uppercase << hex << (int)op[i];                

            }            
            else 
            {                
                //append the hex values directly to obj code
                for(int i = 0; i < op.size()-1; i++)
                    temp<<op[i];
            }
        }
        else if (mne == "WORD")
        {
            stringstream temp2(op);
            int addr;
            temp2>>addr;           
            temp<<setfill('0')<<setw(6)<<uppercase<<hex<<addr;
        }
        
        
    }    
    objcode = temp.str();
    return objcode;
}