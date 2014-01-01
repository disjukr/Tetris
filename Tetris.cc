#include "Tetris.hh"

#include <cmath>
#include <string>
#include <iostream>
#include "Time.hh"
#include "Keyboard.hh"

using namespace std;

void Page::Start() {
    long long int curr;
    long long int prev = Time::msec();
    long long int diff;
    Console::Clear();
    Console::SetEcho(false);
    Console::SetCursor(false);
    while (!exit) {
        curr = Time::msec();
        diff = curr - prev;
        Loop();
        if (diff >= interval) {
            Render();
            Console::Update();
            prev = curr - (diff - interval);
            ++frame;
        }
    }
    Console::Clear();
    Console::SetEcho(true);
    Console::SetCursor(true);
}

void Page::SetFps(int fps) {
    interval = 1000 / fps;
}

void Page::Exit() {
    exit = true;
}

Intro::Intro() {
    Screen* screen = Console::GetScreen();
    int width = screen -> GetWidth();
    int height = screen -> GetHeight();
    int size = Tetromino::size * 2;
    for (int i = 0; i < numberOfParticle; ++i) {
        Tetromino* particle = pieceGenerator.Get();
        particle -> x = random.Get(0, width - size);
        particle -> y = random.Get(0, height) - (height + size);
        int rotate = random.Get(0, 4);
        while (rotate > 0) {
            --rotate;
            particle -> RotateCW();
        }
        particles[i] = particle;
        ResetParticle(i);
    }
    string t = "";
    t += "||||||||    ||||||||    ||||||||    |||||||     ||||     |||||||";
    t += "||||||||    ||||||||    ||||||||    ||||||||    ||||    ||||||||";
    t += "   ||       ||             ||       ||    ||     ||     ||||    ";
    t += "   ||       ||||||         ||       ||    ||     ||      |||||| ";
    t += "   ||       ||             ||       |||||||      ||         ||||";
    t += "  ||||      ||||||||      ||||      |||  |||    ||||    ||||||||";
    t += "  ||||      ||||||||      ||||      |||  |||    ||||    ||||||| ";
    for (int i = 0; i < titleTableSize; ++i)
        titleTable[i] = t[i] == '|';
}

Intro::~Intro() {
    for (int i = 0; i < numberOfParticle; ++i)
        delete particles[i];
}

void Intro::Loop() {
    if (Keyboard::hit()) {
        Keyboard::code(); // consume character
        Exit();
    }
}

void Intro::Render() {
    Screen* screen = Console::GetScreen();
    int width = screen -> GetWidth();
    int height = screen -> GetHeight();
    int size = Tetromino::size * 2;
    screen -> Clear(' ', BLACK, WHITE);
    if ((frame % 50) > 25)
        screen -> WriteLine(
            "Press Any Key to Start", (width - 22) >> 1, height - 7);
    for (int i = 0; i < numberOfParticle; ++i){
        Tetromino* particle = particles[i];
        rotations[i] += rotateSpeeds[i];
        if (rotations[i] > 100) {
            if (directions[i])
                particle -> RotateCW();
            else
                particle -> RotateCCW();
            rotations[i] = 0;
        }
        falls[i] += fallSpeeds[i];
        if (falls[i] > 100) {
            ++particle -> y;
            falls[i] = 0;
        }
        if (particle -> y > height) {
            delete particles[i];
            particles[i] = particle = pieceGenerator.Get();
            particle -> x = random.Get(0, width - size);
            particle -> y = -size;
            ResetParticle(i);
        }
        particle -> Render(*screen, particle -> x, particle -> y);
    }
    RenderTitle((width - titleWidth) >> 1, 6);
}

void Intro::ResetParticle(int index) {
    rotations[index] = random.Get(0, 100);
    rotateSpeeds[index] = random.Get(1, 5);
    falls[index] = random.Get(0, 100);
    fallSpeeds[index] = random.Get(5, 20);
    directions[index] = random.Get() < 0.5;
}

void Intro::RenderTitle(int x, int y) {
    int xx;
    int yy;
    Screen* screen = Console::GetScreen();
    for (int i = 0; i < titleWidth; ++i) {
        for (int j = 0; j < titleHeight; ++j) {
            if (titleTable[j * titleWidth + i]) {
                xx = x + i;
                yy = y + j;
                screen -> FillCell(
                    ColorUtil::GetComplementary(
                        screen -> GetColor(xx, yy, true)),
                    xx, yy, true);
            }
        }
    }
}

