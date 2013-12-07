#include "Tetris.hh"

#include <iostream>
#include "Time.hh"
#include "Console.hh"
#include "Keyboard.hh"

using namespace std;

Tetris::Tetris() {
    this -> exit = false;
    this -> interval = 17;
    this -> frame = 0;
}

Tetris::~Tetris() {}

void Tetris::SetFps(int fps) {
    this -> interval = 1000 / fps;
}

int x;
int y;
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

void Tetris::Exit() {
    this -> exit = true;
}

void Tetris::Start() {
    x = 0;
    y = 0;
    long long int curr;
    long long int prev = Time::msec();
    long long int diff;
    Console::Clear();
    Console::SetEcho(false);
    Console::SetCursor(false);
    while (true) {
        curr = Time::msec();
        diff = curr - prev;
        GameLoop();
        if (exit) break;
        if (diff >= interval) {
            Render();
            prev = curr - (diff - interval);
            ++frame;
        }
    }
    Console::Clear();
    Console::SetEcho(true);
    Console::SetCursor(true);
}

void Tetris::GameLoop() {
    if (Keyboard::hit()) {
        switch(Keyboard::code()) {
        case UP: case W: --y; break;
        case DOWN: case S: ++y; break;
        case LEFT: case A: --x; break;
        case RIGHT: case D: ++x; break;
        case ESC: this -> Exit(); return;
        default: break;
        }
    }
}

void Tetris::Render() {
    Screen* mainScreen = Console::GetScreen();
    mainScreen -> Clear(' ', BLACK, GREEN);
    draw_tetris(mainScreen, frame, x, y);
    Console::Update();
}
