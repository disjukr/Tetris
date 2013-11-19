#include "Tetris.hh"

int main() {
    Tetris* game = new Tetris();
    game -> SetFps(24);
    game -> Start();
    delete game;
    return 0;
}
