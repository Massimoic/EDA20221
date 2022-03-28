#include <vector>
using namespace std;
#ifndef RBTREE_H
#define RBTREE_H

// Lista de colores del arbol
enum Color {RED,BLACK};

struct Nodo{
    // Atributos
    int dato;
    int color;

    // Relacion con otros Nodos
    Nodo *left;
    Nodo *right;
    Nodo *father;

    // Constructor
    Nodo(int);
};

class RBtree
{
private:
    Nodo *root;
    Nodo *insertarNodo(Nodo* &, Nodo* &);
    void corregirArbol(Nodo* &);

    int getColor(Nodo* &);
    void setColor(Nodo* &, int );

    void rotarIzquierda(Nodo* &);
    void rotarDerecha  (Nodo* &);

    vector<int> traversal;
    void swap(Nodo* nodo1, Nodo* nodo2);

    void corregirEliminado(Nodo* nodo);

public:
    RBtree();
    void insertar(int);
    vector<int> inorden();
    vector<int> preorden();
    vector<int> posorden();
    void inordenHelper(Nodo* nodo);
    void preordenHelper(Nodo* nodo);
    void posordenHelper(Nodo* nodo);
    Nodo* search(int);
    Nodo* getRoot();
    bool eliminarNodo(int);
    Nodo* minimum(Nodo* nodo);
    Nodo* maximum(Nodo* nodo);

    ~RBtree();
};






#endif