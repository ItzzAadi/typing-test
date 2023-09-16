#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <vector>
#include <random>
#include <string>
#include <conio.h>
#include <sstream>
#include <iomanip> //for setprecision(n) function
#include <windows.h>    
//#include <locale>

using namespace std;


//ANSI Escape Codes
const string ANSI_RESET = "\033[0m";
const string ANSI_RED = "\033[31m";
const string ANSI_GREEN = "\033[32m";
const string ANSI_YELLOW = "\033[33m";
const string ANSI_BLUE = "\033[34m";
const string ANSI_BOLD = "\033[1m";
const string ANSI_UNDERLINE = "\033[4m";

vector<string> readWordsFromFile(const string& filename) {
    vector<string> words;
    ifstream file(filename);
    if (file.is_open()) {
        string word;
        while (file >> word) {
            words.push_back(word);
        }
        file.close();
    }
    return words;
}

string generateSentence(const vector<string>& words) {
    string sentence;
    unsigned seed = static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count());
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(0, words.size() - 1);
    int n = 20;
    //n refers to the numebr of words that we want
    for (int i = 0; i < n; ++i) {
        sentence += words[dist(rng)] + " ";
    }
    //sentence += "↵"; //↵ \u21B5
    return sentence;
}

string centerString(const string& input, int width, char fillChar = ' ') {
    if (width <= input.length()) {
        return input;
    }

    int visibleLength = 0;
    bool inEscapeCode = false;

    // Calculate the visible length of the input (excluding ANSI escape codes)
    for (char ch : input) {
        if (ch == '\033') {
            inEscapeCode = true;
        } else if (inEscapeCode && (ch < 'A' || ch > 'Z')) {
            inEscapeCode = false;
        } else if (!inEscapeCode) {
            visibleLength++;
        }
    }

    int padding = width - visibleLength;

    int leftPadding = padding / 2;
    int rightPadding = padding - leftPadding;

    return string(leftPadding, fillChar) + input + string(rightPadding, fillChar);
}


int main() {
    //using utf-8 lcoale
    //locale::global(locale("en_US.utf8"));

    //Creating a handle for setting text attribute
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Read words from file
    vector<string> words = readWordsFromFile("words.txt");

    // Generate a random sentence
    string sentence = generateSentence(words);
    string centeredSentence = centerString(sentence, 60);
    // Split the sentence into individual words
    vector<string> sentenceWords;
    string word;
    istringstream iss(sentence);
    while (iss >> word) {
        sentenceWords.push_back(word);
    }

    // Clear the screen
    system("cls");

    // Greet the user
    //SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    //FlushConsoleInputBuffer(hConsole);
    cout << ANSI_RESET << endl;
    string welcomeText = "Welcome to the Typing Test! Press Enter to start.\n";
    welcomeText = centerString(welcomeText, 120);
    cout << welcomeText << endl;
    while (_getch() != '\r') {}


    // Display the sentence
    string welcomeText2 = "Type the following sentence:";
    welcomeText2 = centerString(welcomeText2, 115); 
    cout << welcomeText2 << endl << endl;
    sentence = centerString(sentence, 125);
    cout << sentence << endl;

    // Start the timer
    auto startTime = chrono::system_clock::now();

    // Read user input
    string typedSentence;
    char ch;
    while ((ch = _getch()) != '\r') {
        cout << ch;
        typedSentence += ch;
    }
    cout << endl;

    // Split the typed sentence into individual words
    vector<string> typedWords;
    istringstream typedIss(typedSentence);
    while (typedIss >> word) {
        typedWords.push_back(word);
    }

    // Calculate the number of correct words
    int correctWords = 0;
    for (size_t i = 0; i < typedWords.size() && i < sentenceWords.size(); ++i) {
        if (typedWords[i] == sentenceWords[i]) {
            correctWords++;
        }
    }

    // Calculate the elapsed time and typing speed in words per minute (WPM)
    auto endTime = chrono::system_clock::now();
    chrono::duration<double> elapsedSeconds = endTime - startTime;
    double typingSpeed = (correctWords / elapsedSeconds.count()) * 60.0;

    // Calculate the accuracy percentage
    double accuracy = (static_cast<double>(correctWords) / sentenceWords.size()) * 100.0;

    // Display the results
    //cout << setprecision();
    // SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << ANSI_UNDERLINE << ANSI_RED << "Time elapsed: " << elapsedSeconds.count() << "s" << ANSI_RESET << endl;
    // SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << ANSI_BLUE << "Typing speed: " << typingSpeed << " words per minute" << ANSI_RESET << endl;
    // SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);

    cout << ANSI_GREEN << "Accuracy: " << accuracy << "%" << ANSI_RESET << endl;
    // SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    
    //_getch();
    return 0;
}
