
#include "Decoder.h"
#include <iostream>

double ktns(std::vector<int>,std::vector< std::set<int> >);
void printToolsVet(std::vector<int>);
int getToolNeededLast(std::vector<int> ,std::vector<int>,std::vector< std::set<int> > );

Decoder::Decoder(signed char** mat,
                 int r,
                 int c,
                 int m,
                 std::vector< std::set<int> > *vetSet) {
    
    cols = c;
    rows = r;
    mag = m;
    
    this->toolsPerTask = *vetSet;
    
    memcpy(&matriz,&mat, sizeof(signed char**) * rows * cols);
    
//    std::cout << "Matriz Copiada" << std::endl;
//    
//    for (int i = 0; i < rows; i++){
//        for (int j = 0; j < coloumns; j++){
//            std::cout << (int)matriz[i][j] << " ";
//        }
//        std::cout << std::endl;
//    }
//    std::cout << std::endl;
    
    for(int i = 0; i < r; i++){
        std::cout << "Tarefa "<< i << " -> Ferramentas: ";
        for (auto it = toolsPerTask[i].begin(); it != toolsPerTask[i].end(); ++it){
            std::cout << " " << *it+1;
        }
        std::cout << std::endl;
    }

    std::cout << rows << " " << cols << std::endl;
}


Decoder::~Decoder() {
    
    for (int i = 0;  i < rows; i++) {
        matriz[i] = NULL;
    }
    
    for(int i = 0; i < rows; i++){
        toolsPerTask[i].clear();
    }
    
    toolsPerTask.clear();
    
    matriz = NULL;
    
    rows = 0;
    cols = 0;
}



double Decoder::decode(const std::vector< double >& chromosome) const  {
    double myFitness = 0.0;
    
        typedef std::pair< double, unsigned > ValueKeyPair;
        std::vector< ValueKeyPair > rank(chromosome.size());

        for(int i = 0; i < (int)chromosome.size(); i++) {
            rank[i] = ValueKeyPair(chromosome[i], i);
        }
        
        // Here we sort 'permutation', which will then produce a permutation of [n]
        // stored in ValueKeyPair::second:
        std::sort(rank.begin(), rank.end());
        
        // permutation[i].second is in {0, ..., n - 1}; a permutation can be obtained as follows
        std::vector< int > permutation;
    std::cout << "Cromossomo:"<<std::endl;
        for(std::vector< ValueKeyPair >::const_iterator i = rank.begin(); i != rank.end(); ++i) {
            std::cout << i->second << " ";
            permutation.push_back(i->second);
        }
        std::cout <<std::endl;
        myFitness = ktns(permutation,toolsPerTask);

    
	// Return the fitness:
	return myFitness;
}

double ktns(std::vector<int> taksOrder,
            std::vector< std::set<int> > toolsInTask){
    int magSize = 5; //ERRADO

    std::set<int> magazine;
    
    //TRANSFORMAR EM GLOBAL E REMOVER ESTA ATRIBUIÇÃO
    int numberOfTasks = (int)taksOrder.size();
    
    // Adiciona todas as ferramentas da tarefa 1 a caixa
    double fitness = toolsInTask[taksOrder[0]].size();
    magazine = toolsInTask[taksOrder[0]];
    
    for(int task = 1; task < numberOfTasks; task++){
        int emptySpaceInMag = magSize - (int)magazine.size();
        std::set<int> setTools = std::set<int>(toolsInTask[taksOrder[task]].begin(),toolsInTask[taksOrder[task]].end());

        //verifica se existe espaco vazio no magazine
        if(emptySpaceInMag >= setTools.size()){
            magazine.insert(setTools.begin(),setTools.end());
            fitness += setTools.size();
        
        }else{
            
            // E preciso fazer trocas
            //std::sort (magazine.begin(),magazine.end());
            //std::sort (setTools.begin(),setTools.end());
            
            std::vector<int>::iterator it;
            
            std::vector<int> diff(magSize,-1);
            std::vector<int> keepInMag(magSize,-1);
            std::vector<int> spareTools(magSize,-1);

            
            // The difference of two sets is formed by the elements that are present in the first set, but not in the second one.
            it = std::set_difference (setTools.begin(), setTools.end(),
                                    magazine.begin(), magazine.end(),
                                    spareTools.begin());
            spareTools.resize(it-spareTools.begin());
            
            it = std::set_difference (magazine.begin(), magazine.end(),
                                    setTools.begin(), setTools.end(),
                                    diff.begin());
            diff.resize(it-diff.begin());

            it = std::set_intersection (magazine.begin(), magazine.end(),
                                    setTools.begin(), setTools.end(),
                                    keepInMag.begin());
            
            keepInMag.resize(it-keepInMag.begin());

            
            // diff contem as ferramentas presentes na tarefa corrente e nao estao no magazine
            // e preciso fazer diff.size trocas no magazine
            fitness += (int)diff.size();
            
            magazine = std::set<int>(keepInMag.begin(),
                                     keepInMag.end());

            if( (int)diff.size() == (magSize - (int)keepInMag.size()) ){
                // caso a diferenca seja igual ao tamanho do magazine..troca-se todas as pecas
                magazine.insert(diff.begin(),
                                diff.end());
            
            }else{
                // e preciso procurar qual ferramenta retirar do magazine
                for(int numTroca = 0; numTroca < (int)diff.size(); numTroca++){
                    
                    magazine.erase(getToolNeededLast(spareTools,
                                                     std::vector<int>(taksOrder.begin() + task ,taksOrder.end()),
                                                     toolsInTask));
                }
                magazine.insert(diff.begin(),diff.end());
            }
            
        }// Fim else
    }// Fim For
    
    return fitness;
}

int getToolNeededLast(std::vector<int> spareTools,
                      std::vector<int> missingTasks,
                      std::vector< std::set<int> > toolsInTask){
        
    typedef std::pair< int, int > Pair;
    std::vector< Pair > menosTrocas;
    
    for (int i =0; i < (int)spareTools.size(); i++) {
        int tool = spareTools[i];
        int posNeeded = 0;
        while( toolsInTask[missingTasks[posNeeded]].find(tool) == toolsInTask[missingTasks[posNeeded]].end()){
            posNeeded++;
        }
        menosTrocas.push_back(Pair(posNeeded,tool));
    }
    
    std::sort(menosTrocas.begin(),
              menosTrocas.end());


    return menosTrocas[0].second;
}


void printToolsVet(std::vector<int> vet){
    
    for (int i = 0; i < vet.size(); i++) {
        std::cout << vet[i] << " ";
    }
    std::cout<<std::endl;
    
}

