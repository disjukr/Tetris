#include "Console.hh"
#include "Tetromino.hh"

class TetrisStage {
    Color map[22][10];
    Screen* screen;
    bool CheckOutOfRange(int x, int y);
    bool CheckOutOfMap(int x, int y);
    Color ReadBlock(int x, int y);
    void WriteBlock(Color color, int x, int y);
public:
    TetrisStage();
    ~TetrisStage();
    bool CheckBlock(int x, int y);
    bool CheckCollision(Tetromino& piece);
    void CastPiece(Tetromino& piece);
    bool AttachPiece(Tetromino& piece);
    void RenderStage();
    void RenderGhostPiece(Tetromino& piece);
    void RenderPiece(Tetromino& piece);
    void RenderBlock(Color color, int x, int y);
    Screen* GetScreen();
};

class PieceGenerator {
public:
    PieceGenerator();
    ~PieceGenerator();
    Tetromino* Get();
};

class Tetris {
    bool exit;
    int interval;
    unsigned int frame;
    int dropFrameInterval;
    int lastDrop;
    TetrisStage stage;
    PieceGenerator pieceGenerator;
    Tetromino* currentPiece;
    void GameLoop();
    void Render();
    void AttachPiece();
    void SoftDrop();
    void HardDrop();
    void MovePiece(int x);
    void RotatePiece(bool clockwise);
    bool CheckGameOver();
public:
    Tetris();
    ~Tetris();
    void SetFps(int fps);
    void Start();
    void Exit();
};
