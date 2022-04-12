#include "BplusTree.h"

#include <algorithm>
#include <iostream>
#include <queue>

using namespace std;

const int DEGREE = 21;

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

BplusTree::BplusTree(){
    root = nullptr;
    deg = DEGREE;
    capacity = deg - 1;
    minKeys = ceil(deg / 2) - 1;
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
    queue<Node*> q;
    q.push(root);

    while(!q.empty()){
        auto cursor = q.front(); q.pop();

        for(auto child : cursor->children) q.push(child);

        delete cursor;
    }
}



void BplusTree::borrar(int key){

    // Si no existe el root, terminar
    if(root == nullptr) return;

    auto cursor = root;
    bool flagInternalNode = false;

    // Localizar el Nodo
    while(cursor && cursor->isLeaf == false){
        int i;

        for(i = 0 ; i < cursor->keys.size() ; ++i){
            if(key < cursor->keys[i]) break;

            else if(key == cursor->keys[i]) flagInternalNode = true;
        }

        cursor = cursor->children[i];
    }


    // Buscar el valor en nodo hoja
    int leafIdx = -1;

    for(int i = 0 ; i < cursor->keys.size() ; i++){
        if(cursor->keys[i] == key){
            leafIdx = i;
            break;
        }
    }

    // Si no existe, terminar
    if(leafIdx == -1) return;

    // Eliminar en el nodo hoja
    cursor->keys.erase(cursor->keys.begin() + leafIdx);

    // Si el nodo es root, terminar
    if(cursor == root){
        if(cursor->keys.empty()) {
            root = nullptr;
            delete cursor;
        }
        return;
    }

    // Si el nodo aun cumple con reglas, terminar
    if(cursor->keys.size() >= minKeys) {
        // Si es que encontramos nodo interno, eliminarlo
        if(flagInternalNode) deleteInternal(key);
        return;
    }

    auto sibling = findSibling(cursor);

    // Si existe un hermano:

    if(sibling.node){

        // Check si el hermano puede prestar un key

        if(sibling.idx != -1){

            // Izq, se inserta al comienzo
            if(sibling.direction == LEFT){
                cursor->keys.insert(cursor->keys.begin(), sibling.node->keys[sibling.idx]);
                sibling.node->keys.erase(sibling.node->keys.begin() + sibling.idx);
                int index = 0;
                while(index < sibling.node->parent->children.size() && sibling.node->parent->children[index] != sibling.node) index++;
                cursor->parent->keys[index-1] = cursor->keys.front();
            }

            // Derecha, se inserta al final
            else {
                cursor->keys.push_back(sibling.node->keys[sibling.idx]);
                sibling.node->keys.erase(sibling.node->keys.begin() + sibling.idx);
                int index = 0;
                while(index < sibling.node->parent->children.size() && sibling.node->parent->children[index] != sibling.node) index++;
                cursor->parent->keys[index-1] = cursor->keys.front();
            }

            if(flagInternalNode) deleteInternal(key);
        }

        // Si no puede prestar realizar un merge de nodos

        else{
            bool flagRoot;
            if(sibling.direction == LEFT){
                flagRoot = mergeNodes(sibling.node, cursor);
                cursor = sibling.node;
            }
            else flagRoot = mergeNodes(cursor, sibling.node);
            if(flagRoot) return;
        }
    }

    // Arreglar el arbol

    arreglar(cursor->parent);
    if(flagInternalNode) deleteInternal(key);
}

