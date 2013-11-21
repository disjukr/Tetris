#include "Tetris.hh"

#include <iostream>
#include "Time.hh"
#include "Console.hh"
#include "Keyboard.hh"

using namespace std;

Tetris::Tetris() {}

Tetris::~Tetris() {}

void Tetris::SetFps(int fps) {
    this -> interval = 1000 / fps;
}

void draw_tetris(Screen* screen, int frame, int x, int y) {
    screen -> WriteLine(":: TETRIS ::", x, y);
    screen -> FillLine(WHITE, x, y, 12, true);
    screen -> FillCell(MAGENTA, x + (frame % 6) + 3, y, true);
    screen -> FillCell(RED, x + ((frame + 1) % 6) + 3, y, true);
    screen -> FillCell(YELLOW, x + ((frame + 2) % 6) + 3, y, true);
    screen -> FillCell(GREEN, x + ((frame + 3) % 6) + 3, y, true);
    screen -> FillCell(CYAN, x + ((frame + 4) % 6) + 3, y, true);
    screen -> FillCell(BLUE, x + ((frame + 5) % 6) + 3, y, true);
}

void Tetris::Start() {
    long long int current;
    long long int prevFrame = Time::msec();
    long long int diff;
    int x = 0;
    int y = 0;
    unsigned int frame = 0;
    Screen* mainScreen = Console::GetScreen();
    Console::SetEcho(false);
    Console::SetCursor(false);
    while (true) {
        current = Time::msec();
        diff = current - prevFrame;
        if (Keyboard::hit()) {
            switch(Keyboard::code()) {
            case UP: case W: --y; break;
            case DOWN: case S: ++y; break;
            case LEFT: case A: --x; break;
            case RIGHT: case D: ++x; break;
            default: break;
            }
        }
        if (diff >= this -> interval) {
            mainScreen -> Clear(' ', BLACK, GREEN);
            draw_tetris(mainScreen, frame, x, y);
            Console::Update();
            prevFrame = current - (diff - interval);
            ++frame;
        }
    }
    Console::SetEcho(true);
    Console::SetCursor(true);
}
