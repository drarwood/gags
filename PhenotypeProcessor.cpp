// GAGS
// Developed by Andrew R Wood
// a.r.wood@exeter.ac.uk

#include "PhenotypeProcessor.h"
#include "StringOperations.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;

void ReadPhenotypeFileMinusExclusions(string f, vector<string>& i, vector<double>& v, map<string,double>& m, map<string,int>& e) {
    string line, id, pheno;
    ifstream inFile(f.c_str());
    if (inFile.is_open()) {
        getline (inFile, line); // skip header
        while (inFile.good()) {
            getline (inFile, line);
            if (!line.empty()) {
                istringstream iss;
                iss.str(line);
                iss >> id;
                iss >> pheno;
                if (pheno != "NA" && e.find(id) == e.end()) {
                    i.push_back(id);
                    v.push_back(stringToDouble(pheno));
                    m[id] = stringToDouble(pheno);
                }
            }
        }
        inFile.close();
    }
    else {
        cout << "Unable to open " << f << " for reading" << endl;
        exit(EXIT_FAILURE);
    }
}
