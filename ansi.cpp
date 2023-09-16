#include <iostream>

using namespace std;

const string ANSI_RESET = "\033[0m";
const string ANSI_RED = "\033[31m";
const string ANSI_GREEN = "\033[32m";
const string ANSI_YELLOW = "\033[33m";
const string ANSI_BLUE = "\u001b[34m";
const string ANSI_BOLD = "\033[1m";
const string ANSI_UNDERLINE = "\033[4m";

int main(){
    cout << ANSI_BLUE << "Hello world" << ANSI_RESET;
    return 0;
}