bool BplusTree::mergeNodes(Node* left, Node* right){

    // Si no tienen padre o el padre no es el mismo, terminar
    if(!left->parent || !right->parent) return false;
    if(left->parent != right->parent) return false;

    // Insertar keys del nodo derecho al nodo izquierdo
    for(auto key : right->keys){
        left->keys.push_back(key);
    }

    // Insertar hijos del nodo derecho al nodo izquierdo

    if(!left->isLeaf){
        for(auto node : right->children){
            node->parent = left;
            left->children.push_back(node);
        }
    }
    else{
        left->ptr = right->ptr;
    }

    if(left->parent == root && root->keys.size() == 1){
        root = left;
        
        if((int)left->keys.size() < (int)left->children.size() - 1){
            insertHelper(left->keys, left->parent->keys.front());
        }

        delete left->parent;
        left->parent = nullptr;
        delete right;
        return true;
    }
    else{
        int idx = 0;
        while(idx < right->parent->children.size() && right->parent->children[idx] != right) {
            idx++;
        }

        left->parent->keys.erase(left->parent->keys.begin() + idx - 1);
        left->parent->children.erase(left->parent->children.begin() + idx);

        delete right;
    }
    return false;
}

void BplusTree::arreglar(Node* node){

    if(node == root) return;
    if(node->keys.size() >= minKeys) return;

    auto sibling = findSibling(node);

    if(sibling.node){
        if(sibling.idx != -1) { // Hermano puede prestar

            int idx = 0;
            while(idx < node->parent->children.size() && node->parent->children[idx] != node){
                idx++;
            }

            if(sibling.direction == LEFT){ // Rotar a la derecha
                node->keys.insert(node->keys.begin(), node->parent->keys[idx-1]);
                node->parent->keys[idx-1] = sibling.node->keys.back();
                sibling.node->keys.pop_back();
                node->children.insert(node->children.begin(), sibling.node->children.back());
                node->children.front()->parent = node;
                sibling.node->keys.pop_back();
            }

            else{ // Rotar a la izquierda
                node->keys.insert(node->keys.begin(), node->parent->keys[idx+1]);
                node->parent->keys[idx+1] = sibling.node->keys.front();
                sibling.node->keys.erase(sibling.node->keys.begin());
                node->children.push_back(sibling.node->children.front());
                node->children.back()->parent = node;
                sibling.node->children.erase(sibling.node->children.begin());
            }

            return;
        }

        // No puede prestar, hacer merge de los nodos
        else{
            bool flag;
            if(sibling.direction == LEFT){
                flag = mergeNodes(sibling.node, node);
                node = sibling.node;
            }

            else flag = mergeNodes(node, sibling.node);

            if(flag) return;
        }

        if(node == root) return;
        if(node->keys.size() >= minKeys) return;

        arreglar(node->parent);
    }
}

void BplusTree::deleteInternal(int key){

    auto cursor = root;
    int i;
    bool flagFound = false;

    while(cursor && cursor->isLeaf == false){

        for(i = 0 ; i < cursor->keys.size() ; ++i){
            if(key < cursor->keys[i]) break;

            else if(key == cursor->keys[i]){
                flagFound = true;
                break;
            }
        }

        if(flagFound) break;
        cursor = cursor->children[i];
    }

    if(flagFound){
        auto update = cursor->children[i+1];
        while(!update->isLeaf) update = update->children.front();
        cursor->keys[i] = update->keys.front();
    }
}

keyDir BplusTree::findSibling(Node* node){
    Node* sibling = nullptr;
    int siblingIdx = -1;
    direction dir = LEFT;

    if(node->parent->children.size() < 2) {
        return{sibling, siblingIdx};
    }

    int idx = 0;

    while(idx < node->parent->children.size() && node->parent->children[idx] != node) idx++;

    if(idx == 0){
        sibling = node->parent->children[idx+1];
        dir = RIGHT;

        if(sibling->keys.size() > minKeys){
            siblingIdx = 0;
        }
    }

    else if(idx == node->parent->children.size() - 1){
        sibling = node->parent->children[idx-1];

        if(sibling->keys.size() > minKeys){
            siblingIdx = sibling->keys.size() - 1;
        }
    }

    else{
        sibling = node->parent->children[idx-1];

        if(sibling->keys.size() > minKeys){
            siblingIdx = sibling->keys.size() - 1;
        }

        else if(node->parent->children[idx+1]->keys.size() > minKeys){
            sibling = node->parent->children[idx+1];
            dir = RIGHT;
            siblingIdx = 0;
        }
    }

    return {sibling, siblingIdx, dir};
}