#ifndef __GA__
#define __GA__

#include <string>
#include <map>
#include <vector>

void   InitializeSolutions(int, std::vector<std::string>&, std::vector<std::vector<std::string> >&, std::map<std::string,int>& );
double ScoreSolution(std::vector<std::string>&, std::map<std::string, double>&, std::vector<int>&, std::vector<double>&, std::vector<double>&);
void   DetermineBestSolution(std::vector<double>&, double&, int&);
bool   TestBestSolution(std::vector<std::string>&, std::map<std::string, double>&, std::vector<int>&, std::vector<double>&, std::vector<double>&, std::vector<int>&, std::vector<int>&);
void   GenerateRouletteWheel(std::vector<double>&, std::vector<double>&);
int    SpinWheel(std::vector<double>&);
void   DetermineWorstSolution(std::vector<double>&, double&, int&);
void   CrossoverPMX(std::vector<std::vector<std::string> >&, std::vector<int>&, std::vector<std::string>&, std::vector<std::vector<std::string> >&, std::map<std::string, int>&, std::map<std::string, int>&) ;
void   Mutate(std::vector<std::vector<std::string> >&, std::vector<int>&, double, std::vector<std::string>&, std::map<std::string,double>&, std::map<std::string, int>&, std::map<std::string, int>&, std::map<std::string, int>&);
void   DetermineX2LeastFit(std::vector<int>&, std::vector<double>);

#endif
