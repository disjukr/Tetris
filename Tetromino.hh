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

class TMino: public Tetromino {
    bool vertical;
    bool tail;
public:
    TMino() {
        vertical = true;
        tail = true;
        color = MAGENTA;
        x = 0;
        y = 0;
    }
    ~TMino() {}
    virtual void RotateCW() {
        if (vertical)
            tail = !tail;
        vertical = !vertical;
    }
    virtual void RotateCCW() {
        if (!vertical)
            tail = !tail;
        vertical = !vertical;
    }
    virtual bool CheckBlock(int x, int y) {
        if (!Tetromino::CheckBlock(x, y))
            return false;
        if (y == 0 || x == 3 ||
            ((y == 1 || y == 3) && (x == 0 || x == 2)))
            return false;
        if (vertical) {
            if (x == 1 &&
                ((tail && y == 1) || (!tail && y == 3)))
                return false;
        }
        else {
            if (y == 2 &&
                ((tail && x == 0) || (!tail && x == 2)))
                return false;
        }
        return true;
    }
};
