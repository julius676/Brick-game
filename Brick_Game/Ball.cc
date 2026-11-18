#include "Ball.h"
#include <iostream>

using namespace std;

double Ball::getBallX() const
{
    return circle.getCircleX();
}

double Ball::getBallY() const
{
    return circle.getCircleY();
}

double Ball::getBallRayon() const
{
    return circle.getCircleRayon();
}

double Ball::getDeltaX() const
{
    return delta_x;
}

double Ball::getDeltaY() const
{
    return delta_y;
}  
void Ball::drawBall() const {
    circle.drawCircle();
}

void Ball::moveBall(){
    circle.updateCircleX(circle.getCircleX() + delta_x);
    circle.updateCircleY(circle.getCircleY() + delta_y);
}

bool Ball::col_arena(){
    double x = getBallX();
    double y = getBallY();
    double r = getBallRayon();
    if(x +r+ delta_x >= arena_size ){
        if(x + r >= arena_size and delta_x < 0) return false;
        delta_x = -delta_x;
        return true;
    }

    if(x -r+ delta_x <= 0){
        if(x - r <= 0 and delta_x > 0) return false;
        delta_x = -delta_x;
        return true;
    }

    if(y+r + delta_y >= arena_size){
        if(y +r >= arena_size and delta_y < 0) return false;
        delta_y = -delta_y;
        return true;
    }
    return false;
}

bool Ball::ball_sortie() const{
    if(getBallY() < 0)
        return true;
    return false;
}

void Ball::setBall_dx(double dx)
{
    delta_x = dx;
}

void Ball::setBall_dy(double dy)
{
    delta_y = dy;
}

bool Ball::col_ball_ball(Ball& autre){ 
    double dx = circle.getCircleX() - autre.getBallX();
    double dy = circle.getCircleY() - autre.getBallY();
    double dist = sqrt(dx*dx + dy*dy);
    if (dist < epsil_zero) return false; // Evite division par zéro, 
    double r       = circle.getCircleRayon(), r_autre = autre.getBallRayon();
    bool currentOverlap = dist < r + r_autre, nextOverlap = inter_next_frame(autre); 
    if (!currentOverlap && !nextOverlap) return false;
    dx /= dist;
    dy /= dist;
    double relVx = delta_x - autre.getDeltaX(), relVy = delta_y - autre.getDeltaY();
    double approach = relVx * dx + relVy * dy;
    if (approach < 0) {
    double vn       = proj_ortho(delta_x,           delta_y,           dx, dy);
    double vn_autre = proj_ortho(autre.getDeltaX(), autre.getDeltaY(), dx, dy);
    double vnx = vn * dx,       vny = vn * dy;
    double vn_autre_x = vn_autre * dx, vn_autre_y = vn_autre * dy;
    double impulseX1 = (-vnx + vn_autre_x)*(2*r_autre*r_autre)/(r*r+r_autre*r_autre);
    double impulseY1 = (-vny + vn_autre_y)*(2*r_autre*r_autre)/(r*r+r_autre*r_autre);
    double impulseX2 = -impulseX1 * r*r / (r_autre*r_autre);
    double impulseY2 = -impulseY1 * r*r / (r_autre*r_autre);
    delta_x += impulseX1;
    delta_y += impulseY1;
    double new_dx = autre.getDeltaX() + impulseX2;
    double new_dy = autre.getDeltaY() + impulseY2;
    col_ball_ball1(delta_x, delta_y, new_dx, new_dy);
    autre.setBall_dx(new_dx);
    autre.setBall_dy(new_dy);
    } 
    if (currentOverlap) {
        double totalMass = r*r + r_autre*r_autre;
        double correction = r + r_autre - dist + 0.5;
        double c1 = correction * (r_autre*r_autre / totalMass);
        double c2 = correction * (r*r             / totalMass);
        this->setBallX(circle.getCircleX() + dx * c1);
        this->setBallY(circle.getCircleY() + dy * c1);
        autre.setBallX(autre.getBallX() - dx * c2);
        autre.setBallY(autre.getBallY() - dy * c2);
    }
    return true;
}

void Ball::col_ball_ball1(double& delta_x, double& delta_y, double& new_dx, double& new_dy){ 
    if (sqrt(delta_x*delta_x + delta_y*delta_y) > delta_norm_max) {
        double coef = coef_normalisation_Vmax(delta_x, delta_y);
        delta_x /= coef;
        delta_y /= coef;
    }
    if (sqrt(new_dx*new_dx + new_dy*new_dy) > delta_norm_max) {
        double coef = coef_normalisation_Vmax(new_dx, new_dy);
        new_dx /= coef;
        new_dy /= coef;
    }

    }
bool Ball::inter_next_frame(Ball autre_ball){
    double circle1x = circle.getCircleX() + delta_x;
    double circle1y = circle.getCircleY() + delta_y;
    double circle2x = autre_ball.getBallX() + autre_ball.getDeltaX();
    double circle2y = autre_ball.getBallY() + autre_ball.getDeltaY();

    double cur_dx = circle.getCircleX() - autre_ball.getBallX();
    double cur_dy = circle.getCircleY() - autre_ball.getBallY();
    double cur_dist = sqrt(cur_dx*cur_dx + cur_dy*cur_dy);
    double minDist  = circle.getCircleRayon() + autre_ball.getBallRayon();

    if (cur_dist < minDist) {
        double next_dx = circle1x - circle2x;
        double next_dy = circle1y - circle2y;
        double next_dist = sqrt(next_dx*next_dx + next_dy*next_dy);
        return next_dist < cur_dist;   
    }

    Circle circle1(circle1x, circle1y, circle.getCircleRayon());
    Circle circle2(circle2x, circle2y, autre_ball.getBallRayon());
    return intersection_circles(circle1, circle2);
}


void Ball::setBallX(double x){
    circle.updateCircleX(x);
}

void Ball::setBallY(double y){
    circle.updateCircleY(y);
}
