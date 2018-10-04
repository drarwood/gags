#ifndef __INPUTPARSER__
#define __INPUTPARSER__

struct ARGS
{
    std::string phenoString;
    std::string nsString;
    std::string meansString;
    std::string sdsString;
    std::string exclusionsString;
    std::string outString;
    int seed;
    int popsize;
    int iterations;
    double mRate;
    bool ok;
};

void ShowHeader();
void ShowOptions();
void ParseCommand(int, char**, ARGS*);
void SplitStringArgs(std::string&, std::vector<std::string>&);
void SplitDoubleArgs(std::string&, std::vector<double>&);
void SplitIntArgs(std::string&, std::vector<int>&);
void DetermineDoubleArgsPrecision(std::string&, std::vector<int>&);

#endif
