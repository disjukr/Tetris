enum Key {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE,
    ESC,
    W,
    A,
    S,
    D,
    Z,
    X,
    C,
    ETC
};

class Keyboard {
public:
    static bool hit();
    static Key code();
};
