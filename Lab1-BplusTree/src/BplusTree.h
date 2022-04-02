#include <vector>
using namespace std;
#ifndef BplusTree_H
#define BplusTree_H

struct Node{
    // Atributos
    int *key;
    int size;
    bool isLeaf;

    // Relacion con otros Nodos
    Node** ptr;

    // Constructor
    Node();
};

class BplusTree{
    private:
    Node *root;
    int degree;       // orden del arbol, no se usa.
    void insertInternalNode(int, Node*, Node*);
    Node* getParent(Node*, Node*);

    public:
    BplusTree();
    Node* getRoot();
    void display(Node*);
    void search(int);
    void insertar(int);
    void borrar(int);
    

    ~BplusTree();
};



#endif