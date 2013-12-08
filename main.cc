#include <cstdlib>
#include <csignal>
#include "Tetris.hh"

Tetris* game;

void signalHandler(int sig) {
    switch (sig) {
    case SIGINT: case SIGTERM:
        game -> Exit();
        break;
    default: break;
    }
}

int main() {
    // handle signal
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    // game logic
    game = new Tetris();
    game -> SetFps(24);
    game -> Start();
    delete game;
    return 0;
}
