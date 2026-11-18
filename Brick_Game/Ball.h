#ifndef BALL_H
#define BALL_H


#include "message.h"
#include "constants.h"
#include <iostream>
#include <vector>
#include "tools.h"
#include "graphic_gui.h"
using namespace std;

class Ball{
    private:
        Circle circle;
        double delta_x;
        double delta_y;
    
    public:
        Ball(double cx=0, double cy=0, double r=0, double dx=0, double dy=0):
        circle(cx, cy, r), delta_x(dx), delta_y(dy) {}
        double getBallX() const;
        double getBallY() const;
        double getBallRayon() const;
        double getDeltaX() const ;
        double getDeltaY() const ;
        void drawBall() const;
        void moveBall();
        bool col_arena();
        bool ball_sortie() const;
        bool col_ball_ball(Ball& autre_ball);
        void setBall_dx(double dx);
        void setBall_dy(double dy);
        bool inter_next_frame(Ball autre_ball);
        void setBallX(double x);
        void setBallY(double y);
        void col_ball_ball1(double& delta_x, double& delta_y, double& new_dx, double& new_dy);
};
typedef vector<Ball> Liste_Ball;

#endif // BALL_H