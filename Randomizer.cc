#include "Randomizer.hh"

#include <cmath>

int RandomGenerator::Get(int min, int max) {
    return (int) std::floor(this -> Get() * (max - min + 1)) + min;
}

PieceGenerator::PieceGenerator() {}

PieceGenerator::~PieceGenerator() {}

Tetromino* PieceGenerator::Get() {
    Tetromino* piece;
    switch (this -> random.Get(0, 7)) {
    case 0:
        piece = new IMino();
        break;
    case 1:
        piece = new OMino();
        break;
    case 2:
        piece = new TMino();
        break;
    case 3:
        piece = new SMino();
        break;
    case 4:
        piece = new ZMino();
        break;
    case 5:
        piece = new JMino();
        break;
    default:
        piece = new LMino();
        break;
    }
    return piece;
}
