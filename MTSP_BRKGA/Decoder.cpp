
#include "Decoder.h"
#include <iostream>

Decoder::Decoder(signed char** mat,
                 int rows,
                 int coloumns) {
    
    this->cols = coloumns;
    this->rows = rows;
    
    memcpy(matriz, mat, sizeof(signed char**) * rows * coloumns);
    
    std::cout << "Matriz Decoder" << std::endl;
    
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < coloumns; j++){
            std::cout << (int)matriz[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

}

Decoder::~Decoder() {
    
    for (int i = 0;  i < rows; i++) {
        matriz[i] = NULL;
    }
    
    matriz = NULL;
    
    rows = 0;
    cols = 0;
}

void printToolsVet(std::vector<int> vet){
    
    for (int i = 0; i < vet.size(); i++) {
        std::cout << vet[i] << " ";
    }
    std::cout<<std::endl;
    
}

std::vector<int> setUpToolsVet(std::vector<double> cromossomos){
    
    std::vector<int> vet;

    // iterador para conseguir o maior elemento do vetor de cromossomo
    std::vector<double>::iterator it = max_element(cromossomos.begin(),cromossomos.end());
    
    double maior = (double) *it;
    
    // preenche o vetor de ferramentas sempre com o maior elemento do vetor de cromossomo
    while ( maior != -1) {
        int pos = (int) distance(cromossomos.begin(),it);
        vet.push_back(pos);
        cromossomos[pos] = -1;
        it = max_element(cromossomos.begin(),cromossomos.end());
        maior = (double) *it;
    }

    return vet;
}

double Decoder::decode( std::vector< double >& chromosome) const  {
    
	double myFitness = 0.0;
    
    if ( !chromosome.empty() ) {
        
        // Vetor de Ferramentas
        std::vector<int> toolsVet = setUpToolsVet(chromosome);
        
        //printToolsVet(toolsVet);

    }
    
	// Return the fitness:
	return myFitness;
}


