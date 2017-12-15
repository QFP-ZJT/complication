//
// Created by Jingtao Zhang on 17/12/1.
//

#include <iostream>
#include "COMMIT.h"

using namespace std;

COMMIT::COMMIT(int **c, int main) {
    OP = c;
    mem = new int[10000];
    P = main; // 下一条应该执行的指令
    T = -1; // 栈顶地址
    B = 0; // 程序基址
}


void COMMIT::CINT() {
    mem[++T] = OP[P][1] + 1;// 第几层  抄送              0
    mem[++T] = B;         //                            1
    ++T;                  // 返回位置                    2
    for (int i = 0; i < OP[P][1]; i++) {
        mem[++T] = mem[B + 3 + i];
    }
    B = T - OP[P][1] - 2;                         //更改程序基址
    mem[++T] = B;       // 自身位置
    T += OP[P][2];      // 开辟数据区
    P++;
}

void COMMIT::CLIT() {
    mem[++T] = OP[P][2];
    P++;
}

void COMMIT::CLOD() {
//    寻找层数  与地址
    int B_LE = mem[B + 2 + mem[B]/*自身B所在的位置*/- OP[P][1]];
    mem[++T] = mem[B_LE + mem[B_LE] + 3 + OP[P][2]];
    P++;
}

void COMMIT::CSTO() {
//    寻找层数  与地址
    int B_LE = mem[B + 2 + mem[B]/*自身B所在的位置*/- OP[P][1]];
    mem[B_LE + mem[B_LE] + 3 + OP[P][2]] = mem[T--];
    P++;
}

void COMMIT::CCAL() {
    mem[T + 3] = P;
    P = OP[P][2];
}

void COMMIT::CJMP() {
    P = OP[P][2];
}

void COMMIT::CJPC() {
    if (mem[T--] == 0)
        P = OP[P][2];
    else
        P++;
}

void COMMIT::COPR() {
    switch (OP[P][2]) {
        case ADD:
            T--;
            mem[T] = mem[T] + mem[T + 1];
            break;
        case SUB:
            T--;
            mem[T] = mem[T] - mem[T + 1];
            break;
        case MUL:
            T--;
            mem[T] = mem[T] * mem[T + 1];
            break;
        case DIV:
            T--;
            mem[T] = mem[T] / mem[T + 1];
            break;
        case MERGE:
            break;
        case EQUAL:
            T--;
            if (mem[T] == mem[T + 1])
                mem[T] = 1;
            else
                mem[T] = 0;
            break;
        case SMALLER:
            T--;
            if (mem[T] < mem[T + 1])
                mem[T] = 1;
            else
                mem[T] = 0;
            break;
        case ESMALLER:
            T--;
            if (mem[T] <= mem[T + 1])
                mem[T] = 1;
            else
                mem[T] = 0;
            break;
        case GRATER:
            T--;
            if (mem[T] > mem[T + 1])
                mem[T] = 1;
            else
                mem[T] = 0;
            break;
        case EGRATER:
            T--;
            if (mem[T] >= mem[T + 1])
                mem[T] = 1;
            else
                mem[T] = 0;
            break;
        case NOTEQUAL:
            T--;
            if (mem[T] != mem[T + 1])
                mem[T] = 1;
            else
                mem[T] = 0;
            break;
        case READ:
            std::cout << "请输入:";
            std::cin >> mem[++T];
            break;
        case WRITE:
            std::cout << "输出:" << mem[T] << std::endl;
            break;
        case CLC:
            P = mem[B + 2];
            T = B - 1;
            B = mem[B + 1];
            break;
        default:
            std::cout << "您不再服务区" << std::endl;
    }
    P++;
}

void COMMIT::CRUN() {
    cout << endl;
    while (OP[P][0] != -1) {
        switch (OP[P][0]) {
            case INT:
                CINT();
                break;
            case LOD:
                CLOD();
                break;
            case LIT:
                CLIT();
                break;
            case JMP:
                CJMP();
                break;
            case JPC:
                CJPC();
                break;
            case STO:
                CSTO();
                break;
            case CAL:
                CCAL();
                break;
            case OPR:
                COPR();
                break;
            default:
                std::cout << "您不在服务区" << std::endl;
        }
        PR();
    }
}

void COMMIT::PR() {
    cout << "P:" << P << endl;
    int temp_B = B;
    for (int i = T; i >= 0; i--)
        if (i == temp_B + 2)
            std::cout << i << "\tLP\t" << mem[i] << std::endl;
        else if (i == temp_B + 1)
            std::cout << i << "\tLB\t" << mem[i] << std::endl;
        else if (i == temp_B) {
            temp_B = mem[i + 1];
            std::cout << i << "\tNB\t" << mem[i] << std::endl;
        } else if (i == temp_B + 3 + mem[temp_B]) {
            std::cout << i << "\tDA\t" << mem[i] << std::endl;
        } else {
            std::cout << i << "\t  \t" << mem[i] << std::endl;
        }
    cout << endl;
}