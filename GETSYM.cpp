//
// Created by Jingtao Zhang on 17/10/27.

#include <iostream>
#include "GETSYM.h"
#include <iomanip>

using namespace std;

GETSYM::GETSYM() {
    //定义
    strToken = new char[100];
    for (int i = 0; i < 100; i++)
        strToken[i] = '\0';
    //读取保留字表
    ReadConfig(reserve_file, reserve);
    output.open(word_ans, ios::out);
    input.open(filename_input, ios::in);
    if (!input || !output)
        cout << "文件打开失败" << endl;
    input >> noskipws;
}

void GETSYM::GetChar() {
    input >> ch;

};

bool GETSYM::GetBC() {
    while (!input.eof() && (ch == ' ' || ch == '\n')) {
        GetChar();
    }
    if (input.eof())
        return false;
    return true;
}

void GETSYM::Retract() {
    input.seekg(-1, ios::cur);
}

void GETSYM::Concat() {
    //拼接字符串
    int i = 0;
    while (strToken[i] != '\0')
        i++;
    strToken[i] = ch;
}

bool GETSYM::IsLetter() {
    if ((ch >= 'a' && ch <= 'z') || ch >= 'A' && ch <= 'Z')
        return true;
    return false;
}

bool GETSYM::IsDigit() {
    if (ch >= '0' && ch <= '9')
        return true;
    return false;
}

int GETSYM::Reserve() {
    map<string, int>::const_iterator it = reserve.find(strToken);
    if (it == reserve.end()) {
        return 0;
    } else {
        return it->second;
    }
}

//插入变量的名称
int GETSYM::InsertId() {
    output << setiosflags(ios::left) << setw(10) << "id" << resetiosflags(ios::left)
           << setiosflags(ios::right) << setw(6) << strToken << setw(8) << "_"
           << resetiosflags(ios::right) << endl;
    return 0;
}

// 插入到常数表
int GETSYM::InsterConst() {
    output << setiosflags(ios::left) << setw(10) << "num" << resetiosflags(ios::left)
           << setiosflags(ios::right) << setw(6) << "_" << setw(8) << strToken
           << resetiosflags(ios::right) << endl;
    return 0;
}

int GETSYM::InsterSYM() {
    output << setiosflags(ios::left) << setw(10) << strToken << resetiosflags(ios::left)
           << setiosflags(ios::right) << setw(6) << "_" << setw(8) << "_"
           << resetiosflags(ios::right) << endl;
}

void GETSYM::run() {
    int
            code, value;
    //初始化
    while (true) {
        ch = ' ';
        for (int i = 0; i < 100; i++)
            strToken[i] = '\0';
        if (!GetBC())
            break;
        if (IsLetter()) {
            while (IsLetter() || IsDigit()) {
                Concat();
                GetChar();
            }
            Retract();
            code = Reserve();//查表
            if (code == 0) { //不在保留字表中
                InsertId();  //将变量的名称输出到文件中
            } else {
                //是保留字
                InsterSYM();
            }

        } else if (IsDigit()) {
            while (IsDigit()) {
                Concat();
                GetChar();
            }
            Retract();
            //插入数字  返回常亮存放的地址
            InsterConst();
        } else if (ch == '=') {
            Concat();
            InsterSYM();
        } else if (ch == '+') {
            Concat();
            InsterSYM();
        } else if (ch == '-') {
            Concat();
            InsterSYM();
        } else if (ch == '*') {
            Concat();
            InsterSYM();
        } else if (ch == '/') {
            Concat();
            InsterSYM();
        } else if (ch == '#') {
            Concat();
            InsterSYM();
        } else if (ch == '<') {
            Concat();
            GetChar();
            if (ch == '=') {
                Concat();
                InsterSYM();
            } else {
                Retract();
                InsterSYM();
            }
        } else if (ch == '>') {
            Concat();
            GetChar();
            if (ch == '=') {
                Concat();
                InsterSYM();
            } else {
                Retract();
                InsterSYM();
            }
        } else if (ch == '(') {
            Concat();
            InsterSYM();
        } else if (ch == ')') {
            Concat();
            InsterSYM();
        } else if (ch == ',') {
            Concat();
            InsterSYM();
        } else if (ch == ';') {
            Concat();
            InsterSYM();
        } else if (ch == ':') {
            Concat();
            GetChar();
            if (ch == '=') {
                Concat();
                InsterSYM();
            } else {
                Retract();
                cout << "error:" << ch << "  " << strToken << endl;
            }
        } else {
            cout << "error:" << ch << "  " << strToken << endl;
        }
    }
    input.close();
    output.close();
}


//为了初始化保留字表
bool IsSpace(char c) {
    if (' ' == c || '\t' == c)
        return true;
    return false;
}

bool IsCommentChar(char c) {
    switch (c) {
        case COMMENT_CHAR:
            return true;
        default:
            return false;
    }
}

void Trim(string &str) {
    if (str.empty()) {
        return;
    }
    int i, start_pos, end_pos;
    for (i = 0; i < str.size(); ++i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    if (i == str.size()) { // 全部是空白字符串
        str = "";
        return;
    }

    start_pos = i;

    for (i = str.size() - 1; i >= 0; --i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    end_pos = i;

    str = str.substr(start_pos, end_pos - start_pos + 1);
}

bool AnalyseLine(const string &line, string &key, string &value) {
    if (line.empty())
        return false;
    int start_pos = 0, end_pos = line.size() - 1, pos;
    if ((pos = line.find(COMMENT_CHAR)) != -1) {
        if (0 == pos) {  // 行的第一个字符就是注释字符
            return false;
        }
        end_pos = pos - 1;
    }
    string new_line = line.substr(start_pos, start_pos + 1 - end_pos);  // 预处理，删除注释部分

    if ((pos = new_line.find('=')) == -1)
        return false;  // 没有=号

    key = new_line.substr(0, pos);
    value = new_line.substr(pos + 1, end_pos + 1 - (pos + 1));

    Trim(key);
    if (key.empty()) {
        return false;
    }
    Trim(value);
    return true;
}

bool GETSYM::ReadConfig(const string &filename, map<string, int> &m) {
    m.clear();
    ifstream infile(filename.c_str());
    if (!infile) {
        cout << "file open error" << endl;
        return false;
    }
    string line, key, value;
    while (getline(infile, line)) {
        if (AnalyseLine(line, key, value)) {
            m[key] = atoi(value.c_str());
        }
    }
    infile.close();
    return true;
}

void GETSYM::PrintConfig(const map<string, int> &m) {
    map<string, int>::const_iterator mite = m.begin();
    for (; mite != m.end(); ++mite) {
        cout << mite->first << "=" << mite->second << endl;
    }
}