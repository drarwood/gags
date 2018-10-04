// GAGS
// Developed by Andrew R Wood
// a.r.wood@exeter.ac.uk

#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "InputParser.h"
#include "StringOperations.h"


using namespace std;

void ShowHeader() {
    cout << endl << "==================== GAGS ===============" << endl;
    cout <<         "    Genetic Algorithm Group Selection  " << endl;
    cout <<         "       Written by Andrew R Wood        " << endl;
    cout <<         "           a.r.wood@exeter.ac.uk       " << endl;
    cout <<         "  University of Exeter Medical School  " << endl;
    cout <<         "          Version 0.1: 20-09-2018      " << endl;
    cout <<         "=======================================" << endl;
    cout << endl;
}

void ShowOptions() {
    cout << "Usage:" << endl;
    cout << "      --pheno      -p  [phenotype file]"                    << endl;
    cout << "      --ns         -n  [list of group sizes]"               << endl;
    cout << "      --means      -m  [list of respective means]"          << endl;
    cout << "      --sds        -s  [list of respective SDs]"            << endl;
    cout << "      --seed       -r  [seed - default 10000]"              << endl;
    cout << "      --mrate      -y  [max mutation rate - default 0.01]"  << endl;
    cout << "      --popsize    -z  [initial solutions - default 10]"    << endl;
    cout << "      --exclusions -e  [optional: file of IDs to exclude]" << endl;
    cout << "      --iterations -i  [max iterations - default 50000]"    << endl;
    cout << "      --out        -o  [output file prefix]"                << endl;
    cout << endl;
}

void ParseCommand(int argc, char** argv, ARGS *a) {
    
    if (argc > 1) {
        
        // defaults
        a->seed       = 10000;
        a->popsize    = 10;
        a->mRate      = 0.01;
        a->iterations = 50000;
        a->ok         = true;
        
        for (int i = 1; i != argc; ++i) {
            if (string(argv[i]) == "--pheno" || string(argv[i]) == "-p") {
                if (i+1 <= argc-1 && string(argv[i+1]).substr(0,2) != "--") {
                    a->phenoString = argv[i+1];
                }
                else {
                    cout << "Not entered --pheno (-p) argument" << endl << endl;
                    a->ok = false;
                }
            }
            else if (string(argv[i]) == "--ns" || string(argv[i]) == "-n") {
                if (i+1 <= argc-1 && string(argv[i+1]).substr(0,2) != "--") {
                    a->nsString = argv[i+1];
                }
                else {
                    cout << "Not entered --ns (-n) argument" << endl << endl;
                    a->ok = false;
                }
            }
            else if (string(argv[i]) == "--means" || string(argv[i]) == "-m") {
                if (i+1 <= argc-1 && string(argv[i+1]).substr(0,2) != "--") {
                    a->meansString = argv[i+1];
                }
                else {
                    cout << "Not entered --means (-m) argument" << endl << endl;
                    a->ok = false;
                }
            }
            else if (string(argv[i]) == "--sds" || string(argv[i]) == "-s") {
                if (i+1 <= argc-1 && string(argv[i+1]).substr(0,2) != "--") {
                    a->sdsString = argv[i+1];
                }
                else {
                    cout << "Not entered --sds (-s) argument" << endl << endl;
                    a->ok = false;
                }
            }
            else if (string(argv[i]) == "--seed" || string(argv[i]) == "-r") {
                if (i+1 <= argc-1 && string(argv[i+1]).substr(0,2) != "--") {
                    a->seed = atoi(argv[i+1]);
                }
                else {
                    cout << "Not entered --seed (-r) argument" << endl << endl;
                    a->ok = false;
                }
            }
            else if (string(argv[i]) == "--mrate" || string(argv[i]) == "-y") {
                if (i+1 <= argc-1 && string(argv[i+1]).substr(0,2) != "--") {
                    a->mRate = atof(argv[i+1]);
                }
                else {
                    cout << "Not entered --mrate (-y) argument" << endl << endl;
                    a->ok = false;
                }
            }
            else if (string(argv[i]) == "--popsize" || string(argv[i]) == "-z") {
                if (i+1 <= argc-1 && string(argv[i+1]).substr(0,2) != "--") {
                    a->popsize = atoi(argv[i+1]);
                }
                else {
                    cout << "Not entered --popsize (-z) argument" << endl << endl;
                    a->ok = false;
                }
            }
            else if (string(argv[i]) == "--exclusions" || string(argv[i]) == "-e") {
                if (i+1 <= argc-1 && string(argv[i+1]).substr(0,2) != "--") {
                    a->exclusionsString = argv[i+1];
                }
                else {
                    cout << "Not entered --exclusions (-e) argument" << endl << endl;
                    a->ok = false;
                }
            } 
            else if (string(argv[i]) == "--iterations" || string(argv[i]) == "-i") {
                if (i+1 <= argc-1 && string(argv[i+1]).substr(0,2) != "--") {
                    a->iterations = atoi(argv[i+1]);
                }
                else {
                    cout << "Not entered --iterations (-i) argument" << endl << endl;
                    a->ok = false;
                }
            } 
            else if (string(argv[i]) == "--out" || string(argv[i]) == "-o") {
                if (i+1 <= argc-1 && string(argv[i+1]).substr(0,2) != "--") {
                    a->outString = argv[i+1];
                }
                else {
                    cout << "Not entered --out (-o) argument" << endl << endl;
                    a->ok = false;
                }
            }
            else if (string(argv[i]) == "--help" || string(argv[i]) == "-h") {
                a->ok = false;
            }
        }
        
        if (a->phenoString.empty() || a->meansString.empty() || a->sdsString.empty() || a->outString.empty() ) {
            a->ok = false;
        }

    }
    else {
        a->ok = false;
    }

}


void SplitStringArgs(string& s, vector<string>& v) {
    split(s, ",", v);
}


void SplitDoubleArgs(string& s, vector<double>& v) {
    vector<string> vec;
    split(s, ",", vec);
    for (int i = 0; i < vec.size(); ++i) {
        v.push_back(stringToDouble(vec[i]));
    }
}


void SplitIntArgs(string& s, vector<int>& v) {
    vector<string> vec;
    split(s, ",", vec);
    for (int i = 0; i < vec.size(); ++i) {
        v.push_back(stringToInt(vec[i]));
    }
}


void DetermineDoubleArgsPrecision(string& s, vector<int>& v) {
    vector<string> vec;
    split(s, ",", vec);
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i].find(".") != -1) {
            v.push_back(vec[i].size() - (vec[i].find(".")+1));
        }
        else {
            v.push_back(0);
        }
    }
}


