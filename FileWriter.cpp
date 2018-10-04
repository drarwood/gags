// GAGS
// Developed by Andrew R Wood
// a.r.wood@exeter.ac.uk

#include "FileWriter.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

void OutputSolution(vector<string>& s, map<string,double>& m, vector<int>& n, string& o, int& g) {
    ofstream outFile;
    outFile.open(o.c_str(), ios_base::out);
    if (outFile.is_open()) {
        int index  = 0;
        //int group  = 0;
        int offset = 0;
        outFile << "Group\tID\tPhenotype\n";   
        for (int i = 0; i < n.size(); ++i) {
            //group++;
            while (index < n[i] + offset) {
                outFile << g << "\t" << s[index] << "\t" << m[s[index]] << endl;
                index++;
            }
            offset = offset + index;
            g++;
        }
        outFile.close();
    }
    else {
        cout << "Can not open " << o << " for writing" << endl;
        exit(EXIT_FAILURE);
    }
}

void OutputIndependentSolution(vector<string>& s, map<string,double>& m, vector<int>& n, string& o, int& g) {
    
    ofstream outFile;
    
    if (g == 1) {
        outFile.open(o.c_str(), ios_base::out);
        if (outFile.is_open()) {
            outFile << "Group\tID\tPhenotype\n";   
            for (int i = 0; i < n.size(); ++i) {
                for (int j = 0; j < n[i]; ++j) {
                    outFile << g << "\t" << s[j] << "\t" << m[s[j]] << endl;
                }
            }
            outFile.close();
        }
        else {
            cout << "Can not open " << o << " for writing" << endl;
            exit(EXIT_FAILURE);
        }
    }
    else {
        outFile.open(o.c_str(), ios_base::app);
        if (outFile.is_open()) {
            for (int i = 0; i < n.size(); ++i) {
                for (int j = 0; j < n[i]; ++j) {
                    outFile << g << "\t" << s[j] << "\t" << m[s[j]] << endl;
                }
            }
            outFile.close();
        }
        else {
            cout << "Can not open " << o << " for appending" << endl;
            exit(EXIT_FAILURE);
        }
    }
}

