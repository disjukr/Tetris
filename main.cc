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
    struct sigaction signalActionHandler;
    signalActionHandler.sa_handler = signalHandler;
    sigemptyset(&signalActionHandler.sa_mask);
    signalActionHandler.sa_flags = 0;
    sigaction(SIGINT, &signalActionHandler, NULL);
    sigaction(SIGTERM, &signalActionHandler, NULL);
    // game logic
    game = new Tetris();
    game -> SetFps(24);
    game -> Start();
    delete game;
    return 0;
}
