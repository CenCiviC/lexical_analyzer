#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

class dfa{
private:
    unsigned int finalState = -1;
    unsigned int curState = 0;
    unsigned int startState = 0;
    bool flag = false;
    vector<char> leximes;
    set<unsigned int> states;
    map<pair<unsigned int, char>, unsigned int> transitionMap;

public:
    void addState(unsigned int stateNumber){
        states.insert(stateNumber);
    }

    void addTransition(unsigned int inputState, char ch, unsigned int outputState){
        transitionMap.insert(make_pair(inputState, ch), outputState);
    }

    void transition(unsigned int inputState, char ch){
        if(flag) return;
        else{
            
            
            
            //trainsition에 대한 내용=
        }
    }


    unsigned int curState(){
        return curState;
    }

    bool acceptDfa(){
        return flag;
    }

    void reset(){
        curState = 0;
    }

    pair<string, string> makeLeximes(){
        //override dfa에 따라 string 내용이 다름
    }

    bool checkDfa(char ch){
        transition(curState, ch);
        return acceptDfa;
    }
    
}


struct Dfa{
    //각 dfa 모음
};








int main(int argc, char* argv[]) {    
    string fileName(argv[1]);
    ifstream readFile(fileName);

    if (readFile.fail()) {
        cout << "file does not exist" << endl;
        exit(0);
    }
    
    char input_char;
    while (readFile.get(input_char)) {
        cout << input_char;
    }
    readFile.close();

}