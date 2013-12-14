#include "Screen.hh"

using namespace std;

void Screen::init(int width, int height) {
    this -> width = width;
    this -> height = height;
    this -> buffer = new Cell*[height];
    for (int i = 0; i < height; ++i) {
        buffer[i] = new Cell[width];
    }
}

Screen::Screen(int width, int height) {
    init(width, height);
}

Screen::Screen() {
    init(79, 24);
}

Screen::~Screen() {
    for (int i = 0; i < height; ++i)
        delete[] buffer[i];
    delete[] buffer;
}

int Screen::GetWidth() {
    return this -> width;
}

int Screen::GetHeight() {
    return this -> height;
}

Cell* Screen::GetCell(int x, int y) {
    if (x < 0 || x >= this -> GetWidth() ||
        y < 0 || y >= this -> GetHeight())
        return &(this -> dummy);
    else
        return &(this -> buffer[y][x]);
}

char Screen::GetChar(int x, int y) {
    return this -> GetCell(x, y) -> value;
}

Color Screen::GetColor(int x, int y, bool background) {
    Cell* cell = this -> GetCell(x, y);
    return background ? cell -> background : cell -> foreground;
}

void Screen::WriteCell(char value, int x, int y) {
    GetCell(x, y) -> value = value;
}

void Screen::WriteLine(string value, int x, int y) {
    int length = value.length();
    for (int i = 0; i < length; ++i)
        GetCell(x + i, y) -> value = value[i];
}

void Screen::FillCell(Color color, int x, int y, bool background) {
    Cell* cell = this -> GetCell(x, y);
    if (background)
        cell -> background = color;
    else
        cell -> foreground = color;
}

void Screen::FillLine(Color color, int x, int y, int length, bool background) {
    for (int i = 0; i < length; ++i) {
        Cell* cell = this -> GetCell(x + i, y);
        if (background)
            cell -> background = color;
        else
            cell -> foreground = color;
    }
}

void Screen::RenderScreen(Screen& screen, int x, int y) {
    for (int j = 0; j < screen.height; ++j) {
        for (int i = 0; i < screen.width; ++i) {
            int tx = i + x;
            int ty = j + y;
            Cell* cell = screen.GetCell(i, j);
            this -> WriteCell(cell -> value, tx, ty);
            this -> FillCell(cell -> foreground, tx, ty, false);
            this -> FillCell(cell -> background, tx, ty, true);
        }
    }
}

void Screen::Clear(char value, Color foreground, Color background) {
    for (int j = 0; j < height; ++j)
        for (int i = 0; i < width; ++i)
            buffer[j][i].Clear(value, foreground, background);
}

void Screen::Clear(char value, Color background) {
    Clear(value, NONE, background);
}

void Screen::Clear(Color background) {
    Clear(' ', NONE, background);
}

void Screen::Clear(char value) {
    Clear(value, NONE, NONE);
}

void Screen::Clear() {
    Clear(' ', NONE, NONE);
}
