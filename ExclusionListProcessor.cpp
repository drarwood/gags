// GAGS
// Developed by Andrew R Wood
// a.r.wood@exeter.ac.uk

#include "ExclusionListProcessor.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

void ReadExclusionsFile(string f, map<string, int>& m) {
    string line, id;
    ifstream inFile(f.c_str());
    if (inFile.is_open()) {
        while (inFile.good()) {
            getline (inFile, line);
            if (!line.empty()) {
                m[line] = 0;
            }
        }
        inFile.close();
    }
    else {
        cout << "Unable to open " << f << " for reading" << endl;
        exit(EXIT_FAILURE);
    }
}
