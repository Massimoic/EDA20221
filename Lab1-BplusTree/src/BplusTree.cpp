#include "BplusTree.h"

#include <algorithm>
#include <iostream>
#include <queue>

using namespace std;

size_t insertHelper(vector<int> &keys, int value){

    if(keys.empty()){
        keys.push_back(value);
        return 0;
    }

    auto temp = keys.begin();
    while(value > *temp && temp != keys.end()) temp++;

    auto dist = distance(keys.begin(), temp);
    keys.insert(temp, value);

    return dist;
}

void BplusTree::insertar(int value){
    
    // Si no existe el root, arbol vacio, solo insertar elemento y asignar como root.
    if(root == nullptr){
        root = new Node();
        root->keys.push_back(value);
        return;
    }

    // Localizar lugar de insercion.
    auto cursor = root;
    bool aux;

    while(cursor && cursor->isLeaf==false){
        aux = false;

        // Si es menor, ir al hijo izquierdo.
        if(value <= cursor->keys.front()){
            cursor = cursor->children[0];
            continue;
        }

        for(int i = 0 ; i < cursor->keys.size()-1 ; i++){

            if(value > cursor->keys[i] && value <= cursor->keys[i+1]) {
                cursor = cursor->children[i+1];
                aux = true;
                break;
            }
        }

        if(!aux && value > cursor->keys.back()) cursor = cursor->children.back();
    }

    // Hay espacio para insertar:
    if(cursor->keys.size() < capacity){
        insertHelper(cursor->keys, value);
        return;
    }

    // No hay espacio, hacer split
    split(cursor, value);
}

void BplusTree::split(Node* node, int value){

    // Calcular el medio
    insertHelper(node->keys, value);
    int mid = int(node->keys.size() / 2);
    auto midKey = node->keys[mid];

    if(node->parent == nullptr){
        // Si no tiene padre, crear nuevo nodo padre.
        node->parent = new Node(false);
        root = node->parent;
        node->parent->children.push_back(node);
    }

    // Padre existe y tiene espacio:
    // Insertar y separar nodo

    if(node->parent->keys.size() < capacity){
        auto index = insertHelper(node->parent->keys, midKey);
        auto brother = newBrother(node, midKey);

        if(index+1 >= node->parent->children.size()) {
            node->parent->children.push_back(brother);
        }

        else{
            node->parent->children.insert(node->parent->children.begin() + index + 1 , brother);
        }
    }

    // Padre existe no hay espacio:
    // Separar nodo, separar padre

    else{
        auto brother = newBrother(node, midKey);
        int i = 0;

        // iterar padre y encontrar indice del nuevo hijo
        while(i < node->parent->children.size() && node->parent->children[i] != node) i++;

        // Insertar nodo hermano
        if(i + 1 >= node->parent->children.size()){
            node->parent->children.push_back(brother);
        }
        else{
            node->parent->children.insert(node->parent->children.begin() + i + 1, brother);
        }

        // Separar al padre
        split(node->parent, midKey);
    }

}

Node* BplusTree::newBrother(Node* node, int midKey){
    auto brother = new Node(node->isLeaf);
    
    // Copy keys

    auto it = node->keys.begin();
    while(it != node->keys.end()){
        if(*it == midKey){
            if(node->isLeaf) { // Solo copiar dato a subir si es nodo hoja
                brother->keys.push_back(*it);
            }
            node->keys.erase(it);
        }

        else if(*it > midKey){
            brother->keys.push_back(*it);
            node->keys.erase(it);
        }

        else{
            it++;
        }
    }

    // Copy children

    if(node->isLeaf == false){
        int i = 0;
        while(i <= node->keys.size()) i++;
        while(i < node->children.size()){
            node->children[i]->parent = brother;
            brother->children.push_back(node->children[i]);
            node->children.erase(node->children.begin() + i);
        }
    }

    // Si es un nodo hoja, actualizar links al siguiente

    if(node->isLeaf == true){
        auto ptr = node->ptr;
        node->ptr = brother;
        brother->ptr = ptr;
    }

    // Asignar padre

    brother->parent = node->parent;
    return brother;
}

vector<int> BplusTree::bfs(){

    queue<Node*> queue;
    vector<int> vec;
    if(root == nullptr) return vec;
    queue.push(root);

    while(!queue.empty()){
        auto node = queue.front();
        queue.pop();

        for(auto value : node->keys){
            vec.push_back(value);
        }

        for(auto child : node->children){
            queue.push(child);
        }
    }

    return vec;
}

BplusTree::~BplusTree(){

}