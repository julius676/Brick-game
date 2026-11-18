#include "Erreur.h"
#include <cmath>


// ERREUR

// Fonctions erreur lié au joueur
bool valide_player(Player & player)
{
    int score = player.getScore();
    int lives = player.getLives();

    if (score < 0)
    {
        cout << message::invalid_score(score) << endl;
        return true;
    }

    if (lives < 0)
    {
        cout << message::invalid_lives(lives) << endl;
        return true;
    }
    return false;
}

//Fonctions erreur balls

bool valide_arene_ball(Ball& ball)
 // affiche message d erreur si la balle est en dehors de l arene
{
    double px = ball.getBallX();
    double py = ball.getBallY();
    double r = ball.getBallRayon();
    if(px < r || px > arena_size - r || py > arena_size - r || py < 0){
        cout << message::ball_outside(ball.getBallX(), ball.getBallY()) << endl;
        return true;}
    return false;    
    
}

//Fonctions erreur bricks

bool valide_arene_brick(Brick& brick)
{
    double px = brick.getBrickX();
    double py = brick.getBrickY();
    double size = brick.getBrickSize();

    if(px < size/2 || px > arena_size - size/2 ||
       py > arena_size - size/2 || py < size/2){
        cout << message::brick_outside(px, py) << endl;
        return true;
    }
    return false;
}

//Fonctions erreur paddle

bool valide_arene_paddle(Paddle& paddle)
{
    double px = paddle.getPaddleX();
    double py = paddle.getPaddleY();
    double r = paddle.getPaddleRayon();
    double distance_min = sqrt(pow(r,2) - pow(py, 2));

    if(px < distance_min || px > arena_size - distance_min || py > 0 || py <= -r){
        cout << message::paddle_outside(px, py) << endl;
        return true;}
    return false;
}

bool valide_brick_size_hp(Brick& brick)
{
    double size = brick.getBrickSize();
    int hit_points = brick.getBrickHP();

    if(size < brick_size_min){
        cout << message::invalid_brick_size(size) << endl;
        return true;
    }

    if(hit_points < 1 || hit_points > 7){
        cout << message::invalid_hit_points(hit_points) << endl;
        return true;
    }
    return false;
}

bool valide_norm(Ball& ball)
{
    double delta_x = ball.getDeltaX();
    double delta_y = ball.getDeltaY();
    double norm = sqrt(pow(delta_x, 2) + pow(delta_y, 2));

    if(norm > delta_norm_max){
        cout << message::invalid_delta(delta_x, delta_y) << endl;
        return true;}
    return false;
}

bool verif_col_bricks(const Liste_Brick& Bricks)
{
    int bricks_size = Bricks.size();
    for(int i = 0; i < bricks_size; i++)
    {
        Square sq1(Bricks[i]->getBrickX(), Bricks[i]->getBrickY(),
                   Bricks[i]->getBrickSize());
        for(int j = i + 1; j < bricks_size; j++)
        {
            Square sq2(Bricks[j]->getBrickX(), Bricks[j]->getBrickY(),
                       Bricks[j]->getBrickSize());
            if(intersection_squares(sq1, sq2, 0.))
            {
                cout << message::collision_bricks(i, j) << endl;
                return true;
            }
        }
    }
    return false;
}

bool verif_col_paddle_ball(const Paddle& paddle, const Liste_Ball& Liste_Ball)
{
    int balls_size = Liste_Ball.size();
    Circle cr_paddle(paddle.getPaddleX(), paddle.getPaddleY(),
                     paddle.getPaddleRayon());
    for(int i = 0; i < balls_size; i++)
    {
        Circle cr(Liste_Ball[i].getBallX(), Liste_Ball[i].getBallY(),  
                  Liste_Ball[i].getBallRayon());
        if(intersection_circles(cr, cr_paddle, 0.))
        {
            cout << message::collision_paddle_ball(i) << endl;
            return true;
        }
    }
    return false;
}

bool verif_col_balls(const Liste_Ball& Liste_Ball)
{
    int balls_size = Liste_Ball.size();
    for(int i = 0; i < balls_size; ++i)
    {
        Circle c1(Liste_Ball[i].getBallX(), Liste_Ball[i].getBallY(),
                  Liste_Ball[i].getBallRayon());
        for(int j = i + 1; j < balls_size; ++j)
        {
            Circle c2(Liste_Ball[j].getBallX(), Liste_Ball[j].getBallY(),
                      Liste_Ball[j].getBallRayon());
            if(intersection_circles(c1, c2, 0.))
            {
                cout << message::collision_balls(i, j) << endl;
                return true;
            }
            }
        }
    return false;
}


bool verif_col_ball_brick(const Liste_Ball& Liste_Ball, const Liste_Brick& Liste_Brick)
{
    int balls_size = Liste_Ball.size();
    int bricks_size = Liste_Brick.size();
    for (int i = 0; i < balls_size; ++i)
    {
        Circle c(Liste_Ball[i].getBallX(), Liste_Ball[i].getBallY(),
                 Liste_Ball[i].getBallRayon());
        for(int j = 0; j < bricks_size; ++j)
        {
            Square s(Liste_Brick[j]->getBrickX(), Liste_Brick[j]->getBrickY(),
                     Liste_Brick[j]->getBrickSize());
            if(intersection_circle_square(c, s, 0.))
            {
                cout << message::collision_ball_brick(i, j) << endl;
                return true;
            }
        }
    }
    return false;
}

bool verif_col_paddle_brick(const Paddle& paddle, const Liste_Brick& Liste_Brick)
{
    int bricks_size = Liste_Brick.size();
    Circle cr_paddle(paddle.getPaddleX(), paddle.getPaddleY(),
                     paddle.getPaddleRayon());
    for(int i = 0; i < bricks_size; ++i)
    {
        Square s(Liste_Brick[i]->getBrickX(), Liste_Brick[i]->getBrickY(),
                 Liste_Brick[i]->getBrickSize());
        if(intersection_circle_square(cr_paddle, s, 0.))
        {
            cout << message::collision_paddle_brick(i) << endl;
            return true;
        }
    }
    return false;
}


