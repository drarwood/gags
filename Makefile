.PHONY: all
all: gags

gags: Main.cpp Calculator.cpp ExclusionListProcessor.cpp FileWriter.cpp GA.cpp InputParser.cpp PhenotypeProcessor.cpp StringOperations.cpp
	g++ Main.cpp Calculator.cpp ExclusionListProcessor.cpp FileWriter.cpp GA.cpp InputParser.cpp PhenotypeProcessor.cpp StringOperations.cpp -o gags -O2

# creating it if necessary
.PHONY: install
install:
	cp gags /usr/local/bin/

.PHONY: clean
clean:
	rm -f gags


