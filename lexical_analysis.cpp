#include <iostream>
#include <fstream>
#include <string>
using namespace std;


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