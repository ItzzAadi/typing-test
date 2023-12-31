#include <iostream>
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
using namespace ftxui;
int main(void){
    Element doc = 
        hbox({
            text("Textbox 1") | border,
            text("Textbox 2") | border | flex,
            text("Textbox 3") | border,
        });
    auto screen = Screen::Create(
        Dimension::Full(),
        Dimension::Fit(doc)
    );
    Render(screen, doc);
    screen.Print();

    return EXIT_SUCCESS;
}