#include "Brick.h"

double Brick::getBrickX() const
{
    return square.getSquareX();
}

double Brick::getBrickY() const
{
    return square.getSquareY();
}

double Brick::getBrickSize() const
{
    return square.getSquareSize();
}

BrickType Brick::getBrickType() const
{
    return type;
}

int Brick::getBrickHP() const
{
    return clr;
}
void BrickRainbow::drawBrick()const{
    square.drawSquare(clr);
}
void BrickBall::drawBrick()const{
    square.drawSquare(clr);
    ball_int.drawBall();
}
void BrickSplit::drawBrick()const{
    double x = square.getSquareX();
    double y = square.getSquareY();
    double c = square.getSquareSize();
    double gap = split_brick_gap / 2.;
    drawSplit(x,y,c,gap,clr);
}
void BrickSplit::drawSplit(double x, double y, double c,double gap, int clr_)const{
    Square sq (x, y, c);
    sq.drawSquare(clr_);
    double taille = c / 2. - gap;
    if (taille<brick_size_min)
        return;
    double ecart = taille / 2. + gap;
    drawSplit(x + ecart, y + ecart, taille, gap, clr_ + 1);
    drawSplit(x - ecart, y + ecart, taille,gap,clr_+1);
    drawSplit(x + ecart, y - ecart, taille,gap,clr_+1);
    drawSplit(x - ecart, y - ecart, taille,gap,clr_+1);
}

//Sauvegare des bricks
void BrickRainbow::printBrick(ofstream &file) const{
    file << "0 " << getBrickX() << " " << getBrickY() << " " << getBrickSize() 
         << " " << getBrickHP() << endl;
}

void BrickBall::printBrick(ofstream &file) const{
    file << "1 " << getBrickX() << " " << getBrickY() << " " << getBrickSize() << endl;
}

void BrickSplit::printBrick(ofstream &file) const{
    file << "2 " << getBrickX() << " " << getBrickY() << " " << getBrickSize() << endl;
}

void BrickRainbow::brickHit(int& i, Liste_Ball &balls,
     vector<unique_ptr<Brick>> &bricks, int &j){
    --clr;
    if (clr <= 0){
        swap(bricks[j], bricks.back());
        bricks.pop_back();
    }
}

void BrickBall::brickHit(int& i, Liste_Ball &balls,
     vector<unique_ptr<Brick>> &bricks, int &j){
    clr =0;
    ball_int.setBall_dx(balls[i].getDeltaX());
    ball_int.setBall_dy(balls[i].getDeltaY());
    balls.push_back(ball_int);//ajout nouvelle balle donc rallocation ce qui peut
                            //provoquer des bugs si on ne passe pas i mais ball
    swap(bricks[j], bricks.back());
    bricks.pop_back();
}

void BrickSplit::brickHit(int& i, Liste_Ball &balls,
     vector<unique_ptr<Brick>> &bricks, int &j){
    double gap= split_brick_gap / 2.;
    double taille = square.getSquareSize() / 2. - gap;
    double x = square.getSquareX();
    double y = square.getSquareY();
    swap(bricks[j], bricks.back());
    bricks.pop_back(); //On supprime la brique 
    if (taille<brick_size_min)
        return;
    double ecart = taille / 2. + gap;
    bricks.push_back(unique_ptr<Brick>(new BrickSplit(x + ecart, y + ecart, taille, 1)));
    bricks.push_back(unique_ptr<Brick>(new BrickSplit(x - ecart, y + ecart, taille, 1)));
    bricks.push_back(unique_ptr<Brick>(new BrickSplit(x + ecart, y - ecart, taille, 1)));
    bricks.push_back(unique_ptr<Brick>(new BrickSplit(x - ecart, y - ecart, taille, 1)));
}