// GAGS
// Developed by Andrew R Wood
// a.r.wood@exeter.ac.uk

#include "GA.h"
#include "StringOperations.h"
#include "Calculator.h"
#include <cstdlib>
#include <stdlib.h> 
#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>

using namespace std;

void InitializeSolutions(int n, vector<string>& s, vector<vector<string> >& p, map<string,int>& e) {
    // solutions will be a hash containing arrays of subject IDs. 
    // We will score these later by referencing ID to phenotype value
    // Note that N in each solution will be the total N across the groups in which to find specific distributions for. 
    // We will break each solution up into the relevant subsets for scoring and returning. 
    map<string, int> idsSelected;
    int randomIndex;
    for (int i = 0; i < p.size(); ++i) {
        for (int j = 0; j < n; ++j) {
            do {
                randomIndex = rand() % s.size(); // 0 to vector-size -1
            } while (!((idsSelected.find(s[randomIndex]) == idsSelected.end()) && (e.find(s[randomIndex]) == e.end())));
            p[i].push_back(s[randomIndex]);
            idsSelected[s[randomIndex]] = 0;
        }        
        idsSelected.clear();
    }     
}


double ScoreSolution(vector<string>& c, map<string, double>& p, vector<int>& n, vector<double>& m, vector<double>& s) {
    vector<double> phenotype;
    int index = 0;
    double score = 0;
    for (int i = 0; i < n.size(); ++i) {
        int count = 0;
        while (count < n[i]) {
            phenotype.push_back(p[c[index]]);
            index++;
            count++;
        }
        double mean = CalcMean(phenotype);
        double sd   = sqrt(CalcVariance(phenotype));
        if (fabs(m[i] - mean) >= 0) {
            score += fabs(m[i] - mean) * 2;
        }
        if (fabs(s[i] - sd) >= 0) {
            score += fabs(s[i] - sd);
        }
        phenotype.clear();
    }
    return score;
}


void DetermineBestSolution(std::vector<double>& v, double& s, int& x) {
    double bestScore = 999999999;
    int    bestScoreIndex;
    for (int i = 0; i < v.size(); ++i) {
        if (v[i] < bestScore) {
            bestScore = v[i];
            bestScoreIndex = i;
        }
    }
    s = bestScore;
    x = bestScoreIndex;
}


bool TestBestSolution(vector<string>& c, map<string, double>& p, vector<int>& n, vector<double>& m, vector<double>& s, vector<int>& mp, vector<int>& sp) {
    vector<double> values;
    int index = 0;
    bool solutionFound;
    for (int i = 0; i < n.size(); ++i) {
        int count = 0;
        while (count < n[i]) {
            values.push_back(p[c[index]]);
            index++;
            count++;
        }
        
        double mean = roundf(CalcMean(values) * pow(10,mp[i])) / pow(10,mp[i]);
        double sd   = roundf(sqrt(CalcVariance(values)) * pow(10,sp[i])) / pow(10,sp[i]);
        if (doubleEqual(mean, m[i], 0.00001) && doubleEqual(sd, s[i], 0.00001)) {
            solutionFound = true;
        }
        else {
            solutionFound = false;
            break;
        }
        values.clear();
    }
    return solutionFound;
}


void GenerateRouletteWheel(vector<double>& r, vector<double>& s) {
    vector<double>s2;
    vector<double>::iterator it = max_element(s.begin(), s.end());
    double worstScore = *it;
    double reqSum = 0;
    double prevProb = 0;    
    for (int i = 0; i < s.size(); ++i) {
        s2.push_back(worstScore-s[i]+1);
        reqSum += (worstScore-s[i]+1);
    }
    for (int i = 0; i < s2.size(); ++i) {
        r.push_back(prevProb + (s2[i] / reqSum));
        prevProb += s2[i] / reqSum;
    }
}


int SpinWheel(vector<double>& r) {
    int i;
    double randomNumber = rand() / (double)RAND_MAX;
    for (i = 0; i < r.size(); ++i) {
        if (randomNumber < r[i]) {
            break;
        }
    }
    return i;
}


void DetermineWorstSolution(vector<double>& v, double& s, int& x) {
    double worstScore = 0;
    int    worstScoreIndex;
    for (int i = 0; i < v.size(); ++i) {
        if (v[i] > worstScore) {
            worstScore = v[i];
            worstScoreIndex = i;
        }
    }
    s = worstScore;
    x = worstScoreIndex;
}


