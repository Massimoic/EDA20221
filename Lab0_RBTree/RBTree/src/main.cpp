#include "rbtree.h"
#include "rbtree.cpp"
#include <iostream>
#include <vector>

using namespace std;


int main(){

    RBtree *test = new RBtree();
    test->insertar(3);
    test->insertar(4);
    test->insertar(9);
    test->insertar(1);
    test->insertar(11);
    test->insertar(15);
    test->insertar(10);

    vector<int> v1 = test->inorden();
    vector<int> v2 = test->preorden();
    vector<int> v3 = test->posorden();

    for(auto a : v1){
        cout << a << "-> ";
    }
    cout << endl;
    for(auto a : v2){
        cout << a << "-> ";
    }
    cout << endl;
    for(auto a : v3){
        cout << a << "-> ";
    }

    return 0;
}