#include <algorithm> 
#include <iostream>
#include "rbtree.h"
using namespace std;

Nodo::Nodo(int dato){
    this->dato = dato;
    color = RED;
    
    left   = nullptr;
    right  = nullptr;
    father = nullptr;
}




int RBtree::getColor(Nodo* &puntero){
    if (puntero == nullptr) return BLACK;
    return puntero->color;
}

void RBtree::setColor(Nodo* &puntero, int color){
    if (puntero == nullptr) return;
    puntero->color = color;
}

/*          
 *        X                            Y
 *       / \     Rotar izquierda      / \
 *      A   Y    --------------->    X   C
 *         / \                      / \
 *        B   C                    A   B
 * 
 * La función se aplica a X
 */
void RBtree::rotarIzquierda(Nodo* &puntero){
    // Nodo Y
    Nodo *hijoDerecho = puntero->right;
    
    
    puntero->right = hijoDerecho->left;         // Asignar como hijo derecho de X a B
    if (puntero->right != nullptr)
        puntero->right->father = puntero;       // Asignar como nuevo padre de B a X


    hijoDerecho->father = puntero->father;      // Asignar que el nuevo padre de Y sea el padre de X
                                                // Esto es para conectar con el resto del arbol
    // Asignar la Y como hijo derecho o izquierdo 
    // En el paso anterior contectamos a Y con el resto del arbol, ahora Y será asignado como hijo del nodo superior
    if (puntero->father == nullptr)
        root = hijoDerecho;                     // Si no hay padre, significa que llegamos a la raiz
    else if (puntero == puntero->father->left)  
        puntero->father->left = hijoDerecho;    // Antes de la rotacion, X era el hijo izquiero
    else
        puntero->father->right = hijoDerecho;   // Antes de la rotacion, X era el hijo derecho

    hijoDerecho->left = puntero;                // Asignar a X como hijo izquierdo de Y
    puntero->father = hijoDerecho;              // Asignar a Y como padre de X
}

/*         
 *        Y                        X
 *       / \     Rotar derecha    / \
 *      X   C    ------------->  A   Y
 *     / \                          / \
 *    A   B                        B   C
 * 
 * La función se aplica a Y
 */
void RBtree::rotarDerecha(Nodo* &puntero){
    // Nodo X
    Nodo *hijoIzquierdo = puntero->left;        


    puntero->left = hijoIzquierdo->right;       // Asignar como hijo izquierdo de Y a B
    if (puntero->left != nullptr)
        puntero->left->father = puntero;        // Asignar como nuevo padre de B a Y


    hijoIzquierdo->father = puntero->father;    // Asignar que el nuevo padre de X sea el padre Y
                                                // Esto es para conectar con el resto del arbol
    // Asignar la X como hijo derecho o izquierdo 
    // En el paso anterior contectamos a X con el resto del arbol, ahora X será asignado como hijo del nodo superior
    if (puntero->father == nullptr)
        root = hijoIzquierdo;                   // Si no hay padre, significa que llegamos a la raiz
    else if (puntero == puntero->father->left)
        puntero->father->left = hijoIzquierdo;  // Antes de la rotacion, Y era el hijo izquiero
    else
        puntero->father->right = hijoIzquierdo;  // Antes de la rotacion, Y era el hijo derecho


    hijoIzquierdo->right = puntero;              // Asignar a Y como hijo derecho de X
    puntero->father = hijoIzquierdo;             // Asignar a X como padre de Y
}



Nodo* RBtree::insertarNodo(Nodo* &padre, Nodo* &puntero){
    // Arbol vacio
    if (padre == nullptr) return puntero;
    
    // Si el nuevo numero es menor al padre
    if(puntero->dato < padre->dato){
        padre->left = insertarNodo(padre->left,puntero);
        padre->left->father = padre;
    }
    // Si el nuevo numero es mayo al padre
    else if (puntero->dato > padre->dato){
        padre->right = insertarNodo(padre->right,puntero);
        padre->right->father = padre;
    }
    return padre;
}


