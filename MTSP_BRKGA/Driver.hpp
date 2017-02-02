#include <random>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <vector>
#include <set>

#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"
#include "Node.h"

using namespace std;
using namespace std::chrono;

typedef vector< Node > vNode;
typedef signed char**  sCC;


// Variávies Globais

double bestSolValue = 0;
vector<int> bestSolVet;
vector<set<int> > toolsPerTask;

// Dados do Problema

sCC Matrix_Graph; // Matriz de Chars - Mais rapido

int sizeOfMagazine = 1;
int numberOfTools = 1; // Linha
int numberOfTasks = 1; // Colunas

// Grafo

vNode Graph;

/* Funcoes */

double ktns(std::vector<int> taksOrder);
int getToolNeededSoonest(std::vector<int> spareTools,
                         std::vector<int> missingTasks);
std::vector<int> decodeChromosome(std::vector< double >& chromosome);

/*
Imprime Grafo
*/

void imprimeGrafo(){
    vector <Aresta> te;
    cout << endl;
    for ( int i = 0; i < (int)Graph.size(); i++){
        te = Graph[i].getArestas();
        for(int j = 0; j <(int) te.size(); j++){
            cout << te[j].origem+1 << " --- " << te[j].peso << " ---> " << te[j].destino+1 << endl;
        }
        cout << endl;
    }
}

/*
 Reads the problem from a file specified by fileName
*/
void readProblem(string fileName)
{
    
    fileName = "Index/" + fileName;
    cout << "Read File: " << fileName << endl;

    FILE* file = fopen (fileName.c_str(), "r");                         //input file

    if (!file) {
        //erro durante leitura
        cout << "Erro na abertura do arquivo de entrada" << endl;
        exit(1);
    }
    
    fscanf(file,"%d",&numberOfTasks); // Num Ferrametnas - Coluna
    
    fscanf(file,"%d",&numberOfTools); // Num Ferrametnas - Linha

    fscanf(file,"%d",&sizeOfMagazine); // Tamanho Magazine
    
   // cout << "\nInformacoes Lidas\nLinhas-Ferramentas:  " << numberOfTools
        // << "\nColunas-Tarefa: " << numberOfTasks << "\nMag: " << sizeOfMagazine << endl;

    
    Matrix_Graph = (signed char **) malloc (numberOfTools * sizeof (signed char*));
    
    for ( int k = 0; k < numberOfTools; k++){
        Matrix_Graph[k] = (signed char *) malloc (numberOfTasks * sizeof(signed char));
    }
    
    int num, i=0, j=0;
    
    while (!feof (file)){
        
        fscanf (file, "%d", &num);
        
        if(i == numberOfTools)
            break;
        
        Matrix_Graph[i][j] = num;
        
        j++;
        
        // reseta coluna
        if (j == numberOfTasks){
            i++;
            j=0;
        }
    }
    
    set<int> auxSet;
    for (int k = 0; k < numberOfTasks; k++){
        for(int j = 0; j < numberOfTools; j++){
            if((int)Matrix_Graph[j][k] == 1){
                auxSet.insert(j);
            }
        }
        toolsPerTask.push_back(auxSet);
        auxSet.clear();
    }
    
//    set<int>::iterator iter;
//    for(int i = 0;i<toolsPerTask.size();i++){
//        for(iter=toolsPerTask[i].begin(); iter!=toolsPerTask[i].end();++iter){
//            cout<<(*iter)<<endl;
//        }
//        cout<<endl;
//    }
    

}

/*
 Builds the graph 
 */
void buildGraph()
{
    
    vector <Aresta> te;             //vector temporarios de aresta
    Aresta a;                       //aresta temporaria
    Node no;                        //vertice temporario
    int peso = 0;
    bool e = false;
    
    for(int i = 0; i < numberOfTools; i++){
        for (int k = 0; k < numberOfTools; k++){
            for(int j = 0; j < numberOfTasks; j++){
                
                if (k == i) // Pula diagonal
                    break;
                
                if(Matrix_Graph[i][j] >= 1 && Matrix_Graph[k][j] >= 1){
                    peso++;                     //conta o peso de uma aresta que liga dois vertices
                }
                if(Matrix_Graph[i][j] >= 1)
                    e = true; // existe
                
            }
            
            if(peso > 0){                       //se peso = 0 não existe aresta
                a.origem = i;                   //monta aresta
                a.peso = peso;
                a.destino = k;
                te.push_back(a);                //adiciona aresta no vetor de arestas
            }
            peso = 0;
        }
        
        no.addArestas(te);                      //adiciona todas as aresta de um vertice no vertice temporario
        no.setId(i);
        no.setExist(e);
        
        e = false;
        te.clear();
        
        Graph.push_back(no);                     //adiciona o vertice nó no grafo
    }
    
    //imprimeGrafo();
    
}
// Decoder

