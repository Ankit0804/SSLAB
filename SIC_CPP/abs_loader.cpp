#include <fstream>
#include <cstring>
#include <stdlib.h>

using namespace std;

void parseInput()
{

	fstream input_file, output_file;
	string line, addr;
	char hex_loc[4];
	int loc;

	// Open files
	input_file.open("load_input.txt", ios::in );
	output_file.open("abs_load_output.txt", ios::out);

	// Read input file till end
	while ( getline(input_file, line) )
	{
		// Checks for header record
		if(line[0] == 'H')
			continue;

		// Checks for end record
		if ( line[0] == 'E')
			break;

		// Retrieves starting address from text record, converts to int
		addr = line.substr(3,4);
		loc = strtol(addr.c_str(), NULL, 16);

		// Iterates through object code bytes of text record
		for ( int i = 9; line[i] != '\0'; i = i+2 )
		{
  			sprintf(hex_loc, "%X", loc);
			// Loads hex location and one byte to file
  			output_file << hex_loc <<"\t"<<line[i]<<line[i+1]<<endl;
  			loc = loc + 1;
		}

	}
}

int main ()
{
	parseInput();
}
