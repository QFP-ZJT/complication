//
// Created by Jingtao Zhang on 17/11/3.
//

#include "BLOCK.h"
#include <iostream>
#include <iomanip>
#include <strstream>

using namespace std;

char getfc(string a);

int transfer(char a);

bool inVT(string a);

bool IsLetter(char ch);

bool IsDigit(char ch);

BLOCK::BLOCK() {
    //
    int i;//初始化table
    name = new std::string[100];
    table = new int *[100];
    for (i = 0; i < 100; i++) {
        table[i] = new int[4];
    }
    code = new int *[100];
    expression = new string[100];
    for (i = 0; i < 100; i++) {
        code[i] = new int[3];
        code[i][0] = -1;
    }

    code_index = 0;
    input.open(word_ans, ios::in);
    word = "";
    index = 0;
    leavel.push(0);
    addr.push(0);
    tab = "";
}

bool BLOCK::haveword() {
    if (!input.eof())
        return true;
    return false;
}

bool BLOCK::Getword() {
    if (!input.eof()) {
        input >> word;
        return true;
    }
    return false;
}

bool BLOCK::Getword(int i) {
    int j;
    for (j = 0; j < i && !input.eof(); j++) {
        input >> word;
    }
    if (j == i)
        return true;
    return false;
}

void BLOCK::emit(int a, int b, int c) {
    code[code_index][0] = a; // 栈顶布尔值为假  转向a域
    code[code_index][1] = b;
    code[code_index][2] = c;
    code_index++;
}

int BLOCK::Getsize(int lo) {
    int size = 0;
    while (lo - 1 >= 0 && table[lo - 1][TLEAVEL] == leavel.top() && table[lo - 1][TKIND] == VARIABLE) {
        size++;
        lo--;
    }
    return size;
}

void BLOCK::Analyconst() {
    type = CONSTANT;
    do {
        Getword(3);
        Getword();// name
        name[index] = word;
        table[index][TKIND] = type;
        table[index][TLEAVEL] = leavel.top();
        table[index][TADDR] = -1;
        Getword();// _
        Getword();// _
        if (word == "=") {
            Getword();// _
            Getword();// _
            Getword();// num
            if (word == "num") {
                Getword();// _
                Getword();// value
                table[index][TVALUE] = atoi(word.c_str());
            } else {
                cout << "error" << "常量的定义中等号右侧不是数字";
                exit(-1);
            }
            Getword();// , or ;
        }
        index++;
    } while (word == ",");
    if (word != ";") {
        cout << "期望   ;   or    ,";
        exit(-1);
    }
    Getword(3);
    cout << tab << "CONST  分析结束  当前词位:" << word << endl;
}

void BLOCK::Analyvar() {
    type = VARIABLE;
    do {
        Getword(3);
        Getword();// name
        name[index] = word;
        table[index][TKIND] = type;
        table[index][TVALUE] = -1;
        table[index][TLEAVEL] = leavel.top();
        table[index][TADDR] = addr.top();
        addr.pop();
        addr.push(table[index][TADDR] + 1);
        Getword();// _
        Getword();// _
        if (word == "=") {
            Getword(3);
            if (word == "num") {
                Getword(2);
                table[index][TVALUE] = atoi(word.c_str());
            } else {
                cout << "error" << "常量的定义中等号右侧不是数字";
                exit(-1);
            }
            Getword();
        }
        index++;
    } while (word == ",");
    if (word != ";") {
        cout << "期望   ;   or    ,";
        exit(-1);
    }
    Getword(3);
    cout << tab << "VAR  分析结束  当前词位:" << word << endl;
    //emit  计算应该开辟的空间
    SIN.pop();                  // 弹出0
    SIN.push(Getsize(index));   // 更新实际需要的数据区
}

