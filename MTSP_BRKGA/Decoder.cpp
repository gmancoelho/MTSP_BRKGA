
#include "Decoder.h"
#include <iostream>

Decoder::Decoder(signed char** mat,
                 int rows,
                 int coloumns) {
    
    memcpy(matriz, mat, sizeof(signed char**) * rows * coloumns);
    
    std::cout << "Matriz Decoder" << std::endl;
    
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < coloumns; j++){
            std::cout << (int)matriz[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

Decoder::~Decoder() { }

double Decoder::decode(const std::vector< double >& chromosome) const  {
    
	double myFitness = 0.0;
    
    for (int i = 0; i < chromosome.size(); i++) {
        std::cout<< chromosome[i]<<" ";
    }
    std::cout<<std::endl;
	// Return the fitness:
	return myFitness;
}
