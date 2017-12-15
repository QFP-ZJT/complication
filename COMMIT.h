//
// Created by Jingtao Zhang on 17/12/1.
//

#ifndef COMPLICATION_COMMIT_H
#define COMPLICATION_COMMIT_H

#define LIT 0
#define LOD 1
#define STO 2
#define CAL 3
#define INT 4
#define JMP 5
#define JPC 6
#define OPR 7

#define ADD 1
#define SUB 2
#define MUL 3
#define DIV 4
#define MERGE 5
#define EQUAL 6
#define SMALLER 7
#define GRATER 8
#define ESMALLER 9
#define EGRATER 10
#define NOTEQUAL 11
#define READ 12
#define WRITE 13
#define CLC 14

//  [BASE]
//  P_LAST
//  B_LAST
//  B_NUM         <-B

class COMMIT {
public:
    COMMIT(int **code,int main);   //执行的指令
    void CINT(); // 处理INT
    void CLIT();
    void CLOD();
    void CSTO();
    void CCAL();
    void CJMP();
    void CJPC();
    void COPR();
    void CRUN();
    void PR();
private:
    int *mem;
    int **OP;
    int P;  // 程序地址
    int T;  // 栈顶
    int B;  // 基址
};


#endif //COMPLICATION_COMMIT_H
