#include "Paddle.h"

double Paddle::getPaddleX() const
{
    return circle.getCircleX();
}

double Paddle::getPaddleY() const
{
    return circle.getCircleY();
}

double Paddle::getPaddleRayon() const
{
    return circle.getCircleRayon();
}


void Paddle::drawPaddle() const
{
    double x = getPaddleX();
    double y = getPaddleY();
    double r = getPaddleRayon();

    // Draw filled outer paddle (black)
    circle.drawCircle();

    Circle inner_circle(x, y, r - 1);
    inner_circle.drawCircle(WHITE);
}

void Paddle::updatePaddleX(double cx)
{
    circle.updateCircleX(cx);
}

int Paddle::num_brick_col_paddle(const Liste_Brick& Liste_Brick)
{
    int bricks_size = Liste_Brick.size();
    Circle cr_paddle(getPaddleX(), getPaddleY(), getPaddleRayon());
    for(int i = 0; i < bricks_size; ++i)
    {
        Square s(Liste_Brick[i]->getBrickX(), Liste_Brick[i]->getBrickY(),
                 Liste_Brick[i]->getBrickSize());
        if(intersection_circle_square(cr_paddle, s))
        {
            return i;
        }
    }
    return -1;
}