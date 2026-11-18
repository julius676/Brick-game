#include "Game.h"


void Game::setBalls(Liste_Ball& bl){
    balls = bl;
}

void Game::setPaddle(Paddle& pd){
    paddle = pd;
}

void Game::setPlayer(Player& plyr){
    player = plyr;
}
void Game::setLives(int lives){
    player.set_lives(lives);
}
void Game::loseLife(){
    player.loseLife();
}

Liste_Ball& Game::getBalls(){
    return balls;
}


Liste_Brick& Game::getBricks(){
    return bricks;
}

double Game::getBrick_i_x(int i) const{
    return bricks[i]->getBrickX();
}

double Game::getBrick_i_y(int i) const{
    return bricks[i]->getBrickY();
}

double Game::getBrick_i_size(int i) const{
    return bricks[i]->getBrickSize();
}

Paddle Game::getPaddle() const{
    return paddle;
}

Player Game::getPlayer() const{
    return player;
}

int Game::getScore() const{
    return player.getScore();
}

int Game::getLives() const{
    return player.getLives();
}

void Game::affichage_etat_game() const //foction de detection d erreur personnelle
{
    cout << "Player: score = " << player.getScore() << ", lives = " 
         << player.getLives() << endl;
    cout << "Paddle: x = " << paddle.getPaddleX() << ", y = " << paddle.getPaddleY()
         << ", r = " << paddle.getPaddleRayon() << endl;
    cout << "Liste de briques: " << endl;
    for (size_t i = 0; i < bricks.size(); i++) {
        cout << "Brique " << i << ": x = " << bricks[i]->getBrickX() << ", y = " 
             << bricks[i]->getBrickY() << ", size = " 
             << bricks[i]->getBrickSize() << ", type = " 
             << bricks[i]->getBrickType() << ", hp = " 
             << bricks[i]->getBrickHP() << endl;   
    }
    cout << "Liste de balles: " << endl;
    for (size_t i = 0; i < balls.size(); i++) {
        cout << "Balle " << i << ": x = " << balls[i].getBallX() 
             << ", y = " << balls[i].getBallY() << ", r = " << balls[i].getBallRayon()
            << ", delta_x = " << balls[i].getDeltaX() << ", delta_y = " 
            << balls[i].getDeltaY() << endl;
    }
}

size_t Game::get_nb_bricks() const{
    return bricks.size();
}

size_t Game::get_nb_balls() const{
    return balls.size();
}
// Initialisation
bool Game::saveGame(const string& nom_fichier){
    ofstream fichier(nom_fichier);
    if (fichier.fail()) {
        cout << "Echec de l'ouverture du fichier" << endl;
        return false;
    }
    
    fichier << "# score" << endl << player.getScore() << endl<<endl;
    fichier << "# lives" << endl << player.getLives() << endl<<endl;
    fichier << "# paddle" << endl <<paddle.getPaddleX() << " " 
            << paddle.getPaddleY() << " " << paddle.getPaddleRayon() << endl<<endl;
    size_t brick_nb=bricks.size();
    fichier << "# bricks"<<endl << brick_nb << endl;
    for (size_t i(0); i<brick_nb; i++)
    {
        bricks[i]->printBrick(fichier);
    }
    size_t ball_nb=balls.size();
    fichier <<endl<< "# balls" <<endl << ball_nb << endl;
    for (size_t i(0); i<ball_nb; i++)
    {
        fichier << balls[i].getBallX() << " " << balls[i].getBallY() << " "
        << balls[i].getBallRayon() << " " << balls[i].getDeltaX() << " "
        << balls[i].getDeltaY() << endl;
    }
    file = nom_fichier;
    return true;
}

bool Game::loadGame(const string& nom_fichier){//renvoie true si le fichier est chargé
    resetGame();
    ifstream fichier(nom_fichier);
    Objet objet = SCORE;
    if (fichier.fail()) {
        cout << "Echec de l'ouverture du fichier" << endl;
        return false;}
    string line;
    size_t nb_ball = 0, nb_brick = 0;
    Liste_Ball liste_balls;
    Liste_Brick liste_bricks;
    Player player;
    while (getline(fichier, line)) {
        if(line[0]=='#')  continue;
        istringstream data(line);
        if (data.str().empty()) continue;
        switch(objet) {
            case(SCORE):
            case(LIVES):
            case(PADDLE): {
                if (!loadPlayer_Paddle(data, objet, player)) return load_fail(fichier);
                break;}
            case NB_BRICK: {
                load_nb_brick(data, nb_brick, objet);
                break;}
            case BRICK: {
                if (!loadBrick(data, nb_brick, liste_bricks, objet))
                     return load_fail(fichier);
                break;}
            case NB_BALL: {
                load_nb_ball(data, nb_ball, objet);
                break;}
            case BALL: {
                if (!loadBall(data, nb_ball, liste_balls)) return load_fail(fichier);
                break;}
            }}
    if (collisionGame())  return load_fail(fichier);
    file = nom_fichier;
    if (!gameFinished()) status = ONGOING;
    fichier.close();
    return true;
}

