#ifndef PLAYER_H
#define PLAYER_H
#include "constants.h"
class Player {
    private:
    int score;
    int lives;

    public:
        Player(int s = 0, int l = 0):score(s), lives(l) {};
        void set_score(int score);
        void set_lives(int lives);
        int getScore() const;
        int getLives() const;
        void hitScore();
        void loseLife();
        void winScore();
};
#endif // PLAYER_H
