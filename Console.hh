#include "Screen.hh"

class Console {
#ifdef _WIN32
    static Screen prevScreen;
#else
    static std::string foregroundBegin;
    static std::string backgroundBegin;
    static std::string foregroundEnd;
    static std::string backgroundEnd;
#endif
    static Screen screen;
    static void SetColor(Color foreground, Color background);
    static void UnsetColor();
public:
    static Screen* GetScreen();
    static void Clear(char value, Color foreground, Color background);
    static void Clear(char value, Color background);
    static void Clear(Color background);
    static void Clear(char value);
    static void Clear();
    static void ShowScreen();
};
