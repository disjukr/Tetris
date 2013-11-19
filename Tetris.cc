#include "Tetris.hh"

#include <iostream>
#include "Time.hh"
#include "Console.hh"

using namespace std;

Tetris::Tetris() {}

Tetris::~Tetris() {}

void Tetris::SetFps(int fps) {
    this -> interval = 1000 / fps;
}

void Tetris::Start() {
    long long int current;
    long long int prevFrame = Time::msec();
    long long int diff;
    unsigned int frame = 0;
    Screen* mainScreen = Console::GetScreen();
    while (true) {
        current = Time::msec();
        diff = current - prevFrame;
        if (diff >= this -> interval) {
            Console::Clear(BLACK);
            mainScreen -> WriteLine(":: TETRIS ::", 0, 0);
            mainScreen -> FillLine(WHITE, 0, 0, 12, true);
            mainScreen -> FillCell(MAGENTA, (frame % 6) + 3, 0, true);
            mainScreen -> FillCell(RED, ((frame + 1) % 6) + 3, 0, true);
            mainScreen -> FillCell(YELLOW, ((frame + 2) % 6) + 3, 0, true);
            mainScreen -> FillCell(GREEN, ((frame + 3) % 6) + 3, 0, true);
            mainScreen -> FillCell(CYAN, ((frame + 4) % 6) + 3, 0, true);
            mainScreen -> FillCell(BLUE, ((frame + 5) % 6) + 3, 0, true);
            Console::ShowScreen();
            prevFrame = current - (diff - interval);
            ++frame;
        }
    }
}
