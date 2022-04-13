#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <cstdarg>

using namespace std;
ofstream fout("output.txt");

class dfa {
public:
    int priority_rank;
    string dfaName;
    bool exist = false;
    bool halt = false;
    unsigned int curState = 0;
    unsigned int startState = 0;
    bool flag = false;
    vector<char> leximes;

    map<unsigned int, bool> states;
    map<pair<unsigned int, char>, unsigned int> transitionMap;

public:
    dfa() {

    }

    dfa(int priority, string name, unsigned int stateAmount, ...) {
        priority_rank = priority;
        exist = true;
        dfaName = name;
        bool exist = true;
        states.insert(make_pair(0, false));


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
            }
            va_end(ap);
            if (_final) states.insert(make_pair(i, false));
        }

    }

    void addTransition(unsigned int inputState, char ch, unsigned int outputState) {
        transitionMap.insert({ make_pair(inputState, ch), outputState });
    }

    void transition(unsigned int inputState, char ch) {


        if (transitionMap.find(make_pair(inputState, ch)) != transitionMap.end() && !halt) {
            flag = true;
            curState = transitionMap.find(make_pair(inputState, ch))->second;// curstate변경
            leximes.push_back(ch);//leximes 생성
        }
        else {
            flag = false;
            halt = true;
        }


    }


    unsigned int getCurState() {
        return curState;
    }

    bool acceptDfa() {
        return flag;
    }

    void reset() {
        curState = 0;
        flag = false;
        halt = false;
        leximes.clear();
    }

    void makeLeximes() {
        cout << dfaName << " ";
        for (char ch : leximes) {
            cout << ch;
        }
        cout << endl;

        fout << dfaName << endl;
        for (char ch : leximes) {
            fout << ch;
        }
        fout << '\n'<<endl;
    }

    bool checkDfa(char ch) {
        transition(curState, ch);
        return acceptDfa();
    }

    bool existDfa() {
        return exist;
    }

    bool finalState() {
        return (*states.find(getCurState())).second;
    }
    void halting() {
        halt = true;
    }
    bool operator <(const dfa& compare) const {
        bool result = priority_rank < compare.priority_rank;

        return result;
    }

};





dfa keywordDfa(1, "KEYWORD", 35, 2, 4, 8, 12, 17, 22, 28, 34);
dfa* ptrKeywordDfa = &keywordDfa;
dfa arithmeticDfa(5, "ARITHMETIC", 2, 1);
dfa* ptrArithmeticDfa = &arithmeticDfa;
dfa assignmentDfa(4, "ASSIGNMENT", 2, 1);
dfa* ptrAssignmentDfa = &assignmentDfa;
dfa comparisonDfa(6, "COMPARISON", 6, 1, 2, 3);
dfa* ptrComparisonDfa = &comparisonDfa;
dfa identifierDfa(9, "IDENTIFIER", 2, 1);
dfa* ptrIdentifierDfa = &identifierDfa;
dfa integerDfa(8, "INTEGER", 6, 2, 3, 4, 5);
dfa* ptrIntegerDfa = &integerDfa;
dfa stringDfa(7, "STRING", 4, 3);
dfa* ptrStringDfa = &stringDfa;
dfa punctunationDfa(2, "PUNCTUNATION", 2, 1);
dfa* ptrPunctunationDfa = &punctunationDfa;
dfa whitespaceDfa(3, "WHITESPACE", 2, 1);
dfa* ptrWhiteSpaceDfa = &whitespaceDfa;


struct orderPriority{
    bool operator() (const dfa* left, const dfa* right) const
    {
        return left->priority_rank < right->priority_rank;
    }
};

unsigned int column = 0;
unsigned int line = 0;
dfa* curDfa;
set<dfa*, orderPriority> curDfaCandidate;
set<dfa*, orderPriority> collectionDfa;

