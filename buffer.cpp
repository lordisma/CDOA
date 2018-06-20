#include "buffer.h"

bool Buffer::validateAlgorithm(int optionAlgorithm){
	return (optionAlgorithm == 1 || optionAlgorithm == 2 || optionAlgorithm == 3 || optionAlgorithm == 4 );
}

void Buffer::printAlgorithmError(){
	cout << "Error al escoger el algoritmo" << endl;
	cout << "1 -> Solis Wets" << endl;
	cout << "2 -> Simplex" << endl;
	cout << "3 -> CMAES" << endl;
	cout << "4 -> CMAES neighborhood" << endl;
}

void Buffer::printAlgorithm(int optionAlgorithm){
	switch(optionAlgorithm){
		case 1:{
			cout << "----------------------------------------------" << endl;
			cout << "Algoritmo: Solis Wets" << endl;
			break;
		}
		case 2:{
			cout << "----------------------------------------------" << endl;
			cout << "Algoritmo: Simplex" << endl;
			break;
		}
		case 3:{
			cout << "----------------------------------------------" << endl;
			cout << "Algoritmo: CMAES" << endl;
			break;
		}
		case 4:{
			cout << "----------------------------------------------" << endl;
			cout << "Algoritmo: CMAES neighborhood" << endl;
			break;
		}
	}
}


int Buffer::getFromParameter( string pathOptionAlgorithm, string pathChosenSeed, string pathDim, int &optionAlgorithm, int &chosenSeed, int &dim){

	string readOptionAlgorithm(pathOptionAlgorithm);
	string readChosenSeed(pathChosenSeed);

	optionAlgorithm = stoi(readOptionAlgorithm);
	if(!validateAlgorithm(optionAlgorithm)){
		printAlgorithmError();
		return -1;
	}
	optionAlgorithm = stoi(readOptionAlgorithm);
	if(!validateAlgorithm(optionAlgorithm)){
		printAlgorithmError();
		return -1;
	}
	int auxSeed = stoi(pathChosenSeed);
	if(auxSeed != 0){
		chosenSeed = auxSeed;
	}

	int auxDim = stoi(pathDim);
	if((auxDim == 10 || auxDim == 30 || auxDim == 50)){
		dim = auxDim;
	}

	printAlgorithm(optionAlgorithm);
	cout << "----------------------------------------------" << endl;
	cout << "Semilla Escogida: " << chosenSeed << endl;
	cout << "----------------------------------------------" << endl;
	cout << "Dimension: " << dim << endl;
	return 1;
}