void RBtree::corregirArbol(Nodo* &puntero){
    Nodo* padre  = nullptr;
    Nodo* abuelo = nullptr;
    while ( puntero != root &&  getColor(puntero)==RED && getColor(puntero->father)==RED ){
        padre  = puntero->father;
        abuelo = padre  ->father;

        // El padre esta a la izquierda
        if (padre == abuelo->left){
            Nodo* tio = abuelo->right;

            // CASO I: padre y tio son rojos
            if(  getColor(tio) ==RED ){
                setColor(padre ,BLACK);
                setColor(tio   ,BLACK);
                setColor(abuelo,RED  );
                puntero = abuelo;
            }
            else{
                // CASO II: padre y el hijo tienen distintas direcciones
                if(puntero == padre->right){
                    rotarIzquierda(padre);
                    puntero = padre;
                    padre = puntero->father;
                }

                // CASO III: padre y el hijo tienen la misma dirección
                rotarDerecha(abuelo);
                std::swap(padre->color,abuelo->color);
                puntero = padre;
            }
        }

        // El padre esta a la derecha
        else{
            Nodo* tio = abuelo->left;

            // CASO I: padre y tio son rojos
            if(  getColor(tio) ==RED ){
                setColor(padre ,BLACK);
                setColor(tio   ,BLACK);
                setColor(abuelo,RED  );
                puntero = abuelo;
            }
            else{
                // CASO II: padre y el hijo tienen distintas direcciones
                if(puntero == padre->left){
                    rotarDerecha(padre);
                    puntero = padre;
                    padre = puntero->father;
                }

                // CASO III: padre y el hijo tienen la misma dirección
                rotarIzquierda(abuelo);
                std::swap(padre->color,abuelo->color);
                puntero = padre;
            }
        }
    }
    setColor(root,BLACK);
    
}

void RBtree::insertar(int dato){
    Nodo *puntero = new Nodo(dato);
    root = insertarNodo(root,puntero);
    corregirArbol(puntero);
}

vector<int> RBtree::inorden(){
    traversal.clear();          // Vaciar vector
    inordenHelper(this->root);  // Funcion inOrden recursiva
    return traversal;           // Vector con el camino
}

void RBtree::inordenHelper(Nodo* nodo){     // Inorder traversal recursivo
    if(nodo == nullptr) return;             // Si estamos apuntando a un nulo, retornar.

    inordenHelper(nodo->left);              // Explorar todo el sub-arbol izquierdo.
    traversal.push_back(nodo->dato);        // Agregar al vector
    inordenHelper(nodo->right);             // Explorar todo el sub-arbol derecho.
}

vector<int> RBtree::preorden(){
    traversal.clear();
    preordenHelper(this->root);
    return traversal;
}

void RBtree::preordenHelper(Nodo* nodo){// PreOrder traversal recursivo
    if(nodo == nullptr) return;         // Si estamos apuntando a un nulo, retornar.

    traversal.push_back(nodo->dato);    // Agregar root al vector
    preordenHelper(nodo->left);         // Explorar todo el sub-arbol izquierdo
    preordenHelper(nodo->right);        // Explorar todo el sub-arbol derecho
}

vector<int> RBtree::posorden(){
    traversal.clear();          // vaciar el vector
    posordenHelper(this->root); // funcion posorden recursiva
    return traversal;           // vector con el camino
}

void RBtree::posordenHelper(Nodo* nodo){    // PostOrder traversal recursivo
    if(nodo == nullptr) return;             // Si es nulo, retornar.

    posordenHelper(nodo->left);             // Explorar todo el sub-arbol izquierdo.
    posordenHelper(nodo->right);            // Explorar todo el sub-arbol derecho.
    traversal.push_back(nodo->dato);        // Agregar al vector
}

Nodo* RBtree::search(int num){              // Funcion de busqueda por dato
    if(num == root->dato) return root;      // Si el elemento a buscar es el root retornarlo.
    Nodo* ptr = root;

    while(ptr->dato != num){                // Mientras no se haya encontrado el nodo:

        if(ptr->dato < num){                // Si num es mayor al nodo actual, intentar ir a la derecha
            if(ptr->right == nullptr) break;
            ptr = ptr->right;
        }
        else if(ptr->dato > num){           // Si num es menor al nodo actual, intentar ir a la izquierda
            if(ptr->left == nullptr) break;
            ptr = ptr->left;
        }

    }

    if(ptr->dato == num) return ptr;        // Si se encontro el nodo, retornar
    else return nullptr;                    // Si no, dar null
}

