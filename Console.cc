#include "Console.hh"
#include <stdlib.h>
#include <iostream>

using namespace std;

Screen Console::screen;

#ifdef _WIN32
Screen Console::prevScreen;

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
string Console::foregroundBegin;
string Console::backgroundBegin;
string Console::foregroundEnd;
string Console::backgroundEnd;
#endif

Screen* Console::GetScreen() {
    return &Console::screen;
}

void Console::Clear() {
#ifdef _WIN32
    //system("cls");
    cls(GetStdHandle(STD_OUTPUT_HANDLE));
#else
    system("clear");
#endif
}

void Console::SetColor(Color foreground, Color background) {
#ifdef _WIN32
    SetConsoleTextAttribute(hStdout,
        GetAttributes(foreground, background));
#else
    foregroundEnd = "\x1B[39m";
    backgroundEnd = "\x1B[49m";
    switch (foreground) {
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
    switch (background) {
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
    cout << foregroundBegin << backgroundBegin;
#endif
}

void Console::UnsetColor() {
#ifdef _WIN32
    SetConsoleTextAttribute(hStdout, oldAttributes);
#else
    cout << foregroundEnd << backgroundEnd;
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
    int width = screen.GetWidth();
    int height = screen.GetHeight();
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Cell* prevCell = prevScreen.GetCell(i, j);
            Cell* cell = screen.GetCell(i, j);
            if (*prevCell == *cell) continue;
            COORD cursorCoord = {i, j};
            SetConsoleCursorPosition(hStdout, cursorCoord);
            cout.put(cell -> value);
            FillConsoleOutputAttribute(
                hStdout,
                GetAttributes(cell -> foreground, cell -> background),
                1, cursorCoord, NULL);
            prevCell -> value = cell -> value;
            prevCell -> foreground = cell -> foreground;
            prevCell -> background = cell -> background;
        }
    }
#else
    Clear();
    ShowScreen();
#endif
}
