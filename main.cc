#if defined _WIN32 && defined _DEBUG
#include <crtdbg.h>
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
#include <cstdlib>
#include <csignal>
#include "Tetris.hh"

Page* currentPage;

void signalHandler(int sig) {
    switch (sig) {
    case SIGINT: case SIGTERM:
        currentPage -> Exit();
        break;
    default: break;
    }
}

int main() {
#if defined _WIN32 && defined _DEBUG
    // check memory leak
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    // handle signal
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    // game logic
    currentPage = new Intro();
    currentPage -> Start();
    delete currentPage;
    currentPage = new Tetris();
    currentPage -> Start();
    delete currentPage;
    return 0;
}
