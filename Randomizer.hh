#pragma once

#include "Tetromino.hh"

class PieceGenerator {
public:
    PieceGenerator();
    ~PieceGenerator();
    Tetromino* Get();
};
