#ifndef BRICK_H
#define BRICK_H
#include "tools.h"
#include <vector>
using namespace std;
#include <memory>
#include "constants.h"
#include "Ball.h"
#include <ostream>
#include <fstream>
enum BrickType {RAINBOW, BALLBRICK, SPLIT};
class Brick {
    private:
    
    protected:
        Square square;
        int clr;
        BrickType type;

    public:
        Brick(double cx=0., double cy=0., double size_sq=0.
             ,BrickType t=RAINBOW, int hp=0)
        :square(cx, cy, size_sq), clr(hp), type(t) {}
        virtual ~Brick() {}
        double getBrickX() const;
        double getBrickY() const;
        double getBrickSize() const;
        int getBrickHP() const;
        BrickType getBrickType() const;
        virtual void drawBrick() const=0;
        virtual void printBrick(ofstream &file) const=0;
        virtual void brickHit(int& i, Liste_Ball &balls,
             vector<unique_ptr<Brick>> &bricks, int &j) = 0;
};

class BrickRainbow : public Brick {
    public:
        BrickRainbow(double cx=0., double cy=0., double size_sq=0., int hp=0)
        :Brick(cx, cy, size_sq, RAINBOW, hp) {}
        ~BrickRainbow() {}
        void drawBrick()const override;
        void printBrick(ofstream &file) const override;
        void brickHit(int& i, Liste_Ball &balls,
             vector<unique_ptr<Brick>> &bricks, int &j) override;
    private:
};

class BrickBall : public Brick {
    public:
        void drawBrick()const override;
        BrickBall(double cx=0., double cy=0., double size_sq=0., int hp=0)
        :Brick(cx, cy, size_sq, BALLBRICK, hp = 1),
        ball_int(cx,cy,new_ball_radius) {}
        ~BrickBall() {}
        void printBrick(ofstream &file) const override;
        void brickHit(int& i, Liste_Ball &balls,
             vector<unique_ptr<Brick>> &bricks, int &j) override;
    private:
        Ball ball_int;
};

class BrickSplit : public Brick {
    public:
        void drawBrick() const override;
        BrickSplit(double cx = 0., double cy = 0., double size_sq = 0., int hp = 0)
        : Brick(cx, cy, size_sq, SPLIT, hp) {}
        ~BrickSplit() {}
        void printBrick(ofstream &file) const override;
        void brickHit(int& i, Liste_Ball &balls,
             vector<unique_ptr<Brick>> &bricks, int &j) override;
    private:
        void drawSplit(double x, double y, double c, double gap,int clr_)const;
};

typedef vector<unique_ptr<Brick>> Liste_Brick;


#endif // BRICK_H