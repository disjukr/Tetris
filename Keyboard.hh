enum Key {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE,
    ESC,
    Q,
    W,
    A,
    S,
    D,
    E,
    R,
    F,
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
