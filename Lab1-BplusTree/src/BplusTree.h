#ifndef BplusTree_H
#define BplusTree_H

#include <vector>
using namespace std;

struct Node{
    // Atributos
    bool isLeaf = true;
    vector<int> keys;       // Data
    vector<Node*> children; // Hijos
    Node* ptr = nullptr;    // Link a la siguiente hoja
    Node* parent = nullptr;

    // Constructores
    Node() = default;
    Node(bool leaf){
        this->isLeaf = leaf;
    }
};

class BplusTree{
    private:
    // Atributos
    Node* root = nullptr;
    int capacity = 20;
    
    // Funciones privadas
    Node* newBrother(Node*, int);
    void split(Node*, int);

    // Funciones publicas
    public:
    BplusTree() = default;
    ~BplusTree();

    void insertar(int);
    void borrar(int);
    vector<int> bfs();
};

#endif