std::vector<int> decodeChromosome(std::vector< double > chromosome){
    
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
    for(std::vector< ValueKeyPair >::const_iterator i = rank.begin(); i != rank.end(); ++i) {
        permutation.push_back(i->second);
    }
    
    return permutation;
}

class Decoder {
public:
    double decode(const std::vector< double >& chromosome) const  {
        
        double myFitness = 0.0;
        
        std::vector< int > permutation = decodeChromosome(chromosome);
       
        myFitness = ktns(permutation);
        
        // Return the fitness:
        return myFitness;
    }
};

// KTNS

double ktns(std::vector<int> taksOrder){
    
    std::set<int> magazine;
    
    // Adiciona todas as ferramentas da tarefa 1 a caixa
    double fitness = toolsPerTask[taksOrder[0]].size();

    magazine = toolsPerTask[taksOrder[0]];
    
    for(int task = 1; task < numberOfTasks; task++){
        
        int emptySpaceInMag = sizeOfMagazine - (int)magazine.size();
        
        std::set<int> setTools = std::set<int>(toolsPerTask[taksOrder[task]].begin(),toolsPerTask[taksOrder[task]].end());
        
        //verifica se existe espaco vazio no magazine
        if(emptySpaceInMag >= setTools.size()){
            magazine.insert(setTools.begin(),setTools.end());
            fitness += setTools.size();

        }else{
            
            // E preciso fazer trocas
            
            std::vector<int>::iterator it;
            
            std::vector<int> diff(sizeOfMagazine,-1);
            std::vector<int> keepInMag(sizeOfMagazine,-1);
            std::vector<int> spareTools(sizeOfMagazine,-1);
            
            
            // The difference of two sets is formed by the elements that are present in the first set, but not in the second one.
            it = std::set_difference (setTools.begin(), setTools.end(),
                                      magazine.begin(), magazine.end(),
                                      diff.begin());
            
            diff.resize(it-diff.begin());
            
            it = std::set_difference (magazine.begin(), magazine.end(),
                                      setTools.begin(), setTools.end(),
                                      spareTools.begin());
            
            spareTools.resize(it-spareTools.begin());
            
            it = std::set_intersection (magazine.begin(), magazine.end(),
                                        setTools.begin(), setTools.end(),
                                        keepInMag.begin());
            
            keepInMag.resize(it-keepInMag.begin());
            
            
            if (diff.size() == 0){
                  // diferenca entre o mag atual e as proxima tarefa for 0..pula tarefa
                break;
            }else{
                fitness += (int)diff.size();
                
                // diff contem as ferramentas presentes na tarefa corrente e nao estao no magazine
                // e preciso fazer diff.size trocas no magazine
                
                magazine = std::set<int>(keepInMag.begin(),
                                         keepInMag.end());
                
                if( (int)diff.size() == (sizeOfMagazine - (int)keepInMag.size()) ){
                    // caso a diferenca seja igual ao tamanho do magazine..troca-se todas as pecas
                    magazine.insert(diff.begin(),
                                    diff.end());
                    
                }else{
                    // e preciso procurar qual ferramenta retirar do magazine
                    for(int numTroca = 0; numTroca < (int)diff.size(); numTroca++){
                        
                        magazine.erase(getToolNeededSoonest(spareTools,
                                                            std::vector<int>(taksOrder.begin() + task ,taksOrder.end())));
                    }
                    magazine.insert(diff.begin(),diff.end());
                }
            }
        }// Fim else
    }// Fim For
    
    return fitness;
}

