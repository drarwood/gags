# GAGS
## Genetic Algorithm for Group Selection
Written by Andrew Wood - a.r.wood@exeter.ac.uk - Genetics of Complex Traits - University of Exeter

### Purpose 
GAGS is a C++ program created to sample data with specific means and standard deviations from large datasets. The original purpose was to select phenotypic distributions from large scale studies, such as the UK Biobank. 

### Compiling 
GAGS can be compiled using the 'make' file provided 
```
make 
```
Installation can occur with root permissions or by manually changing the Makefile.
```
make install
```

#### Input Files

- Phenotype File (required). A tab delimited file containing ordered columns for ID and phenotypic values
```
ID    PHENO
A     178
B     170
C     167
D     169
...
```
- Exclusion File (optional): A list of IDs (without header) for which you do not want the associated phenotypic values to be used by the algorithm and included in the output.

### Runnings GAGS 
#### Options
A full list of options can be found by simply typing 'gags' at the command line:

```
gags  --pheno      -p  [phenotype file]
      --ns         -n  [list of group sizes]
      --means      -m  [list of respective means]
      --sds        -s  [list of respective SDs]
      --seed       -r  [seed - default 10000]
      --mrate      -y  [max mutation rate - default 0.01]
      --popsize    -z  [initial solutions - default 10]
      --exclusions -e  [optional: file of IDs to exclude]
      --iterations -i  [max iterations - default 50000]
      --out        -o  [output file prefix]
```

#### Running GAGS
To extract a subset of 1000 people from a phenotype file with mean = 169 and SD = 5:
```
gags --pheno example_data/ExamplePheno.txt --ns 1000 --means 90 --sds 15 --out MyPhenoSubset.txt
```

To extract a subset of 1000 people with mean = 169 and SD = 5 AND a subset of 500 people with mean = 160 and SD = 4:
```
gags --pheno example_data/ExamplePheno.txt --ns 1000,1000 --means 90,110 --sds 15,12 --out MyPhenoSubset.txt
```
To create multiple non-overlapping distributions then the order of N, mean and SD values need to be respective of each other on the command line.

To create a series of non-identical solutions ensure to inclue the --seed flag:
```
gags --pheno example_data/ExamplePheno.txt --ns 1000,1000 --means 90,110 --sds 15,12 --out MyPhenoSubset1.txt --seed 10
gags --pheno example_data/ExamplePheno.txt --ns 1000,1000 --means 90,110 --sds 15,12 --out MyPhenoSubset2.txt --seed 20
gags --pheno example_data/ExamplePheno.txt --ns 1000,1000 --means 90,110 --sds 15,12 --out MyPhenoSubset3.txt --seed 30
...
```

Note that the algorithm will aim to match the same precision as provided on the command line after rounding. 

#### Output File
The output file will contain the group ID individual ID and phenotypic value
```
Group ID    Phenotype
1     A     178
1     C     167
...
2     B     170
2     D     169
...
```
WARNING: if a solution is not found then the best solution is output once the maximum number of iterations has occured. You should check the program output as this will be flagged as a warning.

#### Fine-tuning the algorithm
Depending on the mean and SD required, it may be necessary to adjust the initial chromosome population size, maximum mutation rate and the maximum number of iterations if it becomes difficult to find a solution. The error for the best solution is output to the console (or log files for HPC). Therefore, you should run GAGS with several tunings of the parameters and compare the error to determine which seems to be the most effective in getting closer to the solution you want. Additional releases of this software will automate this. 

#### Asking for the impossible
The ability to find a desired set that meets the distribution requirements is dependent on how far away the mean and SD are from the full dataset. Be aware that the further away, the smaller the N as a proportion needs to be in order to maximise the chances of obtaining a solution. For example, if the phenotype mean = 200 and SD = 5 (respectively) in a full dataset  of 500,000, then it will be much harder to find a solution for a subset of 499,999 with mean=150 and SD=4. More guidance on this will be available here shortly.T

Note also, the current version is able to sample up to two non-overlapping groups simultaneously with specific means and SDs in an efficient manner when the total N across required groups = N. Creating additional groups simultaneously required the total N of the subsets to be great to be less than the sample size available in the phenotype file. More updates will be made to cater for this.

### Contact 
Please email me any questions or comments to a.r.wood@exeter.ac.uk

### Reference 
If you use GAGS, please cite: 

*Tyrrell et al. Gene-obesogenic environment interactions in the UK Biobank study, Int J Epidemiol. 2017 Apr 1;46(2):559-575*