void BLOCK::Analyprocedure() {
    type = PROCEDURE;
    Getword(3);
    if (word != "id") {
        cout << "error, expected to get a id" << endl;
        exit(-1);
    }
    Getword();
    name[index] = word;
    table[index][TKIND] = type;
    table[index][TLEAVEL] = leavel.top();
    table[index][TADDR] = addr.top();
    addr.pop();
    addr.push(table[index][TLEAVEL] + 1); // 本层下一次应该使用的地址
    addr.push(0);                       // 添加下一层的起始地址
    leavel.pop();
    leavel.push(table[index][TLEAVEL] + 1);//层递增
    PIN.push(index);
    index++;
    Getword(2);
    if (word != ";")
        cout << "期望得到一个  ;" << endl;
    Getword(3);
    cout << tab << "PROCEDURE " << name[index - 1] <<
         " 分析结束  当前词位:" << word << endl;
    tab += "  ";
    SIN.push(0);
}


/** 转换说明
 * N 保留字 返回 －1
 * 如果是字母 返回6   按照i处理
 * 其余不再解释
 * */
int transfer(char a) {
    if (a == 'N')
        return -1;
    if (IsLetter(a) || isdigit(a))
        return 6;
    switch (a) {
        case '+':
            return 0;
        case '-':
            return 1;
        case '*':
            return 2;
        case '/':
            return 3;
        case '(':
            return 4;
        case ')':
            return 5;
        case ';':
            return 7;
        default:
            return -1;
    }
}

int transferopr(string a) {
    if (a == "+")
        return ADD;
    else if (a == "-")
        return SUB;
    else if (a == "*")
        return MUL;
    else if (a == "/")
        return DIV;
    else if (a == "(")
        return MERGE;
    else if (a == ")")
        return MERGE;
    else if (a == "<")
        return SMALLER;
    else if (a == "<=")
        return ESMALLER;
    else if (a == "#")
        return NOTEQUAL;
    else if (a == ">")
        return GRATER;
    else if (a == ">=")
        return EGRATER;
    else return -1;

}

char getfc(string a) {
    const char *p = a.c_str();
    return *p;
}

bool inVT(string a) {
    if (transfer(getfc(a)) != -1)
        return true;
    return false;
}

int BLOCK::get_table_index(string na, int kind) {
    for (int i = 0; i < index; i++) {
        if (name[i] == na && table[i][TKIND] == kind)
            return i;
    }
    cout << "找不到变量" << na << endl;
    return -1;
}

int BLOCK::comparepri(string aa, string bb) {
    char a = getfc(aa);
    char b = getfc(bb);
    try {
        if (priority[transfer(a)][transfer(b)] != -2)
            return priority[transfer(a)][transfer(b)];
        else {
            cout << "运算符出错:" << "  a:" << a << "  b:" << b << endl;
            exit(-1);
        }
    } catch (exception e) {
        cout << "非法比较" << aa << "   " << bb << endl;
        exit(-1);
    }
}

int convertStringToInt(const string &s) {
    int val;
    std::strstream ss;
    ss << s;
    ss >> val;
    return val;
}

bool IsLetter(char ch) {
    if ((ch >= 'a' && ch <= 'z') || ch >= 'A' && ch <= 'Z')
        return true;
    return false;
}

bool IsDigit(char ch) {
    if (ch >= '0' && ch <= '9')
        return true;
    return false;
}

