#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

map <string, string> deftab;
map<string,string>::iterator iter;

void tokenize(string line,string&l,string &op,string &oper)
{
    stringstream s(line);
    getline(s, l, '\t');
    getline(s, op, '\t');
    getline(s, oper, '\n');
}
void parseInput()
{
    fstream input_file, output_file;
    string line, label, opcode, operand,mname;

    // Open files
    input_file.open("macroinput.txt", ios::in );
    output_file.open("macrooutput.txt", ios::out);

    // Read input file till end
    while ( getline(input_file, line) )
	{
        tokenize(line,label,opcode,operand);

        if(opcode == "END")
        {
            output_file << line << endl;
            break;
        }
        if (opcode == "MACRO")
        {
            mname = label;
            stringstream macro;
            while(getline(input_file,line))
            {
                if(line[0]=='.') continue;
                tokenize(line,label,opcode,operand);
                if(opcode == "MEND")
                {
                    deftab[mname] = macro.str();
                    break;
                }
                macro<<line<<endl;
            }   
        }
        // If macro is called
        else if ( (iter = deftab.find(opcode)) != deftab.end())
        {
            output_file<<"."<<opcode<<endl;
            // Write the label (if any) before macro call to file
            output_file << label;            
            // Write out contents of macro_defintion to output file
            output_file << iter->second << endl;            
        }

        else
            {
                // Write out other lines
                output_file << line << endl;
            }
  }
}

int main ()
{
  parseInput();
  return 0;
}
