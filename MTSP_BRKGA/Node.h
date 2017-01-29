//
//  Node.h
//  MTSP_BRKGA
//
//  Created by Guilherme Coelho on 1/29/17.
//  Copyright © 2017 Guilherme Coelho. All rights reserved.
//

#ifndef Node_h
#define Node_h

#include <stdio.h>
#include <vector>

using namespace std;

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

#endif /* Node_h */
