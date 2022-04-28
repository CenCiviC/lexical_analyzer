#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <cstdarg>

using namespace std;
ofstream file_output("output.txt"); //output file

class dfa {
public:
    int priority_rank;// -를 다루기 위해서 dfa set에 정렬하기 위한 순서
    string dfaName;

    unsigned int curState = 0;
    unsigned int startState = 0;
    bool flag = true; // 현재 dfa가 성립하는지 알기 위한 flag
    vector<char> lexemes; //각 dfa당 저장되는 현재 lexeme

    map<unsigned int, bool> states; //state를 표현하기 위한 state 번호와, final state인지 표현하기 위한 bool 변수
    map<pair<unsigned int, char>, unsigned int> transitionMap; //state 번호와, input을 확인하고 다음 state 번호로 넘어가기 위한 변수

public:
    dfa() {
        //생성자
    }
    //       우선순위,  dfa 이름,                 state 개수 , final state의 번호
    dfa(int priority, string name, unsigned int stateAmount, ...) {
        priority_rank = priority;
        dfaName = name;
        states.insert(make_pair(0, false));

        //가변인자를 활용한 dfa 생성과정
        for (unsigned int i = 1; i < stateAmount; i++) {
            bool _final = true;

            va_list ap;
            va_start(ap, stateAmount);

            for (int j = 0; j != stateAmount; ++j) {
                if (i == va_arg(ap, unsigned int)) {
                    states.insert(make_pair(i, true));
                    _final = false;
                    break;
                }
            }//final state일 경우
            va_end(ap);
            if (_final) states.insert(make_pair(i, false)); //final state가 아닌 경우
        }

    }// 한 dfa 생성과정

    void addTransition(unsigned int inputState, char ch, unsigned int outputState) {
        transitionMap.insert({ make_pair(inputState, ch), outputState });
    }//dfa에 transition을 추가하는 함수

    void transition(unsigned int inputState, char ch) {
        if (transitionMap.find(make_pair(inputState, ch)) != transitionMap.end()) {
            curState = transitionMap.find(make_pair(inputState, ch))->second;// curstate변경
            lexemes.push_back(ch);//lexemes 생성
        }
        else {
            flag = false;
        }
    }//dfa에 있는 transition를 진행하는 함수


    unsigned int getCurState() {
        return curState;
    }

    bool acceptDfa() {
        return flag;
    }

    void reset() {
        curState = 0;
        flag = true;
        lexemes.clear();
    }//dfa의 변수를 초기화

    void makeLexemes() {
        cout << dfaName << " ";
        for (char ch : lexemes) {
            cout << ch;
        }
        cout << endl;

        file_output << dfaName << endl;
        for (char ch : lexemes) {
            file_output << ch;
        }
        file_output << '\n' << endl;
    }//dfa에 들어있는 lexeme 출력

    bool checkDfa(char ch) {
        transition(curState, ch);
        return acceptDfa();
    }

    bool finalState() {
        return (*states.find(getCurState())).second;
    }

    bool operator <(const dfa& compare) const {
        bool result = priority_rank < compare.priority_rank;

        return result;
    }//우선순위에 따라 set 정리

    void setFlag() {
        flag = false;
    }
};

//각 dfa 생성
dfa vartypeDfa(1, "VARTYPE", 12, 3);
dfa* ptrVartypeDfa = &vartypeDfa;
dfa keywordDfa(2, "KEYWORD", 35, 2, 4, 8, 12, 17, 22, 28, 34);
dfa* ptrKeywordDfa = &keywordDfa;
dfa arithmeticDfa(5, "ARITHMETIC", 2, 1);
dfa* ptrArithmeticDfa = &arithmeticDfa;
dfa assignmentDfa(4, "ASSIGNMENT", 2, 1);
dfa* ptrAssignmentDfa = &assignmentDfa;
dfa comparisonDfa(6, "COMPARISON", 6, 1, 2, 3);
dfa* ptrComparisonDfa = &comparisonDfa;
dfa identifierDfa(9, "IDENTIFIER", 2, 1);
dfa* ptrIdentifierDfa = &identifierDfa;
dfa integerDfa(8, "INTEGER", 4, 2, 3);
dfa* ptrIntegerDfa = &integerDfa;
dfa stringDfa(7, "STRING", 4, 3);
dfa* ptrStringDfa = &stringDfa;
dfa whitespaceDfa(3, "WHITESPACE", 2, 1);
dfa* ptrWhiteSpaceDfa = &whitespaceDfa;
dfa lparenDfa(10, "LPAREN", 2, 1);
dfa* ptrLparenDfa = &lparenDfa;
dfa rparenDfa(11, "RPAREN", 2, 1);
dfa* ptrRparenDfa = &rparenDfa;
dfa lbracketDfa(12, "LBRACKET", 2, 1);
dfa* ptrLbracketDfa = &lbracketDfa;
dfa rbracketDfa(13, "RBRACKET", 2, 1);
dfa* ptrRbracketDfa = &rbracketDfa;
dfa commaDfa(14, "COMMA", 2, 1);
dfa* ptrCommaDfa = &commaDfa;
dfa semicolonDfa(15, "SEMICOLON", 2, 1);
dfa* ptrSemicolonDfa = &semicolonDfa;


