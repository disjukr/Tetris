#include "Console.hh"
#include <stdlib.h>
#include <iostream>

using namespace std;

Screen Console::screen;
Screen Console::prevScreen;

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

HANDLE hStdout;
WORD oldAttributes;
CONSOLE_SCREEN_BUFFER_INFO screenInfo;
WORD foregroundMask = FOREGROUND_INTENSITY | FOREGROUND_RED |
    FOREGROUND_GREEN | FOREGROUND_BLUE;
WORD backgroundMask = BACKGROUND_INTENSITY | BACKGROUND_RED |
    BACKGROUND_GREEN | BACKGROUND_BLUE;

WORD GetMaskedAttributes(Color color, WORD mask, WORD intensity,
                         WORD red, WORD green, WORD blue) {
    switch (color) {
    default: case NONE: return oldAttributes & mask;
    case WHITE: return mask;
    case GREY: return mask & ~intensity;
    case BLACK: return 0;
    case BLUE: return intensity | blue;
    case CYAN: return mask & ~red;
    case GREEN: return intensity | green;
    case MAGENTA: return mask & ~green;
    case RED: return intensity | red;
    case YELLOW: return mask & ~blue;
    }
}

WORD GetForegroundAttributes(Color color) {
    return GetMaskedAttributes(color, foregroundMask,
        FOREGROUND_INTENSITY, FOREGROUND_RED,
        FOREGROUND_GREEN, FOREGROUND_BLUE);
}

WORD GetBackgroundAttributes(Color color) {
    return GetMaskedAttributes(color, backgroundMask,
        BACKGROUND_INTENSITY, BACKGROUND_RED,
        BACKGROUND_GREEN, BACKGROUND_BLUE);
}

WORD GetAttributes(Color foreground, Color background) {
    return GetForegroundAttributes(foreground) |
        GetBackgroundAttributes(background);
}
#else
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

string Console::foregroundBegin;
string Console::backgroundBegin;
string Console::colorEnd;
#endif

Screen* Console::GetScreen() {
    return &Console::screen;
}

void Console::SetEcho(bool echo) {
#ifndef _WIN32
    struct termios stdIn;
    tcgetattr(STDIN_FILENO, &stdIn);
    stdIn.c_lflag &= ~ECHO;
    stdIn.c_lflag |= echo ? ECHO : 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &stdIn);
#endif
    return;
}

void Console::SetCursor(bool visible) {
#ifndef _WIN32
    cout << "\x1B[?25" << (visible ? 'h' : 'l');
#endif
    return;
}

void Console::Clear() {
#ifdef _WIN32
    cls(GetStdHandle(STD_OUTPUT_HANDLE));
#else
    cout << "\x1B[2J\x1B[H";
#endif
}

void Console::SetColor(Color foreground, Color background) {
#ifdef _WIN32
    SetConsoleTextAttribute(hStdout,
        GetAttributes(foreground, background));
#else
    colorEnd = "\x1B[0m";
    switch (foreground) {
    default: case NONE:
        foregroundBegin = "";
        break;
    case WHITE: foregroundBegin = "\x1B[97"; break;
    case GREY: foregroundBegin = "\x1B[90"; break;
    case BLACK: foregroundBegin = "\x1B[30"; break;
    case BLUE: foregroundBegin = "\x1B[94"; break;
    case CYAN: foregroundBegin = "\x1B[96"; break;
    case GREEN: foregroundBegin = "\x1B[92"; break;
    case MAGENTA: foregroundBegin = "\x1B[95"; break;
    case RED: foregroundBegin = "\x1B[91"; break;
    case YELLOW: foregroundBegin = "\x1B[93"; break;
    }
    switch (background) {
    default: case NONE:
        backgroundBegin = "";
        break;
    case WHITE: backgroundBegin = ";47m"; break;
    case GREY: backgroundBegin = ";47m"; break;
    case BLACK: backgroundBegin = ";40m"; break;
    case BLUE: backgroundBegin = ";44m"; break;
    case CYAN: backgroundBegin = ";46m"; break;
    case GREEN: backgroundBegin = ";42m"; break;
    case MAGENTA: backgroundBegin = ";45m"; break;
    case RED: backgroundBegin = ";41m"; break;
    case YELLOW: backgroundBegin = ";43m"; break;
    }
    cout << foregroundBegin << backgroundBegin;
#endif
}

void Console::UnsetColor() {
#ifdef _WIN32
    SetConsoleTextAttribute(hStdout, oldAttributes);
#else
    cout << colorEnd;
#endif
}

void Console::ShowScreen() {
#ifdef _WIN32
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdout, &screenInfo);
    oldAttributes = screenInfo.wAttributes;
#endif
    int width = screen.GetWidth();
    int height = screen.GetHeight();
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            SetColor(screen.GetColor(i, j, false),
                     screen.GetColor(i, j, true));
            cout.put(screen.GetChar(i, j));
            UnsetColor();
        }
        cout << endl;
    }
}

void Console::Update() {
#ifdef _WIN32
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
    int width = screen.GetWidth();
    int height = screen.GetHeight();
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Cell* prevCell = prevScreen.GetCell(i, j);
            Cell* cell = screen.GetCell(i, j);
            if (*prevCell == *cell) continue;
#ifdef _WIN32
            COORD cursorCoord = {i, j};
            SetConsoleCursorPosition(hStdout, cursorCoord);
#else
            cout << "\x1B[" << to_string(j + 1) << ';'
                << to_string(i + 1) << 'H';
            SetColor(cell -> foreground, cell -> background);
#endif
            cout.put(cell -> value);
#ifdef _WIN32
            FillConsoleOutputAttribute(
                hStdout,
                GetAttributes(cell -> foreground, cell -> background),
                1, cursorCoord, NULL);
#else
            UnsetColor();
#endif
            prevCell -> value = cell -> value;
            prevCell -> foreground = cell -> foreground;
            prevCell -> background = cell -> background;
        }
    }
}