/*当前word为表达式的第一个位置的前一个位置*/
void BLOCK::Analystatement() {
    cout << tab << "进入表达式分析模块" << endl;
    int k = 1;          // 栈顶的位置
    int j;
    string Q;
    string N = "N";
    string temp = ";";
    bool exi = false;
    expression[k] = ";";
    do {
//        cout << "stack";
//        for (int i = 1; i <= k; i++)
//            cout << expression[i];
//        cout << endl;
        if (!(word == "id" || word == "num" || word == "+" || word == "-" || word == "*" || word == "/" ||
              word == "(" || word == ")" || word == ";")) {
            if (expression[k] != ";") {
                temp = word;
                word = ";";
                exi = true;
            }
        }
        if (inVT(expression[k]))//在运算符之中
            j = k;
        else j = k - 1;
        string aaa = expression[j];
        while (comparepri(expression[j], word) == 1) {
            do {
                Q = expression[j];
                if (inVT(expression[j - 1]))
                    j = j - 1;
                else
                    j = j - 2;
            } while (comparepri(expression[j], Q) != -1);
            //  规约
            cout << tab << "  规约:";
            for (int i = j + 1; i <= k; i++) {
                cout << expression[i] << " ";
            }
            cout << ";  ";

            //载入数据
            for (int i = j + 1; i <= k; i++) {
                if (expression[i] != "N" && IsLetter(getfc(expression[i]))) {
                    //  载入常量或者变量    id num
                    int in = get_table_index(expression[i], VARIABLE);//得到变量的值
                    int le; //层差
                    if (in != -1) {
                        le = leavel.top() - table[in][TLEAVEL];
                        if (le < 0) {
                            cout << "层差为负:" << expression[i] << endl;
                            exit(-1);
                        }
                        emit(LOD, le, table[in][TADDR]);
                    } else {
                        in = get_table_index(expression[i], CONSTANT);//得到变量的值
                        if (in != -1) {
                            le = leavel.top() - table[in][TLEAVEL];
                            if (le < 0) {
                                cout << "层差为负:" << expression[i] << endl;
                                exit(-1);
                            }
                            emit(LIT, le, table[in][TVALUE]);
                        } else {
                            cout << "未发现合理变量或常量" << endl;
                        }
                    }
                } else if (IsDigit(getfc(expression[i]))) {   //数字
                    emit(LIT, 0, convertStringToInt(expression[i]));
                } else {

                }
            }
            for (int i = j + 1; i <= k; i++) {
                if (transfer(getfc(expression[i])) != -1 && transfer(getfc(expression[i])) != 6) {
                    emit(OPR, 0, transferopr(expression[i]));
                    break;
                }
            }
            k = j + 1;
            expression[k] = N;
        }
        if (comparepri(expression[k], word) != 1 && !exi) {
            k++;
            if (word == "id") {
                Getword();
                expression[k] = word;
                Getword(2);
            } else if (word == "num") {
                Getword(2);
                expression[k] = word;
                Getword();
            } else {
                expression[k] = word;
                Getword(3);
            }
        } else if (!exi) {
            cout << "不被期望的表达式" << endl;
        }
//        if (expression[3] == ";")
//            break;
    } while (!exi && ((expression[3] != ";" && k == 3) || k != 3));   // 判断什么时候跳出循环   ;N;
    if (exi)
        word = temp;
    // 出循环的时候
    cout << endl << tab << "表达式  分析结束   当前位置" << word << endl;

}

// 分析 IF_THEN_ELSE 语句的
void BLOCK::IF_THEN_OR_ELSE() {
    cout << tab << "IF_THEN_ORELSE" << endl;
    tab += "  ";
    if (word == "if") {
        Getword(3);
        Analycondition();
        int JF = code_index;
        emit(JPC, 0, -1);           // 为假跳转
        int JT = code_index;
        emit(JMP, 0, -1);           // 无条件跳转
        if (word == "then") {
            cout << tab << "THEN" << endl;
            int M1 = code_index;
            code[JT][2] = M1;
            Getword(3);
            S();//执行体语句分析
            if (word == "else") {
                cout << tab << "ELSE" << endl;
                int N1 = code_index;
                emit(JMP, 0, -1);     //then结束之后,无条件跳转   向外跳转
                Getword(3);
                int M2 = code_index;
                code[JF][2] = M2;
                S();
                code[N1][2] = code_index;   // 跳转到语法体外的一条语句
                // TODO nextList()存在问题
                tab = tab.substr(0, tab.length() - 2);
                return;//完成IF_THEN_ELSE语句的分析
            } else {     //没有else
                // 回填
                code[JF][2] = code_index;   //TODO 等待验证正确性
                tab = tab.substr(0, tab.length() - 2);
                return;//完成IF_THEN语句的分析
            }
        } else {
            cout << "不被期望进入THEN语句之中" << endl;
        }
    } else {
        cout << "不被期望进入IF_THEN_ELSE语句之中" << endl;
    }
}

