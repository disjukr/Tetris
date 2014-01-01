#pragma once

#include <string>
#include "Color.hh"

class Cell {
public:
    char value;
    Color foreground;
    Color background;
    Cell(char value, Color foreground, Color background) {
        Clear(value, foreground, background);
    };
    Cell(char value, Color background) {
        Clear(value, background);
    };
    Cell(Color background) {
        Clear(background);
    };
    Cell(char value) {
        Clear(value);
    };
    Cell() {
        Clear();
    };
    Cell(Cell& cell) {
        Clear(cell.value, cell.foreground, cell.background);
    }
    ~Cell() {};
    void Clear(char value, Color foreground, Color background) {
        this -> value = value;
        this -> foreground = foreground;
        this -> background = background;
    }
    void Clear(char value, Color background) {
        Clear(value, NONE, background);
    }
    void Clear(Color background) {
        Clear(' ', NONE, background);
    }
    void Clear(char value) {
        Clear(value, NONE, NONE);
    }
    void Clear() {
        Clear(' ', NONE, NONE);
    }
    bool operator==(const Cell& rhs)const {
        return this -> value == rhs.value &&
            this -> foreground == rhs.foreground &&
            this -> background == rhs.background;
    }
    bool operator!=(const Cell& rhs)const {
        return !(*this == rhs);
    }
};

class Screen {
    int width;
    int height;
    Cell** buffer;
    Cell dummy;
    void init(int width, int height);
public:
    Screen(int width, int height);
    Screen();
    ~Screen();
    int GetWidth();
    int GetHeight();
    Cell* GetCell(int x, int y);
    char GetChar(int x, int y);
    Color GetColor(int x, int y, bool background);
    void WriteCell(char value, int x, int y);
    void WriteLine(std::string value, int x, int y);
    void FillCell(Color color, int x, int y, bool background);
    void FillLine(Color color, int x, int y, int length, bool background);
    void RenderScreen(Screen& screen, int x, int y);
    void Clear(char value, Color foreground, Color background);
    void Clear(char value, Color background);
    void Clear(Color background);
    void Clear(char value);
    void Clear();
};
