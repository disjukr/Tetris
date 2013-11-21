#include "Keyboard.hh"

#include <stdio.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
int _kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}
#endif

char nonblockGetCh() {
#ifdef _WIN32
    return _getch();
#else
    return getchar();
#endif
}

bool Keyboard::hit() {
    return _kbhit();
}

Key Keyboard::code() {
    if (_kbhit()) {
        switch (nonblockGetCh()) {
        case 27:
#ifdef _WIN32
            return ESC;
#else
            if (!_kbhit()) return ESC;
            if (nonblockGetCh() == 91) {
                switch (nonblockGetCh()) {
                case 65: return UP;
                case 66: return DOWN;
                case 68: return LEFT;
                case 67: return RIGHT;
                }
            }
#endif
        case 'w': case 'W': return W;
        case 'a': case 'A': return A;
        case 's': case 'S': return S;
        case 'd': case 'D': return D;
        }
    }
    return ETC;
}
