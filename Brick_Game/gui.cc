#include <filesystem>
#include <iostream>
#include "constants.h"
#include "graphic_gui.h"
#include "gui.h"
#include "Paddle.h"
#include "tools.h"
#include "Erreur.h"
#include "Ball.h"

using namespace std;

enum Response
{
    CANCEL,
    OPEN_FILE,
    SAVE_FILE
};
enum Buttons
{
    EXIT,
    OPEN,
    SAVE,
    RESTART,
    START,
    STEP
};

constexpr unsigned drawing_size(500);

My_window::My_window(string file_name)
    : main_box(Gtk::Orientation::HORIZONTAL), panel_box(Gtk::Orientation::VERTICAL),
      command_box(Gtk::Orientation::VERTICAL), loop_activated(false),
      buttons({Gtk::Button("exit"), Gtk::Button("open"), Gtk::Button("save"),
               Gtk::Button("restart"), Gtk::Button("start"), Gtk::Button("step")}),
      info_frame("Infos :"), info_text({Gtk::Label("score:"), Gtk::Label("lives:"),
                                        Gtk::Label("bricks:"), Gtk::Label("balls:")})
{
    set_title("Brick Breaker");
    set_child(main_box);
    main_box.append(panel_box);
    main_box.append(drawing);
    panel_box.append(command_box);
    panel_box.append(info_frame);
    set_commands();
    set_key_controller();
    set_mouse_controller();
    set_infos();
    set_drawing();
    // TODO: set the game
    if (file_name != "")
    {
        game_loaded = game.loadGame(file_name);
    }
    update_infos();
}
void My_window::set_commands()
{
    for (auto &button : buttons)
    {
        command_box.append(button);
        command_box.set_size_request(120, -1);
        button.set_margin(1);
    }

    buttons[EXIT].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::exit_clicked));
    buttons[OPEN].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::open_clicked));
    buttons[SAVE].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::save_clicked));
    buttons[RESTART].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::restart_clicked));
    buttons[START].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::start_clicked));
    buttons[STEP].signal_clicked().connect(
        sigc::mem_fun(*this, &My_window::step_clicked));
}

void My_window::exit_clicked()
{
    hide();
}
void My_window::open_clicked()
{
    auto dialog = new Gtk::FileChooserDialog("Choose a text file",
                                             Gtk::FileChooserDialog::Action::OPEN);
    set_dialog(dialog);
}
void My_window::save_clicked()
{
    auto dialog = new Gtk::FileChooserDialog("Choose a text file",
                                             Gtk::FileChooserDialog::Action::SAVE);
    set_dialog(dialog);
}
void My_window::restart_clicked()
{
    // TODO: reset the game from the last read file
    game_loaded = game.reloadGame();
    drawing.queue_draw();
    update_infos();
    init_gui();
}
void My_window::start_clicked()
{
    if (!game_loaded or game.getStatus() != ONGOING)
        return;
    if (loop_activated)
    {
        loop_conn.disconnect();
        loop_activated = false;
        buttons[EXIT].set_sensitive(true);
        buttons[OPEN].set_sensitive(true);
        buttons[SAVE].set_sensitive(true);
        buttons[RESTART].set_sensitive(true);
        buttons[START].set_label("start");
        buttons[STEP].set_sensitive(true);
    }
    else // TODO: only if the game is not finished
    {
        loop_conn =
            Glib::signal_timeout().connect(sigc::mem_fun(*this, &My_window::loop), dt);
        loop_activated = true;
        buttons[EXIT].set_sensitive(false);
        buttons[OPEN].set_sensitive(false);
        buttons[SAVE].set_sensitive(false);
        buttons[RESTART].set_sensitive(false);
        buttons[START].set_label("stop");
        buttons[STEP].set_sensitive(false);
    }
}
void My_window::step_clicked()
{
    // TODO: make a single update
    if (!game_loaded)
        return;
    game.stepGame();
    if (game.getStatus() != ONGOING) { 
        //quand le jeu est fini on arrete la loop et on reactive certains boutons
        loop_conn.disconnect();
        loop_activated = false;
        buttons[EXIT].set_sensitive(true);
        buttons[OPEN].set_sensitive(true);
        buttons[SAVE].set_sensitive(true);
        buttons[RESTART].set_sensitive(true);
        buttons[START].set_label("start");
        buttons[START].set_sensitive(false);
        buttons[STEP].set_sensitive(false);
    }
    update_infos();
    drawing.queue_draw();
}
void My_window::set_key_controller()
{
    auto contr = Gtk::EventControllerKey::create();
    contr->signal_key_pressed().connect(sigc::mem_fun(*this, &My_window::key_pressed),
                                        false);
    add_controller(contr);
}
bool My_window::key_pressed(guint keyval, guint keycode, Gdk::ModifierType state)
{
    switch (keyval)
    {
    case '1':
        // TODO: make a single update
        if (loop_activated or game.getStatus() != ONGOING)
            return false;
        step_clicked();
        return true;
    case 's':
        // TODO: pause or unpause the game
        if (game.getStatus() != ONGOING)
            return false;
        start_clicked();
        return true;
    case 'r':
        // TODO: reset the game from the last read file
        if (loop_activated)
            return false;
        restart_clicked();
        return true;
    default:
        break;
    }
    return false;
}

