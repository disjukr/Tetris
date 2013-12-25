#pragma once

#include "Tetromino.hh"

class RandomGenerator {
public:
    virtual ~RandomGenerator() {}
    virtual void Set(int seed) = 0;
    virtual double Get() = 0; // generate 0~1 value
    virtual int Get(int min, int max); // generate integer value
};

class Lfsr113: public RandomGenerator {
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
    const int IA = 16807;
    const int IM = 2147483647;
    const int IQ = 127773;
    const int IR = 2836;
public:
    Lfsr113() {
        this -> Set(0xABCDEF);
    };
    virtual ~Lfsr113() {};
    void Set(int seed) {
        int e;
        if (seed <= 0) seed = 1;
        e = seed / IQ;
        seed = IA * (seed - e * IQ) - IR * e;
        if (seed < 0) seed += IM;
        if (seed < 2) a = seed + 2; else a = seed;
        e = seed / IQ;
        seed = IA * (seed - e * IQ) - IR * e;
        if (seed < 0) seed += IM;
        if (seed < 8) b = seed + 8; else b = seed;
        e = seed / IQ;
        seed = IA * (seed - e * IQ) - IR * e;
        if (seed < 0) seed += IM;
        if (seed < 16) c = seed + 16; else c = seed;
        e = seed / IQ;
        seed = IA * (seed - e * IQ) - IR * e;
        if (seed < 0) seed += IM;
        if (seed < 128) d = seed + 128; else d = seed;
        this -> Get(); // mix
    }
    using RandomGenerator::Get;
    virtual double Get() {
        unsigned int e = ((a << 6) ^ a) >> 13;
        a = ((a & 4294967294) << 18) ^ e;
        e  = ((b << 2) ^ b) >> 27;
        b = ((b & 4294967288) << 2) ^ e;
        e  = ((c << 13) ^ c) >> 21;
        c = ((c & 4294967280) << 7) ^ e;
        e  = ((d << 3) ^ d) >> 12;
        d = ((d & 4294967168) << 13) ^ e;
        return (a ^ b ^ c ^ d) * 2.3283064365386963e-10;
    }
};

class PieceGenerator {
    Lfsr113 random;
public:
    PieceGenerator();
    virtual ~PieceGenerator();
    virtual Tetromino* Get();
};
