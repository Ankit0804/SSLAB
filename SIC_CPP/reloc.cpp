#include <iostream>
#include <fstream>
#include <sstream>  
#include <cstring>
#include <stdlib.h>
#include <bitset>

using namespace std;


void parse_src()
{

    fstream input_file,output_file;
    string line,temp;
    
    unsigned int newstart = 0,oldstart = 0,addr,opcode, inc,bmask;
    
    //get new start address
    cout << "Enter the actual starting address" << endl;
    cin >>hex>>newstart;

    //i/o files
    input_file.open("reloc.txt", ios::in );
    output_file.open("reloc_output.txt", ios::out);

    while(getline(input_file,line)) //read each line
    {
       stringstream tokens; //stringstream for tokenisation
       addr = 0;
       tokens<<line; //add line to string stream
       if(line[0] == 'H'){
            tokens>>temp>>temp; //read H and PROG name to temp
            tokens>>hex>>oldstart;  //Read original start address
            inc = newstart-oldstart; //compute increment
            continue;
        }
        if(line[0]== 'E')
        {
            break;
        }
        else if(line[0] == 'T')
        {
            tokens>>temp;//skip T char
            //read start address and bit mask
            tokens>>hex>>addr;
            tokens>>hex>>bmask;
            //update start address
            addr+=inc;
            while(tokens>>hex>>opcode)
            {
                //for each opcode check if modification is needed
                if(bmask & 0x800)
                {
                    //if modification is needed, add increment
                    opcode+=inc;
                }
                output_file<<uppercase<<hex<<addr<<"\t"<<hex<<opcode<<endl;
                //update location counter by calculating length of opcode digits
                stringstream hexdigit;
                hexdigit<<hex<<opcode;
                addr+=hexdigit.str().length()/2;
                
                bmask= bmask<<1;//shift bitmask by 1 for next opcode
            }
        }
    }
    input_file.close();
    output_file.close();
}



int main()
{
    parse_src();
}