bool Game::load_fail(ifstream& fichier){
    resetGame();
    fichier.close();
    return false;
}

    void Game::load_nb_ball(istringstream & data, size_t &nb_ball, Objet &obj)
    {
        data >> nb_ball;
        if (nb_ball != 0)
        {
            obj = BALL;
        }
}

void Game::load_nb_brick(istringstream& data, size_t& nb_brick, Objet& obj){
    data >> nb_brick;
    if (nb_brick == 0){
        obj = NB_BALL;
        }
    else{
        obj = BRICK;
        }
}

bool Game::loadPlayer_Paddle(istringstream& data, Objet& obj, Player& player){
    int value(0);
    switch (obj) {
        case SCORE:
            data >> value;
            player.set_score(value);
            obj = LIVES;
            break;
        case LIVES:
            data >> value;
            player.set_lives(value);
            if (valide_player(player)) {
                return false;
            }
            this->player = player;
            obj = PADDLE;
            break;
        case PADDLE: {
            double x, y, r;
            data >> x >> y >> r;
            Paddle paddle(x, y, r);
            if (valide_arene_paddle(paddle)){
                return false;}
            this->paddle = paddle;
            setSouris(x, y);
            obj = NB_BRICK;
            break;}
        default :{
            cout << "unexpected error" << endl;
            return false;}
    }
    return true;
}

bool Game::loadBrick(istringstream& data, size_t& nb_brick,
                     Liste_Brick& liste_bricks, Objet& obj){
    double x, y, size;
    int hp=1, type;
    data >> type >> x >> y >> size>> hp;
    if (type == RAINBOW){
        liste_bricks.push_back(unique_ptr<Brick>(new BrickRainbow(x,y,size,hp)));}
    else if (type == BALLBRICK){
        liste_bricks.push_back(unique_ptr<Brick>(new BrickBall(x,y,size,hp))); }
    else if (type == SPLIT){
        liste_bricks.push_back(unique_ptr<Brick>(new BrickSplit(x,y,size,hp))); }
    else {
        cout << message::invalid_brick_type(type) << endl;
        return false;
        }
    if (valide_arene_brick(*liste_bricks.back()) ||
        valide_brick_size_hp(*liste_bricks.back())) {
        return false;
        }
    if (liste_bricks.size() == nb_brick){
        bricks = std::move(liste_bricks);
        obj = NB_BALL;
        }
    return true;
}

bool Game::loadBall(istringstream& data, size_t& nb_ball, Liste_Ball& liste_balls){
    double x, y, r, dx, dy;
    data >> x >> y >> r >> dx >> dy;
    Ball ball(x, y, r, dx, dy);
    if (valide_arene_ball(ball) || valide_norm(ball)){
        return false;
        }
    liste_balls.push_back(ball);
    if (liste_balls.size() == nb_ball){
        this->balls = liste_balls;
        }
    return true;
    }

bool Game::reloadGame(){
    int x = souris_x;
    int y = souris_y;
    bool temp = loadGame(file);
    souris_x = x;
    souris_y = y;
    return temp; //pour eviter un ptit bug quand on restart
}

bool Game::collisionGame() const
{
    if (verif_col_bricks(bricks)) return true;
    if (verif_col_paddle_ball(paddle, balls)) return true;
    if (verif_col_balls(balls)) return true;
    if (verif_col_ball_brick(balls, bricks)) return true;
    if (verif_col_paddle_brick(paddle, bricks)) return true;
    return false;
}

void Game::resetGame(){
    balls.clear();
    bricks.clear();
    paddle = Paddle();
    player = Player();
}

//drawing

void Game::drawGame() const {
    for (const auto& ball : balls) {
        ball.drawBall();
    }
    for (const auto& brick : bricks) {
        brick->drawBrick();
    }

    paddle.drawPaddle();
}

void Game::setSouris(double x, double y){
    souris_x = x;
    souris_y = y;
}

double Game::getSourisX() const{
    return souris_x;
}

double Game::getSourisY() const{
    return souris_y;
}
//Game updates

void Game::stepGame(){
    if (status != ONGOING) return;
    moveBalls();
    movePaddle();
    moveBalls_after_paddle();
    gameFinished();
}