// 分析条件语句
void BLOCK::Analycondition() {
    cout << tab << "进入条件语句分析模块" << endl;
    tab += "  ";
    Analystatement();
    if (word == "=" || word == "#" || word == "<" || word == "<=" || word == ">" || word == ">=") {
        string temp = word;
        Getword(3);
        Analystatement();               //可以保证进入下一行的起始位置
        emit(OPR, 0, transferopr(temp));
    } else {
        //odd<表达式>
    }
    tab = tab.substr(0, tab.length() - 2);
    cout << tab << "条件语句  分析结束  当前词位" << word << endl;
    return;
}

void BLOCK::S() {
    if (word != "var" && word != "const" && word != "procedure" && prostart) {//申请开辟空间
        main = code_index;
        if (!PIN.empty()) { // 最后一次不需要
            table[PIN.top()][TVALUE] = code_index;
            PIN.pop();
        }
        emit(INT, leavel.top(), SIN.top());
        SIN.pop();
        prostart = false;
    }
    if (word == "const") {//说明部分
        Analyconst();
        return;
    } else if (word == "var") {
        Analyvar();
        return;
    } else if (word == "procedure") {
        Analyprocedure();
        return;
    } else if (word == "if") {
        IF_THEN_OR_ELSE();
    } else if (word == "while") {
        WHILE_DO();
    } else if (word == "begin") {
        BEGIN_END();
    } else if (word == "call") {
        Getword(4);// 得到程序名称
        int in = get_table_index(word, PROCEDURE);
        emit(CAL, 0, table[in][TVALUE]);// 跳转指令
        Getword(2);// 得到下一句代码的首部
    } else if (word == "write") {
        cout << tab << "write" << endl;
        tab += "  ";
        Getword(3);
//        do {
        tab += "  ";
        Analystatement();
        tab = tab.substr(0, tab.length() - 2);
        emit(OPR, 0, WRITE);
//        } while (haveword() && word != ")");
        tab = tab.substr(0, tab.length() - 2);
        cout << tab << "write  分析结束  当前位置:" << word << endl;
    } else if (word == "read") {
        cout << tab << "read" << endl;
        tab += "  ";
        cout << tab;
        Getword(3);
        do {
            Getword(3);
            if (word == "id") {
                Getword();
                emit(OPR, 0, READ);
                int in = get_table_index(word, VARIABLE);
                int c = leavel.top();
                int d = table[in][TLEAVEL];
                emit(STO, leavel.top() - table[in][TLEAVEL], table[in][TADDR]);
                cout << word << "  ";
                Getword(2);
            }
        } while (haveword() && word != ")");
        Getword(3);
        tab = tab.substr(0, tab.length() - 2);
        cout << endl << tab << "read  分析结束  当前位置:" << word << endl;
    } else if (word == ";") {
        int last = leavel.top();
        leavel.pop();
        leavel.push(last - 1);//放回上一层
        addr.pop();
        Getword(3);//得到下一句代码的首部
        tab = tab.substr(0, tab.length() - 2); // 缩进
        emit(OPR, 0, CLC);
        prostart = true;
    } else if (word == "id") {
        Getword();
        string name = word;
        Getword(2);
        if (word == ":=") {
            Getword(3);
            Analystatement();
            int it = get_table_index(name, VARIABLE);
            int le = leavel.top() - table[it][TLEAVEL];
            int lo = table[it][TADDR];
            if (le < 0) {
                cout << "层差为负:" << name << endl;
                exit(-1);
            }
            emit(STO, le, lo);//TODO 计算层差与相对位置
        } else if (word=="=") {
//            Getword(5);
//            int it = get_table_index(name, CONSTANT);
//            int le = leavel.top() - table[it][TLEAVEL];
//            if (le < 0) {
//                cout << "层差为负:" << name << endl;
//                exit(-1);
//            }
//            table[it][TVALUE] = convertStringToInt(word);
//            Getword(4);
        }
    }
}

