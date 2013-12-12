#include "Color.hh"

class Tetromino {
public:
    virtual ~Tetromino() {}
    virtual void RotateCW() = 0;
    virtual void RotateCCW() = 0;
    virtual bool CheckBlock(int x, int y) {
        return !(x < 0 || x > 3 || y < 0 || y > 3);
    }
    Color color;
    int x;
    int y;
};

class IMino: public Tetromino {
    bool vertical;
public:
    IMino() {
        vertical = true;
        color = CYAN;
        x = 0;
        y = 0;
    }
    ~IMino() {}
    virtual void RotateCW() {
        vertical = !vertical;
    }
    virtual void RotateCCW() {
        vertical = !vertical;
    }
    virtual bool CheckBlock(int x, int y) {
        if (!Tetromino::CheckBlock(x, y))
            return false;
        if (vertical)
            return x == 1;
        else
            return y == 2;
    }
};

class OMino: public Tetromino {
public:
    OMino() {
        color = YELLOW;
        x = 0;
        y = 0;
    }
    ~OMino() {}
    virtual void RotateCW() {}
    virtual void RotateCCW() {}
    virtual bool CheckBlock(int x, int y) {
        if (!Tetromino::CheckBlock(x, y))
            return false;
        return (x == 1 || x == 2) && (y == 1 || y == 2);
    }
};
