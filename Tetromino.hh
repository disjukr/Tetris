#pragma once

#include "Screen.hh"

class Tetromino {
public:
    Tetromino() {
        color = NONE;
        x = 0;
        y = 0;
    }
    Tetromino(Tetromino& original) {
        this -> color = original.color;
        this -> x = original.x;
        this -> y = original.y;
    }
    static const int size = 4;
    virtual ~Tetromino() {}
    virtual Tetromino* Clone() = 0;
    virtual void RotateCW() = 0;
    virtual void RotateCCW() = 0;
    virtual bool CheckBlock(int x, int y) {
        return !(x < 0 || x >= size || y < 0 || y >= size);
    }
    virtual void Render(Screen& screen, int x, int y) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                int tx = i * 2 + x;
                int ty = j + y;
                if (this -> CheckBlock(i, j)) {
                    screen.FillCell(this -> color, tx, ty, true);
                    screen.FillCell(this -> color, tx + 1, ty, true);
                }
            }
        }
    }
    Color color;
    int x;
    int y;
};

class IMino: public Tetromino {
    bool vertical;
public:
    IMino(): Tetromino() {
        vertical = true;
        color = CYAN;
    }
    IMino(IMino& original): Tetromino(original) {
        this -> vertical = original.vertical;
    }
    ~IMino() {}
    virtual Tetromino* Clone() {
        return new IMino(*this);
    }
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
    OMino(): Tetromino() {
        color = YELLOW;
    }
    OMino(OMino& original): Tetromino(original) {}
    ~OMino() {}
    virtual Tetromino* Clone() {
        return new OMino(*this);
    }
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
    TMino(): Tetromino() {
        vertical = true;
        tail = true;
        color = MAGENTA;
    }
    TMino(TMino& original): Tetromino(original) {
        this -> vertical = original.vertical;
        this -> tail = original.tail;
    }
    ~TMino() {}
    virtual Tetromino* Clone() {
        return new TMino(*this);
    }
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

class SMino: public Tetromino {
    bool vertical;
public:
    SMino(): Tetromino() {
        vertical = true;
        color = GREEN;
    }
    SMino(SMino& original): Tetromino(original) {
        this -> vertical = original.vertical;
    }
    ~SMino() {}
    virtual Tetromino* Clone() {
        return new SMino(*this);
    }
    virtual void RotateCW() {
        vertical = !vertical;
    }
    virtual void RotateCCW() {
        vertical = !vertical;
    }
    virtual bool CheckBlock(int x, int y) {
        if (!Tetromino::CheckBlock(x, y))
            return false;
        if (y == 2 && (x == 1 || x == 2))
            return true;
        if (vertical)
            return (x == 1 && y == 1) || (x == 2 && y == 3);
        else
            return y == 3 && x < 2;
    }
};

class ZMino: public Tetromino {
    bool vertical;
public:
    ZMino(): Tetromino() {
        vertical = true;
        color = RED;
    }
    ZMino(ZMino& original): Tetromino(original) {
        this -> vertical = original.vertical;
    }
    ~ZMino() {}
    virtual Tetromino* Clone() {
        return new ZMino(*this);
    }
    virtual void RotateCW() {
        vertical = !vertical;
    }
    virtual void RotateCCW() {
        vertical = !vertical;
    }
    virtual bool CheckBlock(int x, int y) {
        if (!Tetromino::CheckBlock(x, y))
            return false;
        if (y == 2 && (x == 1 || x == 2))
            return true;
        if (vertical)
            return (x == 2 && y == 1) || (x == 1 && y == 3);
        else
            return y == 3 && x > 1;
    }
};

class JMino: public Tetromino {
    bool vertical;
    bool tail;
public:
    JMino(): Tetromino() {
        vertical = true;
        tail = true;
        color = BLUE;
    }
    JMino(JMino& original): Tetromino(original) {
        this -> vertical = original.vertical;
        this -> tail = original.tail;
    }
    ~JMino() {}
    virtual Tetromino* Clone() {
        return new JMino(*this);
    }
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
        if (vertical) {
            if (x == 1 && y > 0)
                return true;
            if (tail && x == 0 && y == 3)
                return true;
            if (!tail && x == 2 && y == 1)
                return true;
        }
        else {
            if (y == 2 && x < 3 )
                return true;
            if (!tail && x == 0 && y == 1)
                return true;
            if (tail && x == 2 && y == 3)
                return true;
        }
        return false;
    }
};

class LMino: public Tetromino {
    bool vertical;
    bool tail;
public:
    LMino(): Tetromino() {
        vertical = true;
        tail = true;
        color = YELLOW;
    }
    LMino(LMino& original): Tetromino(original) {
        this -> vertical = original.vertical;
        this -> tail = original.tail;
    }
    ~LMino() {}
    virtual Tetromino* Clone() {
        return new LMino(*this);
    }
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
        if (vertical) {
            if (x == 1 && y > 0)
                return true;
            if (tail && x == 2 && y == 3)
                return true;
            if (!tail && x == 0 && y == 1)
                return true;
        }
        else {
            if (y == 2 && x < 3 )
                return true;
            if (!tail && x == 0 && y == 3)
                return true;
            if (tail && x == 2 && y == 1)
                return true;
        }
        return false;
    }
};
