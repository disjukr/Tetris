#include "Tetris.hh"

#include <iostream>
#include "Time.hh"
#include "Keyboard.hh"
#include "Randomizer.hh"

using namespace std;

Tetris::Tetris() {
    this -> exit = false;
    this -> interval = 17;
    this -> frame = 0;
    this -> dropFrameInterval = 20;
    this -> lastDrop = this -> frame;
    this -> pieceGenerator = new TGM2Randomizer();
    this -> queueScreen = new Screen(
        Tetromino::size * 2, (Tetromino::size + 1) * pieceQueueSize);
    for (int i = 0; i < pieceQueueSize; ++i)
        this -> pieceQueue.push_back(pieceGenerator -> Get());
    this -> currentPiece = this -> EmitPiece();
}

Tetris::~Tetris() {
    delete this -> currentPiece;
    delete this -> pieceGenerator;
    delete this -> queueScreen;
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
        switch(Keyboard::code()) {
        case UP: case W:
            this -> ControlPiece([this]() {
                currentPiece -> RotateCW();
            });
            break;
        case DOWN: case S:
            this -> ControlPiece([this]() {
                ++(currentPiece -> y);
            });
            break;
        case LEFT: case A:
            this -> ControlPiece([this]() {
                --(currentPiece -> x);
            });
            break;
        case RIGHT: case D:
            this -> ControlPiece([this]() {
                ++(currentPiece -> x);
            });
            break;
        case SPACE:
            this -> HardDrop();
            break;
        case ESC:
            this -> Exit();
            return;
        default:
            break;
        }
    }
    if (dropFrameInterval < (int) (frame - lastDrop)) {
        this -> SoftDrop();
        lastDrop = frame;
    }
    if (this -> CheckGameOver()) {
        this -> Exit();
        return;
    }
    for (int i = 0; i < TetrisStage::height; ++i)
        if (this -> CheckLine(i))
            this -> stage.EraseLine(i);
}

void Tetris::Render() {
    Screen* mainScreen = Console::GetScreen();
    Screen* stageScreen = stage.GetScreen();
    mainScreen -> Clear(' ', BLACK, GREEN);
    stage.Render(*currentPiece, true);
    mainScreen -> RenderScreen(*stageScreen, 2, 1);
    this -> RenderPieceQueue();
    mainScreen -> RenderScreen(
        *queueScreen, stageScreen -> GetWidth() + 3, 1);
    Console::Update();
}

void Tetris::RenderPieceQueue() {
    int width = queueScreen -> GetWidth();
    int offset = Tetromino::size + 1;
    queueScreen -> Clear(' ', BLACK, BLACK);
    queueScreen -> WriteLine("NEXT", 0, 0);
    for (int i = 0; i < pieceQueueSize; ++i) {
        queueScreen -> FillLine(GREEN, 0, i * offset, width, true);
        PieceRenderer::RenderPiece(
            *queueScreen, *pieceQueue[i], 0, i * offset + 1);
    }
    queueScreen -> FillLine(WHITE, 0, 0, 4, true);
    queueScreen -> FillLine(BLACK, 0, 0, 4, false);
}

Tetromino* Tetris::NextPiece() {
    Tetromino* result;
    this -> pieceQueue.push_back(pieceGenerator -> Get());
    result = this -> pieceQueue.front();
    this -> pieceQueue.pop_front();
    return result;
}

Tetromino* Tetris::EmitPiece() {
    Tetromino* piece = this -> NextPiece();
    piece -> x = (TetrisStage::width - Tetromino::size) / 2;
    piece -> y = -(Tetromino::size / 2);
    return piece;
}

void Tetris::AttachPiece() {
    stage.AttachPiece(*currentPiece);
    delete currentPiece;
    currentPiece = this -> EmitPiece();
}

void Tetris::SoftDrop() {
    ++(this -> currentPiece -> y);
    if (stage.CheckCollision(*currentPiece)) {
        --(this -> currentPiece -> y);
        this -> AttachPiece();
    }
}

void Tetris::HardDrop() {
    stage.CastPiece(*currentPiece);
    this -> AttachPiece();
}

void Tetris::ControlPiece(std::function<void()> controlFunction) {
    // try control
    Tetromino* trash;
    Tetromino* backup = currentPiece -> Clone();
    controlFunction();
    if (stage.CheckCollision(*currentPiece)) {
        trash = currentPiece;
        currentPiece = backup;
    }
    else {
        trash = backup;
    }
    delete trash;
    // infinity rule
    ++(currentPiece -> y);
    if (stage.CheckCollision(*currentPiece))
        lastDrop = frame;
    --(currentPiece -> y);
}

bool Tetris::CheckGameOver() {
    for (int i = 0; i < TetrisStage::width; ++i)
        if (this -> stage.CheckBlock(i, 1))
            return true;
    return false;
}

bool Tetris::CheckLine(int y) {
    for (int i = 0; i < TetrisStage::width; ++i)
        if (!(this -> stage.CheckBlock(i, y)))
            return false;
    return true;
}

TetrisStage::TetrisStage() {
    this -> screen = new Screen(width * 2, height);
}

TetrisStage::~TetrisStage() {
    delete this -> screen;
}

bool TetrisStage::CheckOutOfRange(int x, int y) {
    return x < 0 || x >= width || y >= height;
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
    for (int i = 0; i < Tetromino::size; ++i) {
        for (int j = 0; j < Tetromino::size; ++j) {
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

void TetrisStage::AttachPiece(Tetromino& piece) {
    for (int i = 0; i < Tetromino::size; ++i) {
        for (int j = 0; j < Tetromino::size; ++j) {
            if (piece.CheckBlock(i, j)) {
                int x = i + piece.x;
                int y = j + piece.y;
                this -> WriteBlock(piece.color, x, y);
            }
        }
    }
}

void TetrisStage::Render(Tetromino& piece, bool renderGhost) {
    this -> RenderStage();
    if (renderGhost)
        this -> RenderGhostPiece(piece);
    this -> RenderPiece(piece);
}

void TetrisStage::RenderStage() {
    // render map
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            Color color = this -> ReadBlock(i, j);
            int x = i * 2;
            int y = j;
            color = color == NONE ? BLACK : color;
            this -> screen -> FillCell(color, x, y, true);
            this -> screen -> FillCell(color, x + 1, y, true);
        }
    }
    // render dead line
    this -> screen -> WriteLine("____________________", 0, 1);
    this -> screen -> FillLine(RED, 0, 1, width * 2, false);
}

void TetrisStage::RenderPiece(Tetromino& piece) {
    PieceRenderer::RenderPiece(*(this -> screen), piece, piece.x, piece.y);
}

void TetrisStage::RenderGhostPiece(Tetromino& piece) {
    Tetromino* ghost = piece.Clone();
    ghost -> color = GREY;
    this -> CastPiece(*ghost);
    this -> RenderPiece(*ghost);
    delete ghost;
}

void TetrisStage::EraseLine(int y) {
    for (int j = y; j >= 0; --j)
        for (int i = 0; i < width; ++i)
            this -> WriteBlock(this -> ReadBlock(i, j - 1), i, j);
}

Screen* TetrisStage::GetScreen() {
    return this -> screen;
}
