#include "Console.hh"
#include "Tetromino.hh"
#include "Randomizer.hh"
#include <functional>
#include <deque>

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

class Tetris {
    static const int pieceQueueSize = 4;
    bool exit;
    int interval;
    unsigned int frame;
    int dropFrameInterval;
    int lastDrop;
    TetrisStatistics statistics;
    TetrisStage stage;
    Screen* queueScreen;
    Screen* holdPieceScreen;
    PieceGenerator* pieceGenerator;
    std::deque<Tetromino*> pieceQueue;
    Tetromino* currentPiece;
    Tetromino* holdPiece;
    void GameLoop();
    void Render();
    void RenderPieceQueue();
    void RenderHoldPiece();
    Tetromino* NextPiece();
    Tetromino* EmitPiece(Tetromino* piece = NULL);
    void HoldPiece();
    void AttachPiece();
    void SoftDrop();
    void HardDrop();
    void ControlPiece(std::function<void()> controlFunction);
    bool CheckGameOver();
    bool CheckLine(int y);
public:
    Tetris();
    ~Tetris();
    void SetFps(int fps);
    TetrisStatistics Start();
    void Exit();
};

class PieceRenderer {
public:
    static void RenderPiece(Screen& screen, Tetromino& piece, int x, int y) {
        for (int i = 0; i < Tetromino::size; ++i) {
            for (int j = 0; j < Tetromino::size; ++j) {
                int tx = (i + x) * 2;
                int ty = j + y;
                if (piece.CheckBlock(i, j)) {
                    screen.FillCell(piece.color, tx, ty, true);
                    screen.FillCell(piece.color, tx + 1, ty, true);
                }
            }
        }
    }
};
