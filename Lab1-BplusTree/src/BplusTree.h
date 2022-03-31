#include <vector>
using namespace std;
#ifndef BPLUSTREE_H
#define BPLUSTREE_H

struct Node{
    // Atributos
    int data;

    // Relacion con otros Nodes
    Node *left;
    Node *right;
    Node *parent;

    // Constructor
    Node(int);
};

class BPlusTree{
    private:
    Node *root;
    Node *insertNode(Node* &, Node* &);
}



#endif