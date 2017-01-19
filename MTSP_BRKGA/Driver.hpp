#include <random>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <vector>

#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"
#include "Decoder.h"

using namespace std;
using namespace std::chrono;

// Class Grafo

struct Aresta{
    int origem;
    int peso;
    int destino;
};

class Node{
    
public:
    
    Node()
    {
        degree = 0;
        visited = false;
        id = 0;
    }
    
    vector <Aresta> adjacencia;
    
    void addArestas(vector<Aresta> arestas){
        adjacencia = arestas;
    }
    
    vector<Aresta> getArestas(){
        return adjacencia;
    }
    
    void setDegree(int i)
    {
        degree = i;
    }
    
    void setVisited(bool b)
    {
        visited = b;
    }
    
    void setId(int i)
    {
        id = i;
    }
    
    int getDegree()
    {
        return degree;
    }
    
    bool wasVisited()
    {
        return visited;
    }
    
    int getId()
    {
        return id;
    }
    
    bool getExist(){
        return exist;
    }
    
    void setExist(bool b){
        exist = b;
    }
    
    void addAresta(int origem, int destino, int peso){
        Aresta nova;
        nova.origem = origem;
        nova.peso = peso;
        nova.destino = destino;
        adjacencia.push_back(nova);
    }
    
    int getArestaOrigem(int cont){
        return adjacencia[cont].origem;
    }
    
    int getArestadestino(int origem){
        return adjacencia[origem].destino;
    }
    int getArestapeso(int origem){
        return adjacencia[origem].peso;
    }
    
    int recuperaPeso(int origem, int destino){
        int peso;
        for(int i = 0; i < (int) adjacencia.size(); i++){
            if(adjacencia[i].origem == origem && adjacencia[i].destino == destino){
                peso = adjacencia[i].peso;
                return peso;
            }
            
        }
        return -1;
        
    }
    
    
private:
    int degree;  //stores the degree of the node
    int id;      //stores the index of the node
    bool visited; //indicates whether the node has been reached by the BFS
    bool exist;
};

typedef vector< Node > vNode;
typedef signed char**  sCC;


// Variávies Globais


// Dados do Problema

sCC Matrix_Graph; // Matriz de Chars - Mais rapido

int sizeOfMagazine = 1;
int numberOfTools = 1; // Linha
int numberOfTasks = 1; // Colunas

// Grafo

vNode Graph;

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
    fscanf(file,"%d",&numberOfTools); // Num Tarefas - Linha

    fscanf(file,"%d",&sizeOfMagazine); // Tamanho Magazine
    
    cout << "\nInformacoes Lidas\nLinhas-Tarefas:  " << numberOfTools << "\nColunas-Ferramentas: " << numberOfTasks << "\nMag: " << sizeOfMagazine << endl;

    
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
    
    for (i = 0; i < numberOfTools; i++){
        for (j = 0; j < numberOfTasks; j++){
            cout << (int)Matrix_Graph[i][j] << " ";
        }
        cout << endl;
    }
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
    
    imprimeGrafo();
    
}

void setUpBRKGA(){

    const unsigned n = numberOfTools;		// size of chromosomes
    const unsigned p = 100;		// size of population
    const double pe = 0.25;		// fraction of population to be the elite-set
    const double pm = 0.10;		// fraction of population to be replaced by mutants
    const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent
    const unsigned K = 3;		// number of independent populations
    const unsigned MAXT = 1;	// number of threads for parallel decoding
    
    Decoder decoder = Decoder(Matrix_Graph,
                              numberOfTools,
                              numberOfTasks);				// initialize the decoder
    
    const long unsigned rngSeed = 0;	// seed to the random number generator
    
    MTRand rng(rngSeed);				// initialize the random number generator
    
    // initialize the BRKGA-based heuristic
    BRKGA< Decoder, MTRand > algorithm(n,
                                             p,
                                             pe,
                                             pm,
                                             rhoe,
                                             decoder,
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
    
    // print the fitness of the top 10 individuals of each population:
    std::cout << "Fitness of the top 10 individuals of each population:" << std::endl;
    
    const unsigned bound = std::min(p, unsigned(10));	// makes sure we have 10 individuals
    
    for(unsigned i = 0; i < K; ++i) {
        std::cout << "Population #" << i << ":" << std::endl;
        for(unsigned j = 0; j < bound; ++j) {
            std::cout << "\t" << j << ") "
            << algorithm.getPopulation(i).getFitness(j) << std::endl;
        }
    }
    
    std::cout << "Best solution found has objective value = "
	 		<< algorithm.getBestFitness() << std::endl;

}

/*
Terminates all data structures. 
*/
void termination()
{
	//.clear in all data structures
	//=0 in all numeric variables
    Graph.clear();

    for (int i = 0;  i < numberOfTools; i++) {
        Matrix_Graph[i] = NULL;
    }
    
    Matrix_Graph = NULL;
    
    sizeOfMagazine = 0;
    numberOfTools = 0; // Linha
    numberOfTasks = 0; // Colunas
}

/*
Prints the solution information to the output file
*/
void printSolution(string inputFileName, int solutionValue, double time, int run)
{
    string outputFileName =  "Results/Solution_" + inputFileName;

	ofstream fpSolution(outputFileName);				//file that contains the information about the solution of a problem instance
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