void constructDfa() {
    //keyword
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
    ptrIntegerDfa->addTransition(0, '0', 5);
    ptrIntegerDfa->addTransition(0, '-', 1);
    for (char i = '1'; i <= '9'; i++) {
        ptrIntegerDfa->addTransition(0, i, 4);
    }
    for (char i = '1'; i <= '9'; i++) {
        ptrIntegerDfa->addTransition(1, i, 2);
    }
    for (char i = '0'; i <= '9'; i++) {
        ptrIntegerDfa->addTransition(2, i, 3);
    }
    for (char i = '0'; i <= '9'; i++) {
        ptrIntegerDfa->addTransition(3, i, 3);
    }
    for (char i = '0'; i <= '9'; i++) {
        ptrIntegerDfa->addTransition(4, i, 3);
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

    //punctunation
    ptrPunctunationDfa->addTransition(0, '(', 1);
    ptrPunctunationDfa->addTransition(0, ')', 1);
    ptrPunctunationDfa->addTransition(0, '{', 1);
    ptrPunctunationDfa->addTransition(0, '}', 1);
    ptrPunctunationDfa->addTransition(0, ',', 1);


    //whitespace
    ptrWhiteSpaceDfa->addTransition(0, '\t', 1);
    ptrWhiteSpaceDfa->addTransition(0, '\n', 1);
    ptrWhiteSpaceDfa->addTransition(0, ' ', 1);
    ptrWhiteSpaceDfa->addTransition(1, ' ', 1);
    ptrWhiteSpaceDfa->addTransition(1, '\t', 1);
    ptrWhiteSpaceDfa->addTransition(1, '\n', 1);


    collectionDfa.insert(ptrKeywordDfa);
    collectionDfa.insert(ptrArithmeticDfa);
    collectionDfa.insert(ptrAssignmentDfa);
    collectionDfa.insert(ptrComparisonDfa);
    collectionDfa.insert(ptrIdentifierDfa);
    collectionDfa.insert(ptrIntegerDfa);
    collectionDfa.insert(ptrStringDfa);
    collectionDfa.insert(ptrPunctunationDfa);
    collectionDfa.insert(ptrWhiteSpaceDfa);
}

void parsing(char input_char) {

    for (dfa* dfa : collectionDfa) {
        if (dfa->checkDfa(input_char)) {
            curDfaCandidate.insert(dfa);
        }
    }

    switch (curDfaCandidate.size())
    {
    case 0://제대로 됐을 때 , 마지막 leximes이 바뀔
        if (curDfa->finalState()) {
            curDfa->makeLeximes();
            for (dfa* dfa : collectionDfa) {
                dfa->reset();
            }
            parsing(input_char);
        }
        
        else {
            cout << "error! dfa doesn't exist";
            exit(0);
        }
        break;
    case 1: // leximes 진행 중일때
        if (*curDfaCandidate.begin() == ptrWhiteSpaceDfa) {
            if(input_char == ' ') column++;
            else if(input_char == '\t') column+=4;
            else if(input_char == '\n'){line+=1; column =0;}
            
            for (dfa* dfa : collectionDfa) {
                dfa->reset();
            }
            break;
        }
        curDfa = *curDfaCandidate.begin();
        break;

    case 2: // 여러 상황일 때
        if (*curDfaCandidate.begin() == ptrArithmeticDfa && *curDfaCandidate.rbegin() == ptrIntegerDfa) {
            if (!(curDfa == ptrIdentifierDfa || curDfa == ptrIntegerDfa || curDfa == ptrStringDfa)) {
                curDfa = ptrIntegerDfa;
                ptrArithmeticDfa->halting();
                break;
            }
            else {
                curDfa = ptrArithmeticDfa;
                ptrIntegerDfa->halting();
                break;
            }
        }//- handling
        curDfa = *curDfaCandidate.begin();// 둘중 우선순위가 높은 assignemnt
        break;
    default:
        cout << "set of candidate is wrong";
        break;
    }

    curDfaCandidate.clear();

}

int main(int argc, char* argv[]) {
    string fileName(argv[1]);
    ifstream readFile(fileName);

    if (readFile.fail()) {
        cout << "file does not exist" << endl;
        exit(0);
    }

    char input_char;
    constructDfa();
    while (readFile.get(input_char)) {
        //dfa 확인
        parsing(input_char);
    }
    if (curDfa != nullptr) {
        if (curDfa->finalState()) curDfa->makeLeximes();
    }

    readFile.close();
    fout.close();
}