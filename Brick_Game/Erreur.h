#ifndef ERREUR_H
#define ERREUR_H

#include "Player.h"
#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"
#include <iostream>
#include <vector>
#include "message.h"

bool valide_player(Player& player);
bool valide_arene_ball(Ball& ball);
bool valide_arene_brick(Brick& brick);
bool valide_arene_paddle(Paddle& paddle);
bool valide_brick_size_hp(Brick& brick);   
bool valide_norm(Ball& ball);
bool verif_col_bricks(const Liste_Brick& Liste_Brick);
bool verif_col_paddle_ball(const Paddle& paddle, const Liste_Ball& Liste_Ball);
bool verif_col_balls(const Liste_Ball& Liste_Ball);
bool verif_col_ball_brick(const Liste_Ball& Liste_Ball,
                          const Liste_Brick& Liste_Brick);
bool verif_col_paddle_brick(const Paddle& paddle, const Liste_Brick& Liste_Brick);

#endif // ERREUR_H