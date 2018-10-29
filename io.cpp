#include "io.h"
#include <fstream>
#include <iostream>

using namespace std;

void readDataFile(string inputFileName, vector<string>& buffer)
{
    ifstream inputFile(inputFileName.c_str());
    if(!inputFile.is_open())
    {
        cout << "Couldn't open data file \""
            << inputFileName << "\"; exiting." << endl;
        exit(1);
    }

    // file opening successful; read file contents into buffer
    string line;

    while(getline(inputFile,line))
    {
        if(line.length()==0)
        {
            continue;
        }

        // ignore comments
        if(line[0]=='*')
        {
            continue;
        }

        buffer.push_back(line);
    }
}