void My_window::set_dialog(Gtk::FileChooserDialog *dialog)
{
    dialog->set_modal(true);
    dialog->set_transient_for(*this);
    dialog->set_select_multiple(false);
    dialog->signal_response().connect(
        sigc::bind(sigc::mem_fun(*this, &My_window::dialog_response), dialog));

    dialog->add_button("_Cancel", CANCEL);
    switch (dialog->get_action())
    {
    case Gtk::FileChooserDialog::Action::OPEN:
        dialog->add_button("_Open", OPEN_FILE);
        break;
    case Gtk::FileChooserDialog::Action::SAVE:
        dialog->add_button("_Save", SAVE_FILE);
        break;
    default:
        break;
    }

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_pattern("*.txt");
    dialog->add_filter(filter_text);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog->add_filter(filter_any);

    dialog->show();
}
void My_window::dialog_response(int response, Gtk::FileChooserDialog *dialog)
{
    filesystem::path file_name = "";
    if (dialog->get_file())
    {
        file_name = dialog->get_file()->get_path();
        if (file_name.extension() != ".txt")
        {
            file_name = "";
        }
    }
    switch (response)
    {
    case CANCEL:
        dialog->hide();
        break;
    case OPEN_FILE:
        if (file_name != "")
        {
             // TODO: set game from a file
            game_loaded = game.loadGame(file_name);
            drawing.queue_draw();
            update_infos();
            init_gui();
            dialog->hide();
        }
        break;
    case SAVE_FILE:
        if (file_name != "")
        {   
            // TODO: save the game
            game.saveGame(file_name);
            dialog->hide();
        }
        break;
    default:
        break;
    }
}

bool My_window::loop()
{
    if (loop_activated)
    {
        // TODO: update the game and the interface
        step_clicked();
        
        return true;
    }
    return false;
}

void My_window::set_infos()
{
    info_frame.set_child(info_grid);
    info_grid.set_column_homogeneous(true);
    for (size_t i(0); i < info_text.size(); ++i)
    {
        info_grid.attach(info_text[i], 0, i, 1, 1);
        info_grid.attach(info_value[i], 1, i, 1, 1);
        info_text[i].set_halign(Gtk::Align::START);
        info_value[i].set_halign(Gtk::Align::END);
        info_text[i].set_margin(3);
        info_value[i].set_margin(3);
    }
}

void My_window::update_infos()
// TODO: update the different counters
{
    vector <string> info= {conversion_string(game.getScore()),
                           conversion_string(game.getLives()),
                           conversion_string(game.get_nb_bricks()),
                           conversion_string(game.get_nb_balls())};
    int i = 0;
    for (auto &value : info_value)
    {
        value.set_text(info[i]);
        i++;
    }
}

void My_window::set_drawing()
{
    drawing.set_content_width(drawing_size);
    drawing.set_content_height(drawing_size);
    drawing.set_expand();
    drawing.set_draw_func(sigc::mem_fun(*this, &My_window::on_draw));
}
void My_window::on_draw(const Cairo::RefPtr<Cairo::Context> &cr,
                        int width, int height)
{
    graphic_set_context(cr);
    double side(min(width, height));
    cr->translate((width - side) / 2, (height + side) / 2);
    cr->scale(side / (arena_size), -side / (arena_size));
    // TODO: draw the game
    set_color(GREY);
    if (game_loaded){
        cr->set_line_width(1);
        cr->move_to(0, 0);
        cr->line_to(0, 100);
        cr->line_to(100, 100);
        cr->line_to(100, 0);
        cr->stroke();
    }
    game.drawGame();
}
void My_window::set_mouse_controller()
{
    auto left_click = Gtk::GestureClick::create();
    auto move = Gtk::EventControllerMotion::create();

    left_click->set_button(GDK_BUTTON_PRIMARY);

    left_click->signal_pressed().connect(
        sigc::mem_fun(*this, &My_window::on_drawing_left_click));
    move->signal_motion().connect(sigc::mem_fun(*this, &My_window::on_drawing_move));

    drawing.add_controller(left_click);
    drawing.add_controller(move);
}
void My_window::on_drawing_left_click(int n_press, double x, double y)
{
    int lives = game.getLives();
    if(lives >= 1 and game.get_nb_balls() == 0)
    {
        Paddle paddle = game.getPaddle();
        double px = paddle.getPaddleX();
        double py = paddle.getPaddleY();
        double pr = paddle.getPaddleRayon();
        double y = py + pr + new_ball_radius + epsil_zero;
        Ball ball(px, y, new_ball_radius, 0, new_ball_delta_norm);
        Liste_Ball& balls = game.getBalls();
        balls.push_back(ball);
        game.loseLife();
    }
    drawing.queue_draw();
    update_infos();
}

void My_window::on_drawing_move(double x, double y)
{
    double width = drawing.get_width();
    double height = drawing.get_height();
    double side(min(width, height));
    x = x - (width-side)/2;
    x = x / side * arena_size;
    y = y- (height-side)/2;
    y = -y + side;
    y = y / side * arena_size;
    game.setSouris(x, y);
}

void My_window::init_gui(){
    if (game_loaded){
            buttons[EXIT].set_sensitive(true);
            buttons[OPEN].set_sensitive(true);
            buttons[SAVE].set_sensitive(true);
            buttons[RESTART].set_sensitive(true);
            buttons[START].set_label("start");
        if(game.getStatus()==ONGOING){
            buttons[START].set_sensitive(true);
            buttons[STEP].set_sensitive(true);
        }
        else{
            buttons[START].set_sensitive(false);
            buttons[STEP].set_sensitive(false);
        }
    }
    else{
        buttons[EXIT].set_sensitive(true);
        buttons[OPEN].set_sensitive(true);
        buttons[SAVE].set_sensitive(true);
        buttons[RESTART].set_sensitive(true);
        buttons[START].set_label("start");
        buttons[START].set_sensitive(true);
        buttons[STEP].set_sensitive(true);
    }
}