Tetris::Tetris() {
    this -> dropFrameInterval = 20;
    this -> attachFrameInterval = 20;
    this -> lastDrop = this -> frame;
    this -> pieceGenerator = new TGM2Randomizer();
    this -> backgroundColor = BLUE;
    this -> queueScreen = new Screen(
        Tetromino::size * 2, (Tetromino::size + 1) * pieceQueueSize);
    for (int i = 0; i < pieceQueueSize; ++i)
        this -> pieceQueue.push_back(pieceGenerator -> Get());
    this -> holdPieceScreen = new Screen(
        Tetromino::size * 2, Tetromino::size + 1);
    this -> currentPiece = this -> EmitPiece();
    this -> holdPiece = NULL;
}

Tetris::~Tetris() {
    delete this -> currentPiece;
    delete this -> pieceGenerator;
    for (int i = 0; i < pieceQueueSize; ++i)
        delete this -> pieceQueue[i];
    this -> pieceQueue.clear();
    delete this -> queueScreen;
    delete this -> holdPieceScreen;
    if (this -> holdPiece != NULL)
        delete this -> holdPiece;
}

void Tetris::Loop() {
    if (Keyboard::hit()) {
        switch(Keyboard::code()) {
        case UP: case W:
            this -> ControlPiece([this]() {
                currentPiece -> RotateCW();
            });
            break;
        case X: case Q:
            this -> ControlPiece([this]() {
                currentPiece -> RotateCCW();
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
        case DOWN: case S:
            this -> SoftDrop();
            ++statistics.score;
            break;
        case SPACE: case F:
            this -> HardDrop();
            break;
        case C: case R:
            this -> HardDrop(false);
            break;
        case Z: case E:
            this -> HoldPiece();
            break;
        case ESC:
            this -> Exit();
            return;
        default:
            break;
        }
    }
    int frameDiff = (int) (frame - lastDrop);
    if (dropFrameInterval < frameDiff) {
        if (this -> SoftDrop(attachFrameInterval < frameDiff))
            lastDrop = frame;
    }
    if (this -> CheckGameOver()) {
        this -> Exit();
        return;
    }
    int lineCount = 0;
    for (int i = 0; i < TetrisStage::height; ++i) {
        if (this -> CheckLine(i)) {
            this -> stage.EraseLine(i);
            ++lineCount;
        }
    }
    switch (lineCount) {
    case 1: ++statistics._single; statistics.score += 100; break;
    case 2: ++statistics._double; statistics.score += 300; break;
    case 3: ++statistics._triple; statistics.score += 500; break;
    case 4: ++statistics._tetris; statistics.score += 800; break;
    }
    if (statistics.score > statistics.level * 1000)
        LevelUp();
}

void Tetris::Render() {
    stage.Render(*currentPiece, true);
    this -> RenderPieceQueue();
    this -> RenderHoldPiece();
    int xOffset = 2;
    int xGap = 1;
    int ceilOffset = 1;
    int yOffset = ceilOffset + 1;
    Screen* mainScreen = Console::GetScreen();
    Screen* stageScreen = stage.GetScreen();
    mainScreen -> Clear(
        ' ', ColorUtil::GetComplementary(backgroundColor), backgroundColor);
    mainScreen -> RenderScreen(*holdPieceScreen, xOffset, ceilOffset);
    xOffset += holdPieceScreen -> GetWidth() + xGap;
    mainScreen -> RenderScreen(*stageScreen, xOffset, ceilOffset);
    xOffset += stageScreen -> GetWidth() + xGap;
    mainScreen -> RenderScreen(*queueScreen, xOffset, ceilOffset);
    xOffset += queueScreen -> GetWidth() + xGap;
    // statistics
    mainScreen -> WriteLine("STATISTICS", xOffset, ceilOffset);
    mainScreen -> FillLine(WHITE, xOffset, ceilOffset, 10, true);
    mainScreen -> FillLine(BLACK, xOffset, ceilOffset, 10, false);
    ++xOffset;
    mainScreen -> WriteLine("score:", xOffset, ++yOffset);
    mainScreen -> WriteLine(
        to_string(statistics.score), xOffset + 7, yOffset);
    mainScreen -> WriteLine("level:", xOffset, ++yOffset);
    mainScreen -> WriteLine(
        to_string(statistics.level), xOffset + 7, yOffset);
    ++yOffset;
    int valueOffset = xOffset + 8;
    mainScreen -> WriteLine("single:", xOffset, ++yOffset);
    mainScreen -> WriteLine(
        to_string(statistics._single), valueOffset, yOffset);
    mainScreen -> WriteLine("double:", xOffset, ++yOffset);
    mainScreen -> WriteLine(
        to_string(statistics._double), valueOffset, yOffset);
    mainScreen -> WriteLine("triple:", xOffset, ++yOffset);
    mainScreen -> WriteLine(
        to_string(statistics._triple), valueOffset, yOffset);
    mainScreen -> WriteLine("tetris:", xOffset, ++yOffset);
    mainScreen -> WriteLine(
        to_string(statistics._tetris), valueOffset, yOffset);
}

void Tetris::RenderPieceQueue() {
    int width = queueScreen -> GetWidth();
    int offset = Tetromino::size + 1;
    queueScreen -> Clear(' ', BLACK, BLACK);
    queueScreen -> WriteLine("NEXT", 0, 0);
    for (int i = 0; i < pieceQueueSize; ++i) {
        queueScreen -> FillLine(backgroundColor, 0, i * offset, width, true);
        (*pieceQueue[i]).Render(*queueScreen, 0, i * offset + 1);
    }
    queueScreen -> FillLine(WHITE, 0, 0, 4, true);
    queueScreen -> FillLine(BLACK, 0, 0, 4, false);
}

void Tetris::RenderHoldPiece() {
    holdPieceScreen -> Clear(' ', BLACK, BLACK);
    holdPieceScreen -> WriteLine("HOLD", 0, 0);
    holdPieceScreen -> FillLine(backgroundColor, 0, 0,
        holdPieceScreen -> GetWidth(), true);
    if (holdPiece != NULL)
        (*holdPiece).Render(*holdPieceScreen, 0, 1);
    holdPieceScreen -> FillLine(WHITE, 0, 0, 4, true);
    holdPieceScreen -> FillLine(BLACK, 0, 0, 4, false);
}

Tetromino* Tetris::NextPiece() {
    Tetromino* result;
    this -> pieceQueue.push_back(pieceGenerator -> Get());
    result = this -> pieceQueue.front();
    this -> pieceQueue.pop_front();
    return result;
}

Tetromino* Tetris::EmitPiece(Tetromino* piece) {
    if (piece == NULL)
        piece = this -> NextPiece();
    piece -> x = (TetrisStage::width - Tetromino::size) / 2;
    piece -> y = -(Tetromino::size / 2);
    return piece;
}

void Tetris::HoldPiece() {
    if (holdPiece == NULL) {
        holdPiece = currentPiece;
        currentPiece = this -> EmitPiece();
    }
    else {
        Tetromino* temp = currentPiece;
        currentPiece = this -> EmitPiece(holdPiece);
        holdPiece = temp;
    }
}

void Tetris::AttachPiece() {
    stage.AttachPiece(*currentPiece);
    delete currentPiece;
    currentPiece = this -> EmitPiece();
}

bool Tetris::SoftDrop(bool attach) {
    ++(this -> currentPiece -> y);
    if (stage.CheckCollision(*currentPiece)) {
        --(this -> currentPiece -> y);
        if (attach)
            this -> AttachPiece();
        return false;
    }
    return true;
}

void Tetris::HardDrop(bool attach) {
    int cast = stage.CastPiece(*currentPiece);
    statistics.score += 2 * cast;
    if (attach)
        AttachPiece();
    else
        Infinity();
}

void Tetris::ControlPiece(std::function<void()> controlFunction) {
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
    Infinity();
}

void Tetris::Infinity() {
    ++(currentPiece -> y);
    if (stage.CheckCollision(*currentPiece))
        lastDrop = frame;
    --(currentPiece -> y);
}

void Tetris::LevelUp() {
    ++statistics.level;
    dropFrameInterval = max(dropFrameInterval - 1, 1);
    int color = (int) backgroundColor - 4;
    backgroundColor = (Color) ((++color % 6) + 4);
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
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
            WriteBlock(NONE, i, j);
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

int TetrisStage::CastPiece(Tetromino& piece) {
    int lineCount = 0;
    while (!(this -> CheckCollision(piece))) {
        ++piece.y;
        ++lineCount;
    }
    --piece.y;
    return lineCount - 1;
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
    piece.Render(*(this -> screen), piece.x * 2, piece.y);
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
