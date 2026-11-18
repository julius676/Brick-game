#ifndef GRAPHIC_H
#define GRAPHIC_H

enum Color
{
    RED=1,
    ORANGE,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    PURPLE,
    BLACK,
    GREY,
    WHITE,
};
void set_color(Color color);
void draw_square(double x, double y, double size, const Color color);
void draw_circle(double x, double y, double rayon, const Color color);
#endif