void BLOCK::BEGIN_END() {
    cout << tab << "BEGIN" << endl;
    tab += "  ";
    if (word == "begin") {
        Getword(3);
        do {
            S();
        } while (haveword() && word != "end");
        if (!Getword(3))
            cout << "begin没有end" << endl;
    } else {
        cout << "不被期望进入begin   end的语法分析" << endl;
    }
    tab = tab.substr(0, tab.length() - 2);
    cout << tab << "END 分析结束  当前词位" << word << endl;
}

void BLOCK::WHILE_DO() {
    cout << tab << "WHILE  " << endl;
    tab += "  ";
    if (word == "while") {
        Getword(3);
        int M1 = code_index;
        Analycondition();
        int JF = code_index;
        emit(JPC, 0, -1);           // 为假跳转
        int JT = code_index;
        emit(JMP, 0, -1);           // 无条件跳转
        if (word == "do") {
            int M2 = code_index;
            code[JT][2] = M2;
            Getword(3);
            S();
            emit(JMP, 0, M1);
            code[JF][2] = code_index;
        } else {
            cout << "期望一个do" << endl;
            exit(-1);
        }
    } else {
        cout << "不被期望进入while" << endl;
        exit(-1);
    }
    tab = tab.substr(0, tab.length() - 2);
    cout << "WHILE 分析结束  当前词位" << word << endl;
}

void BLOCK::run() {
    Getword();
    SIN.push(0);
    prostart = true;
    while (true) {
        S();
        if (!haveword())
            break;
    }
    p();
}

string transferopr(int a) {
    switch (a) {
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case MUL:
            return "MUL";
        case DIV:
            return "DIV";
        case MERGE:
            return "MERGE";
        case EQUAL:
            return "=";
        case NOTEQUAL:
            return "#";
        case SMALLER:
            return "<";
        case GRATER:
            return ">";
        case ESMALLER:
            return "<=";
        case EGRATER:
            return ">=";
        case READ:
            return "READ";
        case WRITE:
            return "WRITE";
        case CLC:
            return "CLC";
        default:
            cout << "无法识别的操作符" << a << endl;
            return "error";
    }
}

string transferop(int a) {
    switch (a) {
        case LIT:
            return "LIT";
        case LOD:
            return "LOD";
        case STO:
            return "STO";
        case CAL:
            return "CAL";
        case INT:
            return "INT";
        case JMP:
            return "JMP";
        case JPC:
            return "JPC";
        case OPR:
            return "OPR";

        default:
            return "error";
    }
}

void BLOCK::p() {
    int i;
    cout << "name\ttype\tleavel\tvalue\taddr" << endl;
    for (i = 0; i < index; i++) {
        string temp;
        switch (table[i][TKIND]) {
            case CONSTANT:
                temp = "CONSTANT";
                break;
            case VARIABLE:
                temp = "VARIABLE";
                break;
            case PROCEDURE:
                temp = "procedure";
                break;
        }
        cout << name[i] << "\t" << temp << "\t" << table[i][TLEAVEL] << "\t\t" << table[i][TVALUE] << "\t\t"
             << table[i][TADDR] << endl;
    }
    for (i = 0; i < code_index; i++) {
        cout << i << ":\t";
        if (code[i][0] == OPR)
            cout << transferop(code[i][0]) << " " << code[i][1] << " "
                 << transferopr(code[i][2]) << endl;
        else
            cout << transferop(code[i][0]) << " " << code[i][1] << " " << code[i][2]
                 << endl;
    }
}