void CrossoverPMX(vector<vector<string> >& p, vector<int>& r, vector<string>& s, vector<vector<string> >& chrXY, map<string, int>& chrXAlleles, map<string, int>& chrYAlleles) {
    
    // define crossover points
    int xoverA = rand() % p[r[0]].size();
    int xoverB = rand() % p[r[0]].size();
    if (xoverA > xoverB) {
        int t  = xoverA;
        xoverA = xoverB;
        xoverB = t;
    }
    
    // Record elements in crossover region in maps: id -> index and map IDs across chromosomes within crossover region
    map<string,string> chrXxOverAlleleMap;
    map<string,string> chrYxOverAlleleMap;
    map<string,string>::iterator it;
    
    for (int i = xoverA; i <= xoverB; ++i) {    
        chrXxOverAlleleMap[p[r[1]][i]] = p[r[0]][i];
        chrYxOverAlleleMap[p[r[0]][i]] = p[r[1]][i];
    }
 
    // Perform the PMX crossover        
    for (int i = 0; i < p[r[0]].size(); ++i) {
        
        if (i < xoverA || i > xoverB) {
            
            // if element not in chromosomal crossover region associated with orginal chr Y
            it = chrXxOverAlleleMap.find(p[r[0]][i]);
            if (it == chrXxOverAlleleMap.end()) {
                chrXY[0].push_back(p[r[0]][i]);
                chrXAlleles[p[r[0]][i]] = 0;
            }
            else {
                string newAllele = chrXxOverAlleleMap[p[r[0]][i]];
                it = chrXxOverAlleleMap.find(newAllele);
                while (it != chrXxOverAlleleMap.end()) {
                    newAllele = chrXxOverAlleleMap[newAllele];
                    it = chrXxOverAlleleMap.find(newAllele);
                } 
                chrXY[0].push_back(newAllele);
                chrXAlleles[newAllele] = 0;
            }
            
            // if element not in chromosomal crossover region associated with orginal chr X
            it = chrYxOverAlleleMap.find(p[r[1]][i]);
            if (it == chrYxOverAlleleMap.end()) {
                chrXY[1].push_back(p[r[1]][i]);
                chrYAlleles[p[r[1]][i]] = 0;
            }
            else {
                string newAllele = chrYxOverAlleleMap[p[r[1]][i]];
                it = chrYxOverAlleleMap.find(newAllele);
                while (it != chrYxOverAlleleMap.end()) {
                    newAllele = chrYxOverAlleleMap[newAllele];
                    it = chrYxOverAlleleMap.find(newAllele);
                } 
                chrXY[1].push_back(newAllele);
                chrYAlleles[newAllele] = 0;
            }            
        }
        else { 
            //CROSSOVER REGION
            chrXY[0].push_back(p[r[1]][i]);
            chrXY[1].push_back(p[r[0]][i]);
            chrXAlleles[p[r[1]][i]] = 0;
            chrYAlleles[p[r[0]][i]] = 0;
        }
    }
}



void Mutate(vector<vector<string> >& chrXY, vector<int>& n, double m, vector<string>& s, map<string,double>& p, map<string, int>& chrXAlleles, map<string, int>& chrYAlleles, map<string, int>& e) {
    
    double min = 1/(double)chrXY[0].size();    
    if (min <= m) {
        
        int                      mutationIndex1;
        string                   mutationIndex1Allele;        
        int                      mutationIndex2;
        string                   mutationIndex2Allele;
        string                   newAllele;
        map<string,int>::iterator it;

        double random = min + ((double)rand() / RAND_MAX) * (m - min);        
        int mutations = round((double)chrXY[0].size() * random);
        
        // for each mutation to be performed // mutate each chromosome in turn
        for (int j = 0; j < mutations; ++j) {
            
            // CHROMOSOME X (0)
            // pick indicies at random in which alleles are to be swapper
            mutationIndex1       = rand() % chrXY[0].size();
            mutationIndex1Allele = chrXY[0][mutationIndex1];
            // pick and allele at random to place into this from the full set of IDs. This will help get round local maxima
            do {
                mutationIndex2       = rand() % s.size();
                mutationIndex2Allele = s[mutationIndex2];
            } while (e.find(mutationIndex2Allele) != e.end());

            // Determine if this is a new allele in the chromosome or whether this will need to be a swap if in chromosome
            it = chrXAlleles.find(mutationIndex2Allele);
            // if not found then insert new allele into preselected  mutation site - this should be the default first check
            if (it == chrXAlleles.end()) {
                chrXY[0][mutationIndex1] = mutationIndex2Allele;
                chrXAlleles.erase(mutationIndex1Allele);
                chrXAlleles[mutationIndex2Allele] = 0;
                
            }
            // else mutate by performing simple swap but dont put contraint on index range to chose from as below
            else {
                mutationIndex2       = rand() % chrXY[0].size();
                mutationIndex2Allele = chrXY[0][mutationIndex2];
                chrXY[0][mutationIndex1] = mutationIndex2Allele;
                chrXY[0][mutationIndex2] = mutationIndex1Allele;                
            }
            
            // CHROMOSOME Y (1)
            // pick indicies at random in which alleles are to be swapper
            mutationIndex1       = rand() % chrXY[1].size();
            mutationIndex1Allele = chrXY[1][mutationIndex1];
            // pick and allele at random to place into this from the full set of IDs. This will help get round local maxima
            do {
                mutationIndex2       = rand() % s.size();
                mutationIndex2Allele = s[mutationIndex2];
            } while (e.find(mutationIndex2Allele) != e.end());
            
            // Determine if this is a new allele in the chromosome or whether this will need to be a swap if in chromosome
            it = chrYAlleles.find(mutationIndex2Allele);
            // if not found then insert new allele into preselected  mutation site
            if (it == chrYAlleles.end()) {
                chrXY[1][mutationIndex1] = mutationIndex2Allele;
                chrYAlleles.erase(mutationIndex1Allele);
                chrYAlleles[mutationIndex2Allele] = 0;
                
            }
            // else mutate similar to below performing simple swap but dont put contraint on index range to chose from as below
            else {
                mutationIndex2       = rand() % chrXY[1].size();
                mutationIndex2Allele = chrXY[1][mutationIndex2];
                chrXY[1][mutationIndex1] = mutationIndex2Allele;
                chrXY[1][mutationIndex2] = mutationIndex1Allele;                
            }
        }
    }
    else {
        cout << "Cannot mutate at user rate - not enough samples in chromosome" << endl;
    }
}


void DetermineX2LeastFit(vector<int>& c, std::vector<double> s) {
    int index;
    vector<double>::iterator it;
    it  = max_element(s.begin(), s.end());    
    it = find(s.begin(), s.end(), *it);
    index = distance(s.begin(), it);
    c.push_back(index);
    s[index] = -1;
    it = max_element(s.begin(), s.end());    
    it = find(s.begin(), s.end(), *it);
    index = distance(s.begin(), it);
    c.push_back(index);
}










