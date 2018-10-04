// GAGS
// Developed by Andrew R Wood
// a.r.wood@exeter.ac.uk

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "StringOperations.h"
#include "InputParser.h"
#include "ExclusionListProcessor.h"
#include "PhenotypeProcessor.h"
#include "GA.h"
#include "FileWriter.h"
#include <stdlib.h>
#include <time.h>
#include <math.h> 
#include <algorithm>

using namespace std;

int main(int argc, char** argv) {

    // 1. Show header
    ShowHeader();
    
    // 2. Parse command line
    ARGS theseArgs;
    ParseCommand(argc, argv, &theseArgs);
    if(!theseArgs.ok) {
        ShowOptions();
        exit(EXIT_FAILURE);
    }
    cout << "Arguments: " << endl;
    cout << "        --pheno      -p " << theseArgs.phenoString << endl;
    cout << "        --ns         -n " << theseArgs.nsString << endl;
    cout << "        --means      -m " << theseArgs.meansString << endl;
    cout << "        --sd         -s " << theseArgs.sdsString << endl;
    cout << "        --seed       -r " << theseArgs.seed << endl;
    cout << "        --mrate      -y " << theseArgs.mRate << endl;
    cout << "        --popsize    -z " << theseArgs.popsize << endl;
    cout << "        --exclusions -e " << theseArgs.exclusionsString << endl;
    cout << "        --iterations -i " << theseArgs.iterations << endl;
    cout << "        --out        -o " << theseArgs.outString << endl << endl;    
    srand(theseArgs.seed);
    
    // Create vectors for list arguments
    vector<int> ns;
    SplitIntArgs(theseArgs.nsString, ns);
    vector<double> means;
    SplitDoubleArgs(theseArgs.meansString, means);
    vector<double> sds;
    SplitDoubleArgs(theseArgs.sdsString, sds);

    // Get required precision input by user
    vector<int> t1;
    DetermineDoubleArgsPrecision(theseArgs.meansString, t1);
    vector<int> t2;
    DetermineDoubleArgsPrecision(theseArgs.sdsString, t2);
    
    // read exclusions file    
    map<string,int> exclusions;
    if (theseArgs.exclusionsString.length() > 0) {
        ReadExclusionsFile(theseArgs.exclusionsString, exclusions);
    }

    // Load the phenotypes
    cout << "Reading phenotypes..." << endl;
    vector<string> idVector;
    vector<double> phenoVector;
    map<string,double> phenoMap;
    ReadPhenotypeFileMinusExclusions(theseArgs.phenoString, idVector, phenoVector, phenoMap, exclusions);    
        
    // Check number of individuals required to formulate the distribution is more than the number with pheneotype
    unsigned int total_n = 0;
    for (unsigned int i = 0; i<ns.size(); ++i) {
        total_n += ns[i];
    }
    //cout << total_n << endl;
    if (total_n > phenoVector.size()) {
        cout << "Unable to process this data as required sample sizes for distributions larger than N in phenotype file" << endl;
    }
    
    // At this point we want to order the Ns and adjust ordering of means and SDs in vecto
    vector<int>    nsSorted;  
    vector<double> meansSorted;
    vector<double> sdsSorted;
    vector<int>    meansPrecision;
    vector<int>    sdsPrecision;

    map<int,int>   jsUsed;

    nsSorted = ns;
    sort(nsSorted.begin(), nsSorted.end());
    for (unsigned int i = 0; i < nsSorted.size(); ++i) {
        for (unsigned int j = 0; j < ns.size(); ++j) {
            if (ns[j] == nsSorted[i] && jsUsed.find(j) == jsUsed.end()) {
                meansSorted.push_back(means[j]);
                sdsSorted.push_back(sds[j]);
                meansPrecision.push_back(t1[j]);
                sdsPrecision.push_back(t2[j]);
                jsUsed[j] = 0;
                break;
            }
        }
    }
    ns.clear();
    means.clear();
    sds.clear();
    
    // If the smallest N >= 10% of total_n then we carry on as usual
    bool solveIndependently;
    bool solveIndependentlyEverSet = false;
    int  independentGroupIndex = 0;
    int  smallestOffset = 0;
    int  nSubtract     = 0;
    int  n;
    
    
    do { 
        
        if ( ((double) nsSorted[smallestOffset] / (double) (total_n-nSubtract)) > 0.1 ) {
            solveIndependently = false;
            n = total_n-nSubtract;            
            for (unsigned int i = smallestOffset; i < nsSorted.size(); ++i) {
                ns.push_back(nsSorted[i]);
                means.push_back(meansSorted[i]);
                sds.push_back(sdsSorted[i]);
            }
        }
        // else we need to solve as seperate problem
        else {
            solveIndependently = true;
            solveIndependentlyEverSet = true;
            n = nsSorted[smallestOffset];
            ns.push_back(nsSorted[smallestOffset]);
            means.push_back(meansSorted[smallestOffset]);
            sds.push_back(sdsSorted[smallestOffset]);
            nSubtract += nsSorted[smallestOffset];
            smallestOffset++;
        }
 
        
        // 1. Initialise solutions
        cout << "Initialising solutions..." << endl;
        vector<vector<string> > population(theseArgs.popsize);
        cout << "Exclusion list size " << exclusions.size() << endl;
        InitializeSolutions(n, idVector, population, exclusions);
        
        // 2. Score solutions
        cout << "Scoring solutions..." << endl;
        vector<double> scores(theseArgs.popsize);
        //double thiscore;
        for (int i = 0; i < theseArgs.popsize; ++i) {
            scores[i] = ScoreSolution(population[i], phenoMap, ns, means, sds);
        }
        
        // 3. Determine the lowest (best) score  and solution with lowest score
        double lowestScore;
        int    bestSolutionIndex;
        DetermineBestSolution(scores, lowestScore, bestSolutionIndex);
        cout << "Best score and solution ID presently: " << lowestScore << " "  << bestSolutionIndex << endl;
        
        // 4. Determine if we have found a solution that is good enough by chance using the best one we have in the population
        bool solutionFound;
        solutionFound = TestBestSolution(population[bestSolutionIndex], phenoMap, ns, means, sds, meansPrecision, sdsPrecision);
        
        // 5. If solution found by chance then output it
        if (solutionFound) {
            cout << "Solution found" << endl;
            if (!solveIndependentlyEverSet) {
                independentGroupIndex++;
                OutputSolution(population[bestSolutionIndex], phenoMap, ns, theseArgs.outString, independentGroupIndex);
            }
            else {
                independentGroupIndex++;
                OutputIndependentSolution(population[bestSolutionIndex], phenoMap, ns, theseArgs.outString, independentGroupIndex);
                // add samples from last solution to exclusion list so we don't include them again
                for (vector<string>::iterator it = population[bestSolutionIndex].begin(); it != population[bestSolutionIndex].end(); ++it) {
                    exclusions[*it] = 0;
                }
            }
        }
        
        
        // 6. else we need tocyce up to max iterations to try and find solution
        else {
            
            // Determine highest (worst) score and solution with worst score
            bool   carryOn = true;
            double highestScore;
            int    worstSolutionIndex;
            DetermineWorstSolution(scores, highestScore, worstSolutionIndex);

            cout << "Iteration 1 ";
            
            // Iterate through for MAX iterations
            for (int i = 0; i < theseArgs.iterations; ++i) {
                
                int iter = (i+1)*100;
                if (100 % iter == 0) {
                    cout << iter << " ";
                }

                // [re]generate Roulette Wheel
                vector<double> rWheel;
                GenerateRouletteWheel(rWheel, scores);
                
                // Spin the wheel x2 to select solutions
                vector<int> randomIndicies;
                int ri1 = SpinWheel(rWheel);
                randomIndicies.push_back(ri1);
                int ri2;
                do {
                    ri2 = SpinWheel(rWheel);
                } while (ri1 == ri2);
                randomIndicies.push_back(ri2);
                
                // Crossover
                vector<vector<string> > chrXY(2);   
                map<string,int> chrXAlleles;
                map<string,int> chrYAlleles;
                CrossoverPMX(population, randomIndicies, idVector, chrXY, chrXAlleles, chrYAlleles);
                
                // Mutate
                Mutate(chrXY, ns, theseArgs.mRate, idVector, phenoMap, chrXAlleles, chrYAlleles, exclusions);
                
                // Identify the worst 2 scores in the population frist
                vector<int> worst2Solutions;
                DetermineX2LeastFit(worst2Solutions, scores);
                
                // 1.  First see if either provide us with a good enough solution //            
                for (int j = 0; j < 2; ++j) {
                    
                    // get score for current child
                    double thisScore = ScoreSolution(chrXY[j], phenoMap, ns, means, sds);
                    
                    // if this score lower (i.e. better) than lowest score in the population check whether solution good enough
                    if (thisScore < lowestScore) {
                    
                        lowestScore = thisScore;
                        
                        // check if we have found a solution
                        solutionFound = TestBestSolution(chrXY[j], phenoMap, ns, means, sds, meansPrecision, sdsPrecision);
                        
                        // if it is - print out and stop
                        if (solutionFound) {
                            
                            cout << thisScore << endl;
                            cout << "Solution found! " << endl;
                            
                            if (!solveIndependentlyEverSet) {
                                independentGroupIndex++;
                                OutputSolution(chrXY[j], phenoMap, ns, theseArgs.outString, independentGroupIndex);
                            }
                            else {
                                independentGroupIndex++;
                                OutputIndependentSolution(chrXY[j], phenoMap, ns, theseArgs.outString, independentGroupIndex);
                                // add samples from last solution to exclusion list so we don't include them again
                                for (vector<string>::iterator it = chrXY[j].begin(); it != chrXY[j].end(); ++it) {
                                    exclusions[*it] = 0;
                                }
                            }
                            
                            carryOn = false;
                            break;
                        }
                        else {
                            // place this solution in the population and update respective score
                            population[worst2Solutions[j]] = chrXY[j];
                            scores[worst2Solutions[j]] = thisScore;
                        }
                    }
                    else {
                        // place this solution in the population
                        population[worst2Solutions[j]] = chrXY[j];
                        scores[worst2Solutions[j]] = thisScore;
                    }
                }
                if (!carryOn) {
                    break;
                }
                // cout << lowestScore << endl;
                randomIndicies.clear();
            }  
        }
        
        // clear out the vectors
        ns.clear();
        means.clear();
        sds.clear();
        population.clear();
        scores.clear();
        
        if (!solutionFound) {
            cout << theseArgs.iterations << endl;
            cout << "Sorry - no solution not found this time." << endl;
            cout << "Error score: " << lowestScore << endl;
        }
                
    } while (solveIndependently);
    
    return 0;
}
