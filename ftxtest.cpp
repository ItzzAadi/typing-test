#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;
using namespace std;

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
    for (int i = 0; i < n; ++i) {
        sentence += words[dist(rng)] + " ";
    }
    sentence += "↵"; // Unicode for "↵"
    return sentence;
}

int main(int argc, const char* argv[]) {
    // Initialize the ftxui screen.
    Screen screen;
    Elements elements;

    // Read words from file.
    vector<string> words = readWordsFromFile("words.txt");

    // Generate a random sentence.
    string sentence = generateSentence(words);

    // Greet the user.
    elements.push_back(text(L"Welcome to the Typing Test! Press Enter to start."));

    // Wait for Enter key press.
    char key;
    while (screen.Loop(1000 / 60)) {
        if (key == 'q')
            return 0;
        if (key == '\n')
            break;
    }

    // Display the sentence.
    elements.push_back(text(L"Type the following sentence:"));
    elements.push_back(text(sentence));

    // Start the timer.
    auto start_time = std::chrono::steady_clock::now();

    // Read user input.
    wstring typed_sentence;
    int correct_words = 0;
    int typed_word_index = 0;

    while (typed_word_index < words.size()) {
        auto elapsed_time = std::chrono::steady_clock::now() - start_time;
        float elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count() / 1000.0f;

        int typed_sentence_length = typed_sentence.length();
        bool is_typing = typed_sentence_length < sentence.length() && sentence[typed_sentence_length] != L' ';
        wchar_t typed_char;

        screen.Render([&](ftxui::Screen& screen) {
            auto container = vbox({});
            container->Add(text(L"Time elapsed: " + to_wstring(elapsed_seconds) + L"s"));
            container->Add(text(L"Type the following sentence:"));
            container->Add(text(sentence));

            if (is_typing) {
                typed_char = typed_sentence[typed_sentence_length];
                if (typed_char == sentence[typed_sentence_length]) {
                    correct_words++;
                    typed_word_index++;
                }
                typed_sentence_length++;
                container->Add(text(typed_sentence.substr(0, typed_sentence_length)));
                container->Add(text(typed_sentence.substr(typed_sentence_length)));
            } else {
                container->Add(text(L"Press Enter to finish."));
            }

            return container;
        });

        // Process keyboard input.
        while (screen.PollEvent(&key)) {
            if (key == 'q')
                return 0;
            if (key == '\n') {
                // Finish typing.
                while (screen.PollEvent(&key)) {}
                break;
            }

            if (is_typing) {
                typed_sentence += key;
            }
        }

        // Update the screen.
        screen.PostLoop();
    }

    // Calculate typing speed and accuracy.
    auto end_time = std::chrono::steady_clock::now();
    float elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() / 1000.0f;
    float typing_speed = static_cast<float>(correct_words) / elapsed_seconds * 60.0f;
    float accuracy = static_cast<float>(correct_words) / words.size() * 100.0f;

    // Display results.
    elements.push_back(text(L"Time elapsed: " + to_wstring(elapsed_seconds) + L"s"));
    elements.push_back(text(L"Typing speed: " + to_wstring(typing_speed) + L" words per minute"));
    elements.push_back(text(L"Accuracy: " + to_wstring(accuracy) + L"%"));

    // Wait for Enter key press to exit.
    while (screen.Loop(1000 / 60)) {
        if (key == 'q' || key == '\n')
            break;
    }

    return 0;
}
