#ifndef __BUFFER__H
#define __BUFFER__H

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Buffer{
	private:

		bool validateAlgorithm(int optionAlgorithm);
		void printAlgorithmError();
		void printAlgorithm(int optionAlgorithm);
		void printRest(int dim, int seed);
	public:
		int getFromParameter( string pathOptionAlgorithm, string pathChosenSeed, string pathDim, int &optionAlgorithm, int &chosenSeed, int &dim);
};

#endif
