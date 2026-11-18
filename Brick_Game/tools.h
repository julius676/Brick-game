
#ifndef TOOLS_H
#define TOOLS_H
#include <cmath>
#include <string>
#include "graphic.h"
#include "constants.h"

using namespace std;

constexpr double epsil_zero = 0.125;
class Point{
    public:
        Point(double cx=0., double cy=0.):
        x(cx), y(cy) {}
        double getPtX() const;
        double getPtY() const;

    private:
        double x;
        double y;
};

class Circle{
    public:
        Circle(double cx=0., double cy=0., double r=0.):
        x(cx), y(cy), rayon(r) {}
        double getCircleX() const;
        double getCircleY() const;
        double getCircleRayon() const;
        void drawCircle(const Color color = BLACK) const;
        void updateCircleX(double cx);
        void updateCircleY(double cy);
    private:
        double x; // centre x
        double y;
        double rayon;
};

class Square{
    public:
        Square(double cx=0., double cy=0., double size_sq=0.):
        x(cx), y(cy), size(size_sq) {}
        double getSquareX() const;
        double getSquareY() const;
        double getSquareSize() const;
        void drawSquare(const int lives) const;

    private:
        double x; // centre x
        double y;
        double size; //Longueur coté
};

bool intersection_circles(const Circle& cr1, const Circle& cr2,
                          double epsilon = epsil_zero);
bool intersection_circle_square(const Circle& cr, const Square& sq,
                                double epsilon = epsil_zero);
bool intersection_squares(const Square& sq1, const Square& sq2,
                          double epsilon = epsil_zero); 
                          //Epsilon par def est epsil_zero

double proj_ortho(double x1, double x2, double y1, double y2);

double coef_normalisation_Vmax(double x, double y);

std::string conversion_string(int value);
#endif // TOOLS_H