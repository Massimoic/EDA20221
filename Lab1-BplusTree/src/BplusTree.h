#ifndef BplusTree_H
#define BplusTree_H

#include <vector>
#include <iostream>
#include <queue>
#include <cmath>
using namespace std;

struct Node{
    // Atributos
    bool isLeaf = true;     // Es un nodo hoja
    vector<int> keys;       // Data
    vector<Node*> children; // Hijos
    Node* ptr = nullptr;    // Link a la siguiente hoja
    Node* parent = nullptr; // Nodo Padre

    // Constructores
    Node() = default;       // Valores por defecto
    Node(bool leaf){
        this->isLeaf = leaf;
    }
};

enum direction {
    LEFT,
    RIGHT
};

typedef struct {Node* node; int idx; direction direction;} keyDir;

class BplusTree{
    private:
    // Atributos
    Node* root;     // nodo raiz
    int deg;        // orden del arbol
    int capacity;   // maximo de keys en un nodo
    int minKeys;    // minimo de keys en un nodo
    
    // Funciones privadas (funciones de soporte)
    void split(Node*, int); // Separar nodo en 2
    bool mergeNodes(Node* left, Node* right); // Merge de nodos
    Node* newBrother(Node*, int); // Crear nuevo nodo hoja hermano
    keyDir findSibling(Node* node); // Localizar nodo hermano para prestar
    void deleteInternal(int key); // Eliminar nodo interno
    void arreglar(Node* node); // Funcion recursiva para arreglar nodos que no cumplan reglas

    // Funciones publicas
    public:
    BplusTree();
    ~BplusTree();

    void insertar(int);
    void borrar(int);
    vector<int> bfs();
};

#endif