#include "Randomizer.hh"

#include <cstdlib>

PieceGenerator::PieceGenerator() {}

PieceGenerator::~PieceGenerator() {}

Tetromino* PieceGenerator::Get() {
    Tetromino* piece;
    switch (rand() & 7) {
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
