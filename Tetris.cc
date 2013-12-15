#include "Tetris.hh"

#include <cstdlib>
#include <iostream>
#include "Time.hh"
#include "Keyboard.hh"

using namespace std;

Tetris::Tetris() {
    this -> exit = false;
    this -> interval = 17;
    this -> frame = 0;
    this -> currentPiece = this -> pieceGenerator.Get();
}

Tetris::~Tetris() {
    delete this -> currentPiece;
}

void Tetris::SetFps(int fps) {
    this -> interval = 1000 / fps;
}

void Tetris::Exit() {
    this -> exit = true;
}

void Tetris::Start() {
    long long int curr;
    long long int prev = Time::msec();
    long long int diff;
    Console::Clear();
    Console::SetEcho(false);
    Console::SetCursor(false);
    while (!exit) {
        curr = Time::msec();
        diff = curr - prev;
        GameLoop();
        if (exit) break;
        if (diff >= interval) {
            Render();
            prev = curr - (diff - interval);
            ++frame;
        }
    }
    Console::Clear();
    Console::SetEcho(true);
    Console::SetCursor(true);
}

void Tetris::GameLoop() {
    if (Keyboard::hit()) {
        Tetromino* backup = currentPiece -> Clone();
        switch(Keyboard::code()) {
        case UP: case W: currentPiece -> RotateCW(); break;
        case DOWN: case S: currentPiece -> RotateCCW(); break;
        case LEFT: case A: --(currentPiece -> x); break;
        case RIGHT: case D: ++(currentPiece -> x); break;
        case SPACE:
            stage.CastPiece(*currentPiece);
            stage.AttachPiece(*currentPiece);
            delete currentPiece;
            currentPiece = pieceGenerator.Get();
            break;
        case ESC: this -> Exit(); return;
        default: break;
        }
        if (stage.CheckCollision(*currentPiece)) {
            Tetromino* temp = currentPiece;
            currentPiece = backup;
            delete temp;
        }
    }
}

void Tetris::Render() {
    Screen* mainScreen = Console::GetScreen();
    Screen* stageScreen = this -> stage.GetScreen();
    mainScreen -> Clear(' ', BLACK, GREEN);
    this -> stage.RenderStage();
    this -> stage.RenderGhostPiece(*currentPiece);
    this -> stage.RenderPiece(*currentPiece);
    mainScreen -> RenderScreen(*stageScreen, 2, 1);
    Console::Update();
}

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
    piece -> x = 3;
    piece -> y = -2;
    return piece;
}

TetrisStage::TetrisStage() {
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 22; ++j)
            WriteBlock(NONE, i, j);
    this -> screen = new Screen(20, 22);
}

TetrisStage::~TetrisStage() {
    delete this -> screen;
}

bool TetrisStage::CheckOutOfRange(int x, int y) {
    return x < 0 || x >= 10 || y >= 22;
}

bool TetrisStage::CheckOutOfMap(int x, int y) {
    return this -> CheckOutOfRange(x, y) || y < 0;
}

Color TetrisStage::ReadBlock(int x, int y) {
    return this -> CheckOutOfMap(x, y) ? NONE : map[y][x];
}

void TetrisStage::WriteBlock(Color color, int x, int y) {
    if (this -> CheckOutOfMap(x, y))
        return;
    map[y][x] = color;
}

bool TetrisStage::CheckBlock(int x, int y) {
    return this -> ReadBlock(x, y) != NONE;
}

bool TetrisStage::CheckCollision(Tetromino& piece) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (piece.CheckBlock(i, j)) {
                int x = i + piece.x;
                int y = j + piece.y;
                if (this -> CheckOutOfRange(x, y) ||
                    this -> CheckBlock(x, y))
                    return true;
            }
        }
    }
    return false;
}

void TetrisStage::CastPiece(Tetromino& piece) {
    while (!(this -> CheckCollision(piece)))
        ++piece.y;
    --piece.y;
}

bool TetrisStage::AttachPiece(Tetromino& piece) {
    if (this -> CheckCollision(piece))
        return false;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (piece.CheckBlock(i, j)) {
                int x = i + piece.x;
                int y = j + piece.y;
                this -> WriteBlock(piece.color, x, y);
            }
        }
    }
    return true;
}

void TetrisStage::RenderStage() {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 22; ++j) {
            int x = i;
            int y = j;
            Color color = this -> ReadBlock(i, j);
            this -> RenderBlock(color == NONE ? BLACK : color, x, y);
        }
    }
    this -> screen -> WriteLine("____________________", 0, 1);
    this -> screen -> FillLine(RED, 0, 1, 20, false);
}

void TetrisStage::RenderPiece(Tetromino& piece) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int x = i + piece.x;
            int y = j + piece.y;
            if (piece.CheckBlock(i, j))
                this -> RenderBlock(piece.color, x, y);
        }
    }
}

void TetrisStage::RenderGhostPiece(Tetromino& piece) {
    Tetromino* ghost = piece.Clone();
    ghost -> color = GREY;
    this -> CastPiece(*ghost);
    this -> RenderPiece(*ghost);
    delete ghost;
}

void TetrisStage::RenderBlock(Color color, int x, int y) {
    this -> screen -> FillCell(color, x * 2, y, true);
    this -> screen -> FillCell(color, x * 2 + 1, y, true);
}

Screen* TetrisStage::GetScreen() {
    return this -> screen;
}
