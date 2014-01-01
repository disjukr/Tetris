#include "Console.hh"
#include "Tetromino.hh"
#include "Randomizer.hh"
#include <functional>
#include <deque>

class Page {
    bool exit;
public:
    Page() {
        exit = false;
        frame = 0;
        interval = 17;
    }
    virtual ~Page() {}
    int frame;
    int interval;
    void Start();
    void SetFps(int fps);
    virtual void Exit();
protected:
    virtual void Loop() {}
    virtual void Render() {}
};

class Intro: public Page {
public:
    Intro() {}
    ~Intro() {}
protected:
    virtual void Loop();
    virtual void Render();
};

class TetrisStage {
public:
    static const int width = 10;
    static const int height = 22;
private:
    Color map[height][width];
    Screen* screen;
    bool CheckOutOfRange(int x, int y);
    bool CheckOutOfMap(int x, int y);
    Color ReadBlock(int x, int y);
    void WriteBlock(Color color, int x, int y);
    void RenderStage();
    void RenderGhostPiece(Tetromino& piece);
    void RenderPiece(Tetromino& piece);
public:
    TetrisStage();
    ~TetrisStage();
    bool CheckBlock(int x, int y);
    bool CheckCollision(Tetromino& piece);
    int CastPiece(Tetromino& piece);
    void AttachPiece(Tetromino& piece);
    void Render(Tetromino& piece, bool renderGhost);
    void EraseLine(int y);
    Screen* GetScreen();
};

struct TetrisStatistics {
    int score;
    int level;
    int _single;
    int _double;
    int _triple;
    int _tetris;
    TetrisStatistics() {
        score = 0;
        level = 1;
        _single = 0;
        _double = 0;
        _triple = 0;
        _tetris = 0;
    }
};

class Tetris: public Page {
    static const int pieceQueueSize = 4;
    int dropFrameInterval;
    int attachFrameInterval;
    int lastDrop;
    TetrisStatistics statistics;
    TetrisStage stage;
    Color backgroundColor;
    Screen* queueScreen;
    Screen* holdPieceScreen;
    PieceGenerator* pieceGenerator;
    std::deque<Tetromino*> pieceQueue;
    Tetromino* currentPiece;
    Tetromino* holdPiece;
    void RenderPieceQueue();
    void RenderHoldPiece();
    Tetromino* NextPiece();
    Tetromino* EmitPiece(Tetromino* piece = NULL);
    void HoldPiece();
    void AttachPiece();
    bool SoftDrop(bool attach = true);
    void HardDrop(bool attach = true);
    void ControlPiece(std::function<void()> controlFunction);
    void Infinity();
    void LevelUp();
    bool CheckGameOver();
    bool CheckLine(int y);
public:
    Tetris();
    ~Tetris();
protected:
    virtual void Loop();
    virtual void Render();
};
