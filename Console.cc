#include "Console.hh"
#include <stdlib.h>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
void cls(HANDLE hConsole) {
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, (TCHAR) ' ',
        dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
        dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
    return;
}
#endif

using namespace std;

Screen Console::screen;

Screen* Console::GetScreen() {
    return &Console::screen;
}

void Console::Clear(char value, Color foreground, Color background) {
#ifdef _WIN32
    //system("cls");
    cls(GetStdHandle(STD_OUTPUT_HANDLE));
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
#ifdef _WIN32
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screenInfo;
    GetConsoleScreenBufferInfo(hStdout, &screenInfo);
    WORD oldAttributes = screenInfo.wAttributes;
    WORD foregroundMask = FOREGROUND_INTENSITY | FOREGROUND_RED |
        FOREGROUND_GREEN | FOREGROUND_BLUE;
    WORD backgroundMask = BACKGROUND_INTENSITY | BACKGROUND_RED |
        BACKGROUND_GREEN | BACKGROUND_BLUE;
#else
    string foregroundBegin;
    string backgroundBegin;
    string foregroundEnd;
    string backgroundEnd;
#endif
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
#ifdef _WIN32
            WORD foreground, background;
            switch (screen.GetColor(i, j, false)) {
            default: case NONE:
                foreground = oldAttributes & foregroundMask;
                break;
            case WHITE:
                foreground = foregroundMask;
                break;
            case GREY:
                foreground = foregroundMask & ~FOREGROUND_INTENSITY;
                break;
            case BLACK:
                foreground = 0;
                break;
            case BLUE:
                foreground = FOREGROUND_INTENSITY | FOREGROUND_BLUE;
                break;
            case CYAN:
                foreground = foregroundMask & ~FOREGROUND_RED;
                break;
            case GREEN:
                foreground = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
                break;
            case MAGENTA:
                foreground = foregroundMask & ~FOREGROUND_GREEN;
                break;
            case RED:
                foreground = FOREGROUND_INTENSITY | FOREGROUND_RED;
                break;
            case YELLOW:
                foreground = foregroundMask & ~FOREGROUND_BLUE;
                break;
            }
            switch (screen.GetColor(i, j, true)) {
            default: case NONE:
                background = oldAttributes & backgroundMask;
                break;
            case WHITE:
                background = backgroundMask;
                break;
            case GREY:
                background = backgroundMask & ~BACKGROUND_INTENSITY;
                break;
            case BLACK:
                background = 0;
                break;
            case BLUE:
                background = BACKGROUND_INTENSITY | BACKGROUND_BLUE;
                break;
            case CYAN:
                background = backgroundMask & ~BACKGROUND_RED;
                break;
            case GREEN:
                background = BACKGROUND_INTENSITY | BACKGROUND_GREEN;
                break;
            case MAGENTA:
                background = backgroundMask & ~BACKGROUND_GREEN;
                break;
            case RED:
                background = BACKGROUND_INTENSITY | BACKGROUND_RED;
                break;
            case YELLOW:
                background = backgroundMask & ~BACKGROUND_BLUE;
                break;
            }
            SetConsoleTextAttribute(hStdout, foreground | background);
            std::cout << screen.GetChar(i, j);
            SetConsoleTextAttribute(hStdout, oldAttributes);
#else
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
#endif
        }
        cout << endl;
    }
}
