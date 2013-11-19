#include "Screen.hh"

class Console {
    static Screen screen;
public:
    static Screen* GetScreen();
    static void Clear(char value, Color foreground, Color background);
    static void Clear(char value, Color background);
    static void Clear(Color background);
    static void Clear(char value);
    static void Clear();
    static void ShowScreen();
};
