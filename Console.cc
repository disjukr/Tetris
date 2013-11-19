#include "Console.hh"
#include <stdlib.h>
#include <iostream>

using namespace std;

Screen Console::screen;

Screen* Console::GetScreen() {
    return &Console::screen;
}

void Console::Clear(char value, Color foreground, Color background) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    screen.Clear(value, foreground, background);
}

void Console::Clear(char value, Color background) {
    Clear(value, NONE, background);
}

void Console::Clear(Color background) {
    Clear(' ', NONE, background);
}

void Console::Clear(char value) {
    Clear(value, NONE, NONE);
}

void Console::Clear() {
    Clear(' ', NONE, NONE);
}

void Console::ShowScreen() {
    int width = screen.GetWidth();
    int height = screen.GetHeight();
    string foregroundBegin;
    string backgroundBegin;
    string foregroundEnd;
    string backgroundEnd;
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            foregroundEnd = "\x1B[39m";
            backgroundEnd = "\x1B[49m";
            switch (screen.GetColor(i, j, false)) {
            default: case NONE:
                foregroundBegin = "";
                foregroundEnd = "";
                break;
            case WHITE: foregroundBegin = "\x1B[37m"; break;
            case GREY: foregroundBegin = "\x1B[90m"; break;
            case BLACK: foregroundBegin = "\x1B[30m"; break;
            case BLUE: foregroundBegin = "\x1B[34m"; break;
            case CYAN: foregroundBegin = "\x1B[36m"; break;
            case GREEN: foregroundBegin = "\x1B[32m"; break;
            case MAGENTA: foregroundBegin = "\x1B[35m"; break;
            case RED: foregroundBegin = "\x1B[31m"; break;
            case YELLOW: foregroundBegin = "\x1B[33m"; break;
            }
            switch (screen.GetColor(i, j, true)) {
            default: case NONE:
                backgroundBegin = "";
                backgroundEnd = "";
                break;
            case WHITE: backgroundBegin = "\x1B[47m"; break;
            case GREY: backgroundBegin = "\x1B[49;5;8m"; break;
            case BLACK: backgroundBegin = "\x1B[40m"; break;
            case BLUE: backgroundBegin = "\x1B[44m"; break;
            case CYAN: backgroundBegin = "\x1B[46m"; break;
            case GREEN: backgroundBegin = "\x1B[42m"; break;
            case MAGENTA: backgroundBegin = "\x1B[45m"; break;
            case RED: backgroundBegin = "\x1B[41m"; break;
            case YELLOW: backgroundBegin = "\x1B[43m"; break;
            }
            cout << foregroundBegin << backgroundBegin
                << screen.GetChar(i, j)
                << foregroundEnd << backgroundEnd;
        }
        cout << endl;
    }
}
