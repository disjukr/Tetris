#include "Screen.hh"

class Console {
#ifndef _WIN32
    static int foregroundBegin;
    static int backgroundBegin;
#endif
    static Screen screen;
    static Screen prevScreen;
    static void SetColor(Color foreground, Color background);
    static void UnsetColor();
public:
    static Screen* GetScreen();
    static void SetEcho(bool echo);
    static void SetCursor(bool visible);
    static void Clear();
    static void ShowScreen();
    static void Update();
};