//set 순서 정의
struct orderPriority {
    bool operator() (const dfa* left, const dfa* right) const
    {
        return left->priority_rank < right->priority_rank;
    }
};

//error handling을 위한 위치 변수
unsigned int column = 1;
unsigned int line = 1;

//transition을 성립하는 현재 dfa
dfa* curDfa;

//transition을 성립하는 현재 dfa의 후보들
set<dfa*, orderPriority> curDfaCandidate;

//모든 dfa 정리
set<dfa*, orderPriority> collectionDfa;

void constructDfa() {
    //vartype dfa
    ptrVartypeDfa->addTransition(0, 'i', 1);
    ptrVartypeDfa->addTransition(1, 'n', 2);
    ptrVartypeDfa->addTransition(2, 't', 3);
    ptrVartypeDfa->addTransition(0, 'I', 4);
    ptrVartypeDfa->addTransition(4, 'N', 5);
    ptrVartypeDfa->addTransition(5, 'T', 3);
    ptrVartypeDfa->addTransition(0, 'c', 6);
    ptrVartypeDfa->addTransition(6, 'h', 7);
    ptrVartypeDfa->addTransition(7, 'a', 8);
    ptrVartypeDfa->addTransition(8, 'r', 3);
    ptrVartypeDfa->addTransition(0, 'C', 9);
    ptrVartypeDfa->addTransition(9, 'H', 10);
    ptrVartypeDfa->addTransition(10, 'A', 11);
    ptrVartypeDfa->addTransition(11, 'R', 3);


    //keyword dfa
    ptrKeywordDfa->addTransition(0, 'i', 1);
    ptrKeywordDfa->addTransition(0, 'I', 3);
    ptrKeywordDfa->addTransition(0, 'e', 5);
    ptrKeywordDfa->addTransition(0, 'E', 9);
    ptrKeywordDfa->addTransition(0, 'w', 13);
    ptrKeywordDfa->addTransition(0, 'W', 18);
    ptrKeywordDfa->addTransition(0, 'r', 23);
    ptrKeywordDfa->addTransition(0, 'R', 29);
    ptrKeywordDfa->addTransition(1, 'f', 2);
    ptrKeywordDfa->addTransition(3, 'F', 4);
    ptrKeywordDfa->addTransition(5, 'l', 6);
    ptrKeywordDfa->addTransition(6, 's', 7);
    ptrKeywordDfa->addTransition(7, 'e', 8);
    ptrKeywordDfa->addTransition(9, 'L', 10);
    ptrKeywordDfa->addTransition(10, 'S', 11);
    ptrKeywordDfa->addTransition(11, 'E', 12);
    ptrKeywordDfa->addTransition(13, 'h', 14);
    ptrKeywordDfa->addTransition(14, 'i', 15);
    ptrKeywordDfa->addTransition(15, 'l', 16);
    ptrKeywordDfa->addTransition(16, 'e', 17);
    ptrKeywordDfa->addTransition(18, 'H', 19);
    ptrKeywordDfa->addTransition(19, 'I', 20);
    ptrKeywordDfa->addTransition(20, 'L', 21);
    ptrKeywordDfa->addTransition(21, 'E', 22);
    ptrKeywordDfa->addTransition(23, 'e', 24);
    ptrKeywordDfa->addTransition(24, 't', 25);
    ptrKeywordDfa->addTransition(25, 'u', 26);
    ptrKeywordDfa->addTransition(26, 'r', 27);
    ptrKeywordDfa->addTransition(27, 'n', 28);
    ptrKeywordDfa->addTransition(29, 'E', 30);
    ptrKeywordDfa->addTransition(30, 'T', 31);
    ptrKeywordDfa->addTransition(31, 'U', 32);
    ptrKeywordDfa->addTransition(32, 'R', 33);
    ptrKeywordDfa->addTransition(33, 'N', 34);

    //arithmetic
    ptrArithmeticDfa->addTransition(0, '+', 1);
    ptrArithmeticDfa->addTransition(0, '-', 1);
    ptrArithmeticDfa->addTransition(0, '*', 1);
    ptrArithmeticDfa->addTransition(0, '/', 1);

    //identifier

    for (char ch = 'A'; ch <= 'Z'; ch++) {
        ptrIdentifierDfa->addTransition(0, ch, 1);
    }
    for (char ch = 'a'; ch <= 'z'; ch++) {
        ptrIdentifierDfa->addTransition(0, ch, 1);
    }

    for (char ch = 'A'; ch <= 'Z'; ch++) {
        ptrIdentifierDfa->addTransition(1, ch, 1);
    }
    for (char ch = 'a'; ch <= 'z'; ch++) {
        ptrIdentifierDfa->addTransition(1, ch, 1);
    }
    for (char i = '0'; i <= '9'; i++) {
        ptrIdentifierDfa->addTransition(1, i, 1);
    }

    //string
    ptrStringDfa->addTransition(0, '\"', 1);
    //letter, digit, blank
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        ptrStringDfa->addTransition(1, ch, 2);
    }
    for (char ch = 'a'; ch <= 'z'; ch++) {
        ptrStringDfa->addTransition(1, ch, 2);
    }
    for (char i = '0'; i <= '9'; i++) {
        ptrStringDfa->addTransition(1, i, 2);
    }
    ptrStringDfa->addTransition(1, ' ', 2);
    //letter, digit, blank
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        ptrStringDfa->addTransition(2, ch, 2);
    }
    for (char ch = 'a'; ch <= 'z'; ch++) {
        ptrStringDfa->addTransition(2, ch, 2);
    }
    for (char i = '0'; i <= '9'; i++) {
        ptrStringDfa->addTransition(2, i, 2);
    }
    ptrStringDfa->addTransition(2, ' ', 2);
    //
    ptrStringDfa->addTransition(2, '\"', 3);


    //integer
    ptrIntegerDfa->addTransition(0, '0', 3);
    ptrIntegerDfa->addTransition(0, '-', 1);
    for (char i = '0'; i <= '9'; i++) {
        ptrIntegerDfa->addTransition(2, i, 2);
    }
    for (char i = '1'; i <= '9'; i++) {
        ptrIntegerDfa->addTransition(0, i, 2);
    }

    for (char i = '1'; i <= '9'; i++) {
        ptrIntegerDfa->addTransition(1, i, 2);
    }

    //assignment
    ptrAssignmentDfa->addTransition(0, '=', 1);

    //comparision
    ptrComparisonDfa->addTransition(0, '<', 1);
    ptrComparisonDfa->addTransition(0, '>', 3);
    ptrComparisonDfa->addTransition(0, '=', 4);
    ptrComparisonDfa->addTransition(0, '!', 5);
    ptrComparisonDfa->addTransition(1, '=', 2);
    ptrComparisonDfa->addTransition(3, '=', 2);
    ptrComparisonDfa->addTransition(4, '=', 2);
    ptrComparisonDfa->addTransition(5, '=', 2);

    //lparen
    ptrLparenDfa->addTransition(0, '(', 1);

    //rparen
    ptrRparenDfa->addTransition(0, ')', 1);

    //lbracket
    ptrLbracketDfa->addTransition(0, '{', 1);

    //rbracket
    ptrRbracketDfa->addTransition(0, '}', 1);

    //comma
    ptrCommaDfa->addTransition(0, ',', 1);

    //semicolon
    ptrSemicolonDfa->addTransition(0, ';', 1);



    //whitespace
    ptrWhiteSpaceDfa->addTransition(0, '\t', 1);
    ptrWhiteSpaceDfa->addTransition(0, '\n', 1);
    ptrWhiteSpaceDfa->addTransition(0, ' ', 1);
    ptrWhiteSpaceDfa->addTransition(1, ' ', 1);
    ptrWhiteSpaceDfa->addTransition(1, '\t', 1);
    ptrWhiteSpaceDfa->addTransition(1, '\n', 1);

    //dfa set
    collectionDfa.insert(ptrVartypeDfa);
    collectionDfa.insert(ptrKeywordDfa);
    collectionDfa.insert(ptrArithmeticDfa);
    collectionDfa.insert(ptrAssignmentDfa);
    collectionDfa.insert(ptrComparisonDfa);
    collectionDfa.insert(ptrIdentifierDfa);
    collectionDfa.insert(ptrIntegerDfa);
    collectionDfa.insert(ptrStringDfa);
    collectionDfa.insert(ptrWhiteSpaceDfa);
    collectionDfa.insert(ptrLparenDfa);
    collectionDfa.insert(ptrRparenDfa);
    collectionDfa.insert(ptrLbracketDfa);
    collectionDfa.insert(ptrRbracketDfa);
    collectionDfa.insert(ptrCommaDfa);
    collectionDfa.insert(ptrSemicolonDfa);

}

