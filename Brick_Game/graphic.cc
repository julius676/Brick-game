#include "graphic.h"
#include "graphic_gui.h"

using namespace std;

static const Cairo::RefPtr<Cairo::Context> *ptcr(nullptr);

// graphic_gui.h
void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr)
{
    ptcr = &cr;
}
// local functions
void set_color(Color color)
{
    double r(0.), g(0.), b(0.);

    switch (color)
    {
    case RED:
        r = 1.0;
        break;
    case ORANGE:
        r = 1.0;
        g = 0.5;
        break;
    case YELLOW:
        r = 1.0;
        g = 1.0;
        break;
    case GREEN:
        g = 1.0;
        break;
    case CYAN:
        g = 1.0;
        b = 1.0;
        break;
    case BLUE:
        b = 1.0;
        break;
    case PURPLE:
        r = 0.5;
        b = 1.0;
        break;
    case BLACK:
        r = g = b = 0.0;
        break;
    case GREY:
        r = g = b = 0.5;
        break;
    case WHITE:
        r = g = b = 1.0;
        break;
    default:
        break;
    }
    (*ptcr)->set_source_rgb(r, g, b);
}
void draw_square(double x, double y, double size, const Color color)
{
    set_color(color);
    Cairo::RefPtr<Cairo::Context> cr = *ptcr;
    cr->move_to(x + size / 2., y + size / 2.);
    cr->line_to(x- size / 2., y + size / 2.);
    cr->line_to(x- size / 2., y - size / 2.);
    cr->line_to(x+ size / 2., y  -size / 2.);
    cr->line_to(x+ size / 2., y + size / 2.);
    cr->fill();
}
void draw_circle(double x, double y, double rayon, const Color color)
{
    set_color(color);
    Cairo::RefPtr<Cairo::Context> cr = *ptcr;
    cr->move_to(x, y);
    cr->arc(x, y, rayon, 0, 2 * M_PI);
    cr->fill();
}