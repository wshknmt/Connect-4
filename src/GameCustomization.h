#ifndef GAMECUSTOMIZATION
#define GAMECUSTOMIZATION

struct GameCustomization {
    int gameMode;
    int player1;
    int player2;
    int p1Counter = 0;
    int p2Counter = 0;
    std::string message;
};

#endif // GAMECUSTOMIZATION