void Game::movePaddle(){
    Paddle pd = getPaddle();
    double x_cible = getSourisX();
    double r = pd.getPaddleRayon(), px = pd.getPaddleX(), py = pd.getPaddleY();
    double distance_min = sqrt(pow(r,2) - pow(py, 2));
    double dx = x_cible - px;
    //V max
    if(dx > 0 and dx > delta_norm_max)
        x_cible = px + delta_norm_max;

    if(dx < 0 and dx < -delta_norm_max)
        x_cible = px - delta_norm_max;
    //Collision bord
    if(x_cible - distance_min <= epsil_zero)
        x_cible = distance_min + epsil_zero; 
        //evite erreur au restart

    if(x_cible + distance_min - arena_size >= epsil_zero)
        x_cible = arena_size - distance_min - epsil_zero;
         //evite erreur au restart
    pd.updatePaddleX(x_cible);
    setPaddle(pd);
    //Collision brick
    int i = pd.num_brick_col_paddle(getBricks());
    if(i!=-1)
    { 
        double sx = getBrick_i_x(i), sy = getBrick_i_y(i);
        double size = getBrick_i_size(i);
        double dy = sy - py - size/2;
        double distance_min = sqrt(pow(r,2) - pow(dy, 2))+epsil_zero; 
        //evite erreur au restart
        if (r<=dy) ; // cout << "Collision limite ignoree" << endl;
        // cas limite on ignore la collision (Epsilon_zero responsable)
        else if (px < sx)
            x_cible = sx - size/2 - distance_min;
        else
            x_cible = sx + size/2 + distance_min;
    }
    pd.updatePaddleX(x_cible);
    setPaddle(pd);
}

bool Game::col_paddle_ball(Ball& ball){
    Paddle pd = getPaddle();
    double x_cible = getSourisX();
    double r = pd.getPaddleRayon(), px = pd.getPaddleX(), py = pd.getPaddleY();
    double vy(0);
    double ball_x = ball.getBallX(), ball_y = ball.getBallY();
    double ball_r = ball.getBallRayon();
    double ball_dx = ball.getDeltaX(), ball_dy = ball.getDeltaY();
    double d_min = sqrt(r*r - py*py);
    if (x_cible > px + delta_norm_max) x_cible = px + delta_norm_max;
    if (x_cible < px - delta_norm_max) x_cible = px - delta_norm_max;
    double vx = x_cible - px;
    if ((px < d_min + 0.5 and x_cible < d_min) ||
     (px > arena_size - d_min - 0.5 and x_cible > arena_size - d_min))
        vx = 0;
    Circle n2_ball(ball_x + ball_dx, ball_y + ball_dy, ball_r);
    Circle n2_paddle(px + vx, py, r);

    return col_paddle_ball2(px,  ball_x,  py, ball_y, r, ball_r,
         vx, vy, ball, n2_ball, n2_paddle, ball_dx, ball_dy);
}

bool Game::col_paddle_ball2(double& px, double& ball_x, double& py, double& ball_y,
    double& r, double& ball_r, double& vx, double& vy,
    Ball& ball, Circle& n2_ball, Circle& n2_paddle, double& ball_dx, double& ball_dy){
    if(intersection_circles(n2_ball, n2_paddle)){
        double dx = px - ball_x;
        double dy = py - ball_y;
        double dist = sqrt(dx*dx + dy*dy);
        dx /= dist;
        dy /= dist;
        double overlap = (r + ball_r) - dist;
        if (overlap > 0) {
        ball_x -= dx * overlap;
        ball_y -= dy * overlap;
        ball.setBallX(ball_x);
        ball.setBallY(ball_y);
    }
        double vn = proj_ortho(vx, vy, dx, dy);
        double vnx = vn*dx;
        double vny = vn*dy;
        double vn_autre = proj_ortho(ball_dx, ball_dy, dx, dy);
        double vn_autre_x = vn_autre*dx;
        double vn_autre_y = vn_autre*dy;
        double impulseX = (-vn_autre_x + vnx)*2;
        double impulseY = (-vn_autre_y + vny)*2;
        ball_dx += impulseX;
        ball_dy += impulseY;
        if(sqrt(ball_dx*ball_dx + ball_dy*ball_dy) > delta_norm_max){
            double coef = coef_normalisation_Vmax(ball_dx, ball_dy);
            ball_dx /= coef;
            ball_dy /= coef;
        }
        ball.setBall_dx(ball_dx);
        ball.setBall_dy(ball_dy);
        return true;
    }
    return false;
    }