//들어온 input이 정의된 input 중에 있는지 확인
void check_invalid_value(char input_char) {
    bool invalidValue = true;

    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (input_char == ch) invalidValue = false;
    }
    for (char ch = 'a'; ch <= 'z'; ch++) {
        if (input_char == ch) invalidValue = false;
    }
    for (char ch = '0'; ch <= '9'; ch++) {
        if (input_char == ch) invalidValue = false;
    }
    if (input_char == '-' || input_char == '\"' || input_char == '+' || input_char == '*' || input_char == '/' ||
        input_char == '=' || input_char == '<' || input_char == '>' || input_char == '!' || input_char == ';' ||
        input_char == '{' || input_char == '}' || input_char == '(' || input_char == ')' || input_char == ',' ||
        input_char == '\t' || input_char == '\n' || input_char == ' ') {
        invalidValue = false;
    }

    if (invalidValue) {
        cout << "input value is invalid value" << "\nline :" << line << "   column :" << column;
        exit(0);
    }
}


//input이 들어왔을 때 실제 진행되는 함수
void scanner(char input_char) {
    check_invalid_value(input_char);

    for (dfa* dfa : collectionDfa) {
        if (dfa->checkDfa(input_char)) {
            curDfaCandidate.insert(dfa);
        }
        else dfa->setFlag();
    }

    switch (curDfaCandidate.size())
    {
    case 0://제대로 됐을 때(token의 종류가 바뀔 때) 혹은 성립하는 dfa가 없을 때
        if (curDfa->finalState()) {
            curDfa->makeLexemes();
            for (dfa* dfa : collectionDfa) {
                dfa->reset();
            }
            scanner(input_char);
        }

        else {
            cout << "error! dfa doesn't exist" << "\nline :" << line << " column :" << column; // 만족하는 dfa가 존재하지 않는 경우
            exit(0);
        }
        break;
    case 1: // lexemes 진행 중일때
        if (*curDfaCandidate.begin() == ptrWhiteSpaceDfa) {
            if (input_char == ' ') column++;
            else if (input_char == '\t') column += 4;
            else if (input_char == '\n') { line += 1; column = 1; }

            for (dfa* dfa : collectionDfa) {
                dfa->reset();
            }
            break;
        }
        column += 1;
        curDfa = *curDfaCandidate.begin();
        break;

        // 만족하는 dfa가 여러 개일 때      
    default:
        column += 1;
        if (input_char == '-') {
            if (*curDfaCandidate.begin() == ptrArithmeticDfa && *curDfaCandidate.rbegin() == ptrIntegerDfa) {
                if (curDfa == ptrIdentifierDfa || curDfa == ptrIntegerDfa) {
                    curDfa = ptrArithmeticDfa;
                    ptrIntegerDfa->setFlag();
                    break;
                }
            }
        }// - handling
        curDfa = *curDfaCandidate.begin();// 여러 개 중에서 우선순위가 높은 dfa 선택
        break;
    }
    curDfaCandidate.clear();
}

int main(int argc, char* argv[]) {
    string fileName(argv[1]);
    ifstream readFile(fileName);

    if (readFile.fail()) {
        cout << "Can't make output file" << endl;
        exit(0);
    }

    char input_char;
    constructDfa();
    while (readFile.get(input_char)) {
        //dfa 확인
        scanner(input_char);
    }
    if (curDfa != nullptr) {
        if (curDfa->finalState()) curDfa->makeLexemes();
    }

    readFile.close();
    file_output.close();
}