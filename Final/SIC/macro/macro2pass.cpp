#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

map <string, string> deftab;
map<string,string>::iterator iter;

void readMacros()
{
    fstream input_file, middle_file;
    string line, label, opcode, operand,mname;
    int count = 1;

	// Open files
	input_file.open("macroinput.txt", ios::in );
    middle_file.open("middle.txt", ios::out);

	// Read input file till end
	while ( getline(input_file, line) )
	{
        stringstream s(line);
        getline(s, label, '\t');
        getline(s, opcode, '\t');
        getline(s, operand, '\n');

        // Checks for Macro
        if(opcode == "MACRO")
        {
            // Store each macro in a different string
            stringstream mdef;
            

            // Store the definition along with macro name            
            mname = label;            

            while( getline(input_file, line) )
            {
                stringstream s2 (line);
                getline(s2, label, '\t');
                getline(s2, opcode, '\t');
                getline(s2, operand, '\n');
                if(line[0]=='.') continue;
                if(opcode == "MEND")
                {                    
                    deftab[mname]=mdef.str();                 
                    break;
                }
                // Write line to macro file
                mdef<<line<<endl;               
            }
        }
        else
        // Stores all lines from input_file excluding macro definitions
        middle_file << line << endl;
  }
  input_file.close();
}

void parseInput()
{
    fstream input_file, output_file;
    string line, label, opcode, operand;

    // Open files
    input_file.open("middle.txt", ios::in );
    output_file.open("macrooutput.txt", ios::out);

    // Read input file till end
    while ( getline(input_file, line) )
	{
        stringstream s(line);
        getline(s, label, '\t');
        getline(s, opcode, '\t');
        getline(s, operand, '\n');

        if(opcode == "END")
        {
            output_file << line << endl;
            break;
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
  readMacros();
  parseInput();
  return 0;
}
