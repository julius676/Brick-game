#include "Player.h"

int Player::getScore() const {
    return score;
}

int Player::getLives() const {
    return lives;
}
void Player::set_score(int sc) {
    score = sc;
}
void Player::set_lives(int lv) {
    lives = lv;
}

void Player::hitScore(){
    score += score_per_hit;
}

void Player::winScore(){
    score += score_per_life*lives;
}

void Player::loseLife(){
    lives--;
}