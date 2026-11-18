#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include "Ball.h"
#include "Paddle.h"
#include "Player.h"
#include "Brick.h"
#include "tools.h"
#include "constants.h"
#include <cmath>
#include <string>
#include "message.h"
#include "Erreur.h"
#include <memory>
#include <utility>

enum Objet{SCORE, LIVES, PADDLE, NB_BRICK, BRICK,NB_BALL, BALL};
enum Status{ONGOING, WIN, LOSE};


class Game {
    private:
        Liste_Ball balls;
        Liste_Brick bricks;
        Paddle paddle;
        Player player;
        string file;
        double souris_x;
        double souris_y;
        //load sous fonction
        bool loadPlayer_Paddle(istringstream &data, Objet& obj, Player& player);
        bool loadBrick(istringstream &data, size_t& nb_brick,
                       Liste_Brick& liste_bricks, Objet& obj);
        bool loadBall(istringstream &data, size_t& nb_ball, Liste_Ball& liste_balls);
        void load_nb_brick(istringstream &data, size_t& nb_brick, Objet& obj);
        void load_nb_ball(istringstream &data, size_t& nb_ball, Objet& obj);
        bool load_fail(ifstream& fichier);
        Status status;
        void boucle_ball_ball(unsigned int& nb_rebonds, int i, bool &collision);
        void boucle_ball_arena_paddle(unsigned int& nb_rebonds, 
            int i, bool &collision);
        void boucle_ball_brick(unsigned int& nb_rebonds, int i, bool &collision);
        void movePaddle();
        void moveBalls();
        void moveBalls_after_paddle();
        bool col_paddle_ball(Ball& ball);
        bool col_paddle_ball2(double& px, double& ball_x, double& py, double& ball_y,
            double& r, double& ball_r, double& vx, double& vy,
            Ball& ball, Circle& n2_ball, Circle& n2_paddle, 
            double& ball_dx, double& ball_dy);
        bool col_ball_brick(int& i, int& j);//obligé de passer indice i sinon bug avec
                                    //collision première game (brickhit brickball)
    public:        
        // Constructeur
        Game(Liste_Ball bl = {}, Liste_Brick br = {}, Paddle pd = Paddle(), 
            Player plyr = Player(), double sx=0, double sy=0)
            :balls(bl), bricks(std::move(br)), 
            paddle(pd), player(plyr),file(""),souris_x(sx), souris_y(sy) {}
        // Set
        void setBalls(Liste_Ball& bl);
        void setPaddle(Paddle& pd);
        void setPlayer(Player& plyr);
        void setLives(int lives);
        void loseLife();
        void setSouris(double x, double y);
        Status getStatus() const;
        // Get
        Liste_Ball& getBalls();
        Liste_Brick& getBricks();
        double getBrick_i_x(int i) const;
        double getBrick_i_y(int i) const;
        double getBrick_i_size(int i) const;
        Paddle getPaddle() const;
        Player getPlayer() const;
        int getScore() const;
        int getLives() const;
        size_t get_nb_bricks() const;
        size_t get_nb_balls() const;
        double getSourisX() const;
        double getSourisY() const;
        // Initialisation
        void affichage_etat_game() const; //Utile pour debugage
        bool loadGame(const string& filename);
        bool saveGame(const string &filename);
        bool collisionGame() const;
        void resetGame();
        bool reloadGame();
        // drawing
        void drawGame() const;
        //Game updates
        void stepGame();
        // Game end
        bool gameFinished();

    
};

#endif // GAME_H