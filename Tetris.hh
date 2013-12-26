#include "Console.hh"
#include "Tetromino.hh"
#include "Randomizer.hh"
#include <functional>

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
    void RenderBlock(Color color, int x, int y);
public:
    TetrisStage();
    ~TetrisStage();
    bool CheckBlock(int x, int y);
    bool CheckCollision(Tetromino& piece);
    void CastPiece(Tetromino& piece);
    void AttachPiece(Tetromino& piece);
    void Render(Tetromino& piece, bool renderGhost);
    void HighlightLine(int y);
    void EraseLine(int y);
    Screen* GetScreen();
};

class Tetris {
    bool exit;
    int interval;
    unsigned int frame;
    int dropFrameInterval;
    int lastDrop;
    TetrisStage stage;
    PieceGenerator* pieceGenerator;
    Tetromino* currentPiece;
    void GameLoop();
    void Render();
    void AttachPiece();
    Tetromino* EmitPiece();
    void SoftDrop();
    void HardDrop();
    void ControlPiece(std::function<void()> controlFunction);
    bool CheckGameOver();
    bool CheckLine(int y);
public:
    Tetris();
    ~Tetris();
    void SetFps(int fps);
    void Start();
    void Exit();
};
