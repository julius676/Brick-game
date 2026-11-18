#ifndef PADDLE_H
#define PADDLE_H
#include "tools.h"
#include "Brick.h"
class Paddle {
    private:
        Circle circle;
        
    public:
        Paddle(double cx=0., double cy=0., double r=0.):
        circle(cx, cy, r) {}
        double getPaddleX() const;
        double getPaddleY() const;
        double getPaddleRayon() const;
        void drawPaddle() const;
        void updatePaddleX(double cx);
        int num_brick_col_paddle(const Liste_Brick& Liste_Brick);
       
};
#endif // PADDLE_H