Nodo* RBtree::minimum(Nodo* ptr){
    while(ptr->left != nullptr){
        ptr = ptr->left;
    }
    return ptr;
}

Nodo* RBtree::maximum(Nodo* ptr){
    while(ptr->right != nullptr){
        ptr = ptr->right;
    }
    return ptr;
}

void RBtree::swap(Nodo* n1, Nodo* n2){  // funcion auxiliar para cambiar nodos
    if(n1 == root) root = n2;
    else if(n1 == n1->father->left){    // n1 es un hijo izq?
        n1->father->left = n2;          // cambiar el hijo por n2
    }
    else{                               // n1 es un hijo derecho?
        n1->father->right = n2;         // cambiar el hijo por n2
    }

    n2->father = n1->father;
}

void RBtree::corregirEliminado(Nodo* nodo){
    Nodo* aux = nodo;
    while(nodo != root && nodo->color == BLACK){
        if(nodo == nodo->father->left){
            aux = nodo->father->right;
            if(aux->color == RED){
                aux->color = BLACK;
                nodo->father->color = RED;
                rotarIzquierda(nodo->father);
                aux = nodo->father->right;
            }

            if(aux->left->color == BLACK && aux->right->color == BLACK){
                aux->color = RED;
                nodo = nodo->father;
            }
            else {
                if(aux->right->color == BLACK){
                    aux->left->color = BLACK;
                    aux->color = RED;
                    rotarDerecha(aux);
                    aux = nodo->father->right;
                }

                aux->color = nodo->father->color;
                nodo->father->color = BLACK;
                aux->right->color = BLACK;
                rotarIzquierda(nodo->father);
                nodo = root;
            }
        }

        else{
            aux = nodo->father->left;
            if(aux->color == RED){
                aux->color = BLACK;
                nodo->father->color = RED;
                rotarDerecha(nodo->father);
                aux = nodo->father->left;
            }

            if(aux->right->color == BLACK && aux->right->color == BLACK){
                aux->color = RED;
                nodo = nodo->father;
            }
            else{
                if(aux->left->color == BLACK){
                    aux->right->color = BLACK;
                    aux->color = RED;
                    rotarIzquierda(aux);
                    aux = nodo->father->left;
                }

                aux->color = nodo->father->color;
                nodo->father->color = BLACK;
                aux->left->color = BLACK;
                rotarDerecha(nodo->father);
                nodo = root;
            }
        }
    }
    nodo->color = BLACK;
}

bool RBtree::eliminarNodo(int dato){
    Nodo* toErase = search(dato);           // Localizacion del puntero en el nodo.
    if(toErase == nullptr) return false;    // No existe el nodo a eliminar.

    Nodo* ptr1 = toErase;
    Nodo* ptr2 = toErase;
    int originalColor = toErase->color;

    if(toErase->left == nullptr) {
        ptr1 = toErase->right;
        swap(toErase, toErase->right);
    }
    else if(toErase->right == nullptr){
        ptr1 = toErase->left;
        swap(toErase, toErase->left);
    }
    else{
        ptr2 = minimum(toErase->right);
        originalColor = ptr2->color;
        ptr1 = ptr2->right;
        if(ptr2->father == toErase){
            ptr1->father = ptr2;
        }
        else{
            swap(ptr2, ptr2->right);
            ptr2->right = toErase->right;
            ptr2->right->father = ptr2;
        }

        swap(toErase, ptr2);
        ptr2->left = toErase->left;
        ptr2->left->father = ptr2;
        ptr2->color = toErase->color;
    }

    delete toErase;
    if(originalColor == 1){
        corregirEliminado(ptr1);
    }

    return true;
}

Nodo* RBtree::getRoot(){
    return root;
}

RBtree::RBtree(){
    root = nullptr;
}

RBtree::~RBtree(){
}
