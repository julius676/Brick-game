#include "tools.h"
#include <iostream>
double Point::getPtX() const {
    return x;
}

double Point::getPtY() const {
    return y;
}
double Circle::getCircleX() const {
    return x;
}
double Circle::getCircleY() const {
    return y;
}
double Circle::getCircleRayon() const {
    return rayon;
}
double Square::getSquareX() const {
    return x;
}
double Square::getSquareY() const {
    return y;
}
double Square::getSquareSize() const {
    return size;
}
bool intersection_circles(const Circle& cr1, const Circle& cr2,double epsilon) {
    double diff_x = cr1.getCircleX() - cr2.getCircleX();
    double diff_y = cr1.getCircleY() - cr2.getCircleY();
    double somme_rayons = cr1.getCircleRayon() + cr2.getCircleRayon();
    return (diff_x * diff_x + diff_y * diff_y)-(somme_rayons * somme_rayons) <epsilon;
}
bool intersection_squares(const Square& sq1, const Square& sq2,double epsilon) {
    double diff_x=abs(sq1.getSquareX() - sq2.getSquareX());
    double diff_y=abs(sq1.getSquareY() - sq2.getSquareY());
    double somme_taille=(sq1.getSquareSize() + sq2.getSquareSize())/2.;
    return (diff_x - somme_taille<epsilon) && (diff_y - somme_taille<epsilon);
}
bool intersection_circle_square(const Circle& cr, const Square& sq, double epsilon) {
    double sqx=sq.getSquareX();
    double sqy=sq.getSquareY();
    double crx=cr.getCircleX();
    double cry=cr.getCircleY();
    double diff_x=abs(crx - sqx);
    double diff_y=abs(cry - sqy);
    double rayon=cr.getCircleRayon();
    double demi_taille=sq.getSquareSize()/2.;
    if (diff_y - demi_taille<epsilon){
        return diff_x - (rayon + demi_taille)<epsilon; //Horizontalement
    }
    if (diff_x - demi_taille<epsilon){
        return diff_y - (rayon + demi_taille)<epsilon; //Verticalement
    }
    // Diagonalement
    if (crx > sqx){
        sqx+=demi_taille;
    }
    else{
        sqx -= demi_taille;
    }
    if(cry > sqy){
        sqy+=demi_taille;
    }
    else{
        sqy -= demi_taille;
    }
    double diff_diag_x=abs(crx - sqx);
    double diff_diag_y=abs(cry - sqy);
    return (pow(diff_diag_x, 2) + pow(diff_diag_y, 2))-(pow(rayon, 2)) <epsilon;
}
std::string conversion_string(int value){
    if (value == 0)
        return "0";
    std::string chaine;   
    while (value > 0)
    {
        char c = (value % 10) + '0';
        chaine = c + chaine;
        value /= 10;
    }
    return chaine;
}

//Drawing functions

void Circle::drawCircle(const Color color) const {
    draw_circle(x, y, rayon, color);
}



void Square::drawSquare(int lives)const{
    draw_square(x, y, size, Color(lives));
}

void Circle::updateCircleX(double cx){
    x=cx;
}

void Circle::updateCircleY(double cy){
    y=cy;
}

double proj_ortho(double x1, double x2, double y1, double y2){
    return x1*y1 + x2*y2;
}

double coef_normalisation_Vmax(double x, double y){
    double v_actuelle = sqrt(x*x + y*y);
    return (v_actuelle+epsil_zero)/delta_norm_max;
}
