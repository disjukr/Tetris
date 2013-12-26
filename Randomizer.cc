#include "Randomizer.hh"

#include <cmath>

int RandomGenerator::Get(int min, int max) {
    return (int) std::floor(this -> Get() * (max - min + 1)) + min;
}

PieceGenerator::PieceGenerator() {}

PieceGenerator::~PieceGenerator() {}

Tetromino* PieceGenerator::Get() {
    switch (this -> random.Get(0, 6)) {
    case 0: return new IMino();
    case 1: return new OMino();
    case 2: return new TMino();
    case 3: return new SMino();
    case 4: return new ZMino();
    case 5: return new JMino();
    default: return new LMino();
    }
}

TGM2Randomizer::TGM2Randomizer() {
    history[0] = Z;
    history[1] = S;
    history[2] = Z;
    history[3] = S;
    first = true;
    pos = 0;
}

TGM2Randomizer::~TGM2Randomizer() {}

bool TGM2Randomizer::CheckHistory(int pieceType) {
    for (int i = 0; i < historySize; ++i)
        if (pieceType == history[i])
            return true;
    return false;
}

Tetromino* TGM2Randomizer::Get() {
    int type = random.Get(0, first ? 3 : 6); // exclude O, S, Z
    first = false;
    int i = 0;
    while (this -> CheckHistory(type) && i++ < tryCount)
        type = random.Get(0, 6);
    history[pos] = type;
    pos = (pos + 1) % 4;
    switch (type) {
    case I: return new IMino();
    case O: return new OMino();
    case T: return new TMino();
    case S: return new SMino();
    case Z: return new ZMino();
    case J: return new JMino();
    default: return new LMino();
    }
}
