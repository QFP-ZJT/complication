//
// Created by Jingtao Zhang on 17/10/27.
//

#ifndef COMPLICATION_GETSYM_H
#define COMPLICATION_GETSYM_H
#define COMMENT_CHAR '#'
#define reserve_file "/Users/zjtao/CLionProjects/complication/reserve.cfg"
#define word_ans "/Users/zjtao/CLionProjects/complication/word_ans.txt"
#define filename_input "/Users/zjtao/CLionProjects/complication/code.txt"
#include <map>
#include <string>
#include <fstream>

class GETSYM {
public:
    GETSYM();           // 构造方法
    ~GETSYM();          // 析构方法
    void run();         //启动词法分析程序
    void GetChar();     // 将内容读到ch中
    bool GetBC();       // 调用GetChar(),直到读进一个非空白的字符
    void Concat();      // 将ch拼接到strToken中
    bool IsLetter();    // 判断是否是字母
    bool IsDigit();     // 判断是否是数字
    int  Reserve();     // 检索保留字表
    void Retract();     // 指针前移
    int  InsertId();    // 将strToken中的标志符插入符号表，返回符号表的指针
    int  InsterConst(); // 将strToken中的常数插入常数表，返回常数表的指针
    int  InsterSYM();   // 将单词存储到文件中
    bool ReadConfig(const std::string & filename, std::map<std::string, int> & m);  //读取保留字表
    void PrintConfig(const std::map<std::string, int> & m);


private:
    char ch;            //最新读进来的字符
    char * strToken;    //存放构成单词的符号
    std::map<std::string,int> reserve;//保留字表
    std::ofstream output;
    std::ifstream input;
};
#endif //COMPLICATION_GETSYM_H