int getToolNeededSoonest(std::vector<int> spareTools,
                         std::vector<int> missingTasks){
    
    typedef std::pair< int, int > Pair;
    std::vector< Pair > menosTrocas;
    
    for (int i =0; i < (int)spareTools.size(); i++) {
        int tool = spareTools[i];
        int posNeeded = 0;
        while( posNeeded < missingTasks.size() && toolsPerTask[missingTasks[posNeeded]].find(tool) == toolsPerTask[missingTasks[posNeeded]].end()){
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

// BRKGA

void setUpBRKGA(){
    
    cout<< "N: "<< numberOfTasks << endl << endl;
    
    const unsigned n = numberOfTasks;		// size of chromosomes
    const unsigned p = 100;		// size of population
    const double pe = 0.25;		// fraction of population to be the elite-set
    const double pm = 0.10;		// fraction of population to be replaced by mutants
    const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent
    const unsigned K = 3;		// number of independent populations
    const unsigned MAXT = 1;	// number of threads for parallel decoding

    
    const long unsigned rngSeed = 0;	// seed to the random number generator
    
    MTRand rng(rngSeed);				// initialize the random number generator
    
    Decoder decode;
        
    // initialize the BRKGA-based heuristic
    BRKGA< Decoder, MTRand > algorithm(n,p,
                                             pe,
                                             pm,
                                             rhoe,
                                             decode,
                                             rng,
                                             K,
                                             MAXT);
    
    unsigned generation = 0;		// current generation
    const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
    const unsigned X_NUMBER = 2;	// exchange top 2 best
    const unsigned MAX_GENS = 1000;	// run for 1000 gens
    
    std::cout << "Running for " << MAX_GENS << " generations..." << std::endl;
    
    do {
        algorithm.evolve();	// evolve the population for one generation
        
        if((++generation) % X_INTVL == 0) {
            algorithm.exchangeElite(X_NUMBER);	// exchange top individuals
        }
    } while (generation < MAX_GENS);
    
    bestSolValue = algorithm.getBestFitness();
    
    std::cout << "Best solution found has objective value = "
	 		<< algorithm.getBestFitness() << std::endl;
    
    std::cout << "Best solution found has this chormossome setup" << std::endl;
    
    bestSolVet = decodeChromosome(algorithm.getBestChromosome());
    
    for(int i = 0; i < numberOfTasks; i++) {
        std::cout << bestSolVet[i]+1 << " ";
    }
    std::cout<<std::endl;
    
}

/*
Terminates all data structures. 
*/
void termination()
{
    Graph.clear();

    for (int i = 0;  i < numberOfTools; i++) {
        Matrix_Graph[i] = NULL;
    }
    
    Matrix_Graph = NULL;
    
    sizeOfMagazine = 0;
    numberOfTools = 0; // Linha
    numberOfTasks = 0; // Colunas
    bestSolVet.clear();
    bestSolValue = NULL;
    
    toolsPerTask.clear();
    
}

/*
Prints the solution information to the output file
*/
void printSolution(string inputFileName, int solutionValue, double time, int run)
{
    string outputFileName =  "Results/Solution_" + inputFileName;

	ofstream fpSolution(outputFileName);				//file that contains the information about the solution of a problem instance
    
    fpSolution << "Instancia: " << inputFileName << std::endl;
    
    fpSolution << "Número de tarefas: " << numberOfTasks << std::endl;
    fpSolution << "Número de ferramentas: " << numberOfTools << std::endl;
    fpSolution << "Tamanho da caixa de ferramentas: " << sizeOfMagazine << std::endl;
    
    fpSolution << "Matriz de entrada:" << sizeOfMagazine << std::endl;
    for (int i = 0; i < numberOfTools; i++){
        for (int j = 0; j < numberOfTasks; j++){
            fpSolution << (int)Matrix_Graph[i][j] << " ";
        }
        fpSolution << endl;
    }
    
    fpSolution << "Solução para a execução:" << run << std::endl;
    fpSolution << "Tempo decorrido:" << time << std::endl;
    fpSolution << "Melhor valor encontrado para as trocas:" << bestSolValue << std::endl;
    fpSolution << "Configuração da ordem das tarefas a serem executadas:"<< std::endl;
    
    for (int i = 0; i < numberOfTasks; i++){
        fpSolution << bestSolVet[i]+1 << " ";
    }
    fpSolution << endl;

    fpSolution << "Configuração da matriz final:"<< std::endl;

    for (int i = 0; i < numberOfTools; i++){
        for (int j = 0; j < numberOfTasks; j++){
            int index = bestSolVet[j];
            fpSolution << (int)Matrix_Graph[i][index] << " ";
        }
        fpSolution << endl;
    }
    
}

/*
Main procedure - Multi-Run version
Parameters:
solutionValue			stores the solution value obtained by the method
runningTime				stores the exeution time of the method
inputFileName			stores the name of the file containing the instance
run 					stores the run #
*/

void multiRun(int *solutionValue, double *runningTime, string inputFileName, int run)
{
	int FinalSolutionValue = INT_MAX;

	readProblem(inputFileName);													//reads the problem data

	high_resolution_clock::time_point t1 = high_resolution_clock::now();		//time taking
	
   	buildGraph();																//builds the mosp graph
    
    setUpBRKGA();
    
	high_resolution_clock::time_point t2 = high_resolution_clock::now(); 		//time taking

    duration<double> time_span = duration_cast<duration<double> >(t2 - t1);

	*solutionValue = FinalSolutionValue;                             			//stores the solution value
	*runningTime =  time_span.count();											//stores the execution time

	printSolution(inputFileName, *solutionValue, *runningTime, run);			//prints the solution to the file

	termination();																//terminates all data structures
}
