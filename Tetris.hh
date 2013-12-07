class Tetris {
    bool exit;
    int interval;
    unsigned int frame;
    void GameLoop();
    void Render();
public:
    Tetris();
    ~Tetris();
    void SetFps(int fps);
    void Start();
    void Exit();
};
