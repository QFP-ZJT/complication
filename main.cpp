#include <iostream>
#include "GETSYM.h"
#include "BLOCK.h"
#include "COMMIT.h"

using namespace std;

int main() {
//    std::cout<<("N"=="n")<<std::endl;
//    int *mem = new int[10];
//    int T = 0;
//    mem[++T] = T - 3;
//    for (int i = 0; i < 10; i++)
//        cout << mem[i] << "  ";


    GETSYM *a = new GETSYM();
    a->run();
    BLOCK *b = new BLOCK();
    b->run();
    COMMIT *c = new COMMIT(b->code,b->main);
    c->CRUN();
    return 0;
}