bool Game::col_ball_brick(int& i, int& j){
    double bx = balls[i].getBallX();
    double by = balls[i].getBallY(), br = balls[i].getBallRayon();
    double b_dx = balls[i].getDeltaX(), b_dy = balls[i].getDeltaY();
    Circle circle(bx + b_dx, by + b_dy, br);
    double br_x = bricks[j]->getBrickX();
    double br_y = bricks[j]->getBrickY(), br_size = bricks[j]->getBrickSize();
    Square square(br_x, br_y, br_size);
    br_size /= 2;

    if(intersection_circle_square(circle, square))
    {
    double diff_x = bx - br_x, diff_y = by - br_y;

    double diff_bx = clamp(diff_x, -br_size, br_size);
    double diff_by = clamp(diff_y, -br_size, br_size);
    // la fonction clamp met une limite à la val de diff_bx et diff_by

        double dn_x = diff_x - diff_bx;
        double dn_y = diff_y - diff_by;

        double dist = sqrt(dn_x*dn_x + dn_y*dn_y);
        if (dist >= epsil_zero) {
            dn_x /= dist;
            dn_y /= dist;
        }
        double vn = proj_ortho(b_dx, b_dy, dn_x, dn_y);
        double vnx = vn*dn_x;
        double vny = vn*dn_y;

        b_dx -= 2*vnx;
        b_dy -= 2*vny;

        bricks[j]->brickHit(i, balls, bricks, j);
        player.hitScore();
        balls[i].setBall_dx(b_dx);
        balls[i].setBall_dy(b_dy);
        return true;
    }
    return false;
}

void Game::moveBalls(){
    int i = balls.size() - 1;
    while (i >= 0) { //Pour chaque balle
        if (balls[i].ball_sortie()) {
            swap(balls[i], balls.back());
            balls.pop_back();
            i--;
            continue;
            //detruit balle si sortie et passe a la suivante
        }
        bool collision= true;
        unsigned int nb_rebonds = 0; //verifie rebonds pour eviter loop infini
        while (collision && nb_rebonds < nb_bounce_max){
            collision = false; //hypothese de depart : recommence si collision
            boucle_ball_ball(nb_rebonds, i, collision); //Collision ball-ball
            boucle_ball_arena_paddle(nb_rebonds, i, collision); 
            boucle_ball_brick(nb_rebonds, i, collision); //Collision ball-brick
        }
        if (nb_rebonds < nb_bounce_max){
            balls[i].moveBall();
        }
        i--; //sinon on passe a la suivante pour eviter loop infini
    }
}

bool Game::gameFinished() {
    if (player.getLives()<=0 and balls.empty()){
        cout << message::lost() << endl;
        status = LOSE;
        return true;
    }
    if (bricks.empty()){
        cout << message::won() << endl;
        status = WIN;
        player.winScore();
        return true;
    }
    return false;
}

Status Game::getStatus() const {
    return status;
}

void Game::moveBalls_after_paddle() {
    int i(balls.size()-1);
    while(i>=0){
        if (col_paddle_ball(balls[i])) { //Si coll avec paddle,
            balls[i].moveBall(); 
            bool collision = true;
            unsigned int nb_rebonds = 0;
            while (collision && nb_rebonds < nb_bounce_max) {
                collision = false;//hypothese
                boucle_ball_ball(nb_rebonds, i, collision); //Collision ball-ball
                boucle_ball_brick(nb_rebonds, i, collision); //Collision ball-brick
                boucle_ball_arena_paddle(nb_rebonds, i, collision);
                if (collision) continue;
                if (nb_rebonds < nb_bounce_max && nb_rebonds > 0)
                    balls[i].moveBall();
            }
        }
        i--;
    }
}

void Game::boucle_ball_ball(unsigned int &nb_rebonds, int i, bool &collision) {
    if (collision) return;
    for(size_t j = 0; j < balls.size() && nb_rebonds< nb_bounce_max; ++j){
        if (i != j){
            if (balls[i].col_ball_ball(balls[j])){
                nb_rebonds++;
                collision = true;
                return;
            }
        }
    }
}

void Game::boucle_ball_arena_paddle(unsigned int &nb_rebonds, int i, bool &collision){
    if (collision) return;
    if (balls[i].col_arena() or col_paddle_ball(balls[i])){
        nb_rebonds++;
        collision = true;
    }
}

void Game::boucle_ball_brick(unsigned int &nb_rebonds, int i, bool &collision) {
    if (collision) return;
    int j(0);
    while (j<bricks.size()){
        if (col_ball_brick(i, j)){
            collision = true;
            nb_rebonds++;
            return;
        }
        j++;
    }
}