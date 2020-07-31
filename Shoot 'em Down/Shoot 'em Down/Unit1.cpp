//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <iostream.h>
#include <fstream.h>
#include <math.h>
#include <string.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
struct lvl_stats
{
    int gamemode;
    char *title; // description
    TColor title_color; // color of level title
    int ammo; int duck_count;
    bool allow_gold_duck;
    int gold_duck_count;
    int duck_speed; // length in pixels per tick
};
lvl_stats level[7];
const int complete_levels = 6;
int current_level = 1;

int ammo;

const int max_duck = 10;
int kill_count = 0;
int duck_count;
int gold_duck_count = 0;

struct duck_stat
{   int dirX; // 0 - left, 1 - right
    int dirY; // 0 - down, 1 - up
    bool can_hit;
    bool can_respawn; };
duck_stat duck[max_duck];

struct img_duck
{   TImage *image; };
img_duck duck_img[max_duck];

bool on_pause = false;

//===========================================================================
//---------------------------------------------------------------------------
void file_read(char *filename, lvl_stats *level, int lvl_id)
{
    ifstream f(filename);

    f>>level[lvl_id].gamemode;

    if (level[lvl_id].gamemode>0) // 1 - easy, 2 - medium, 3 - hard, 4 - insane
    {
        switch (level[lvl_id].gamemode) // 1-5 => default presets
        {
            case 1:
            {
                level[lvl_id].title = "Easy mode";
                level[lvl_id].title_color = 0x60FF00; // HEX RGB color palette   //light smooth green
                level[lvl_id].duck_count = 8; level[lvl_id].ammo = 10;
                level[lvl_id].allow_gold_duck = true; level[lvl_id].gold_duck_count = 1;
                level[lvl_id].duck_speed = 6; // length in pixels per tick
                break;
            }
            case 2:
            {
                level[lvl_id].title = "Normal mode";
                level[lvl_id].title_color = 0x60EEEE; // HEX RGB color palette   //light yellow
                level[lvl_id].duck_count = 12; level[lvl_id].ammo = 13;
                level[lvl_id].allow_gold_duck = true; level[lvl_id].gold_duck_count = 2;
                level[lvl_id].duck_speed = 11; // length in pixels per tick
                break;
            }
            case 3:
            {
                level[lvl_id].title = "Hard mode";
                level[lvl_id].title_color = 0x4040FF; // HEX RGB color palette   //red
                level[lvl_id].duck_count = 16; level[lvl_id].ammo = 17;
                level[lvl_id].allow_gold_duck = true; level[lvl_id].gold_duck_count = 3;
                level[lvl_id].duck_speed = 15; // length in pixels per tick
                break;
            }
            case 4:
            {
                level[lvl_id].title = "Insane mode";
                level[lvl_id].title_color = 0xFF30AA; // HEX RGB color palette   //lit up violet
                level[lvl_id].duck_count = 24; level[lvl_id].ammo = 24;
                level[lvl_id].allow_gold_duck = true; level[lvl_id].gold_duck_count = 1;
                level[lvl_id].duck_speed = 20; // length in pixels per tick
                break;
            }
            case 5: // HARDCORE => insane speed + one shot-one kill
            {
                level[lvl_id].title = "HARDCORE";
                level[lvl_id].title_color = 0x0000AA; // HEX RGB color palette   //dark red
                level[lvl_id].duck_count = 32; level[lvl_id].ammo = 32;
                level[lvl_id].allow_gold_duck = false; level[lvl_id].gold_duck_count = 0;
                level[lvl_id].duck_speed = 20; // length in pixels per tick
                break;
            }
            default: // more than 6 => custom difficulty
            {
                level[lvl_id].title = "CUSTOM";
                level[lvl_id].title_color = 0x00D0FF; // bright orange

                f>>level[lvl_id].duck_count;
                f>>level[lvl_id].ammo;
                f>>level[lvl_id].duck_speed;

                f>>level[lvl_id].allow_gold_duck; //if gold duck allowed, next value is number of ducks, else next
                if (level[lvl_id].allow_gold_duck) f>>level[lvl_id].gold_duck_count;
            }
        }
    }
    else  // 0 - Infinite mode
    {
    // maybe in future
    }

    f.close();
}
//-----------------------------------------
void Default(duck_stat *duck) // default duck stats while begin new level or restart
{
    for (int i=0; i<max_duck; i++)
    {
        duck[i].can_hit = false;
        duck[i].dirX = 1; // 0 = left, 1 = right;
        duck[i].dirY = 1; // 0 = down, 1 = up;
        duck[i].can_respawn = true;
    }
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    DoubleBuffered = true;
    randomize();
    srand(time(NULL));

    Screen->Cursors[crNoDrop] = LoadCursorFromFile(".\\cursor\\X-cross.cur");
    Screen->Cursor = crNoDrop;

    Default(duck);

    duck_img[1].image = img_duck1;
    duck_img[2].image = img_duck2;
    duck_img[3].image = img_duck3;
    duck_img[4].image = img_duck4;
    duck_img[5].image = img_duck5;
    duck_img[6].image = img_duck6;
    duck_img[7].image = img_duck7;
    duck_img[8].image = img_duck8;
    duck_img[9].image = img_golden_duck;

    /*for (int i=1; i<=complete_levels; i++)
    {
        char *id, *lvl;
        AnsiString a = IntToStr(i);
        id = a.c_str();
        lvl = strcat(".\\levels\\level_", id);
        lvl = strcat(lvl, ".txt");
        file_read(lvl, level, i); // file_read doesn't read the file
    } // */
    file_read(".\\levels\\level_1.txt", level, 1); //
    file_read(".\\levels\\level_2.txt", level, 2);
    file_read(".\\levels\\level_3.txt", level, 3);
    file_read(".\\levels\\level_4.txt", level, 4);
    file_read(".\\levels\\level_5.txt", level, 5);
    file_read(".\\levels\\level_6.txt", level, 6); // */

    ammo = level[current_level].ammo;
    gold_duck_count = level[current_level].gold_duck_count;
}
//---------------------------------------------------------------------------
//===========================================================================
void change_image(TImage *img, TImageList *list, int image_id)
{
    TRect rect(0,0,img->Width, img->Height);
    img->Canvas->Brush->Color = clBlack;
    img->Canvas->FillRect(rect);
    list->Draw(img->Canvas, 0, 0, image_id);
}
//---------------------------------------------------------------------------
void ammo_update()
{
    if (ammo>=16)
    {
        change_image(Form1->img_ammo2, Form1->bullet_list, ammo-16);
        change_image(Form1->img_ammo, Form1->bullet_list, 16);
    }
    else change_image(Form1->img_ammo, Form1->bullet_list, ammo);
}
//--------------------------------------
void miss()
{
    ammo--;
    if (ammo>=0) ammo_update();
    Form1->ammo_count->Caption = IntToStr(ammo);
}
//--------------------------------------
void duck_kill(TImage *image, int duckID)
{
    miss();
    if (ammo>=0)
    {
        duck[duckID].can_hit = false; kill_count++;
        change_image(image, Form1->duck_list, duck[duckID].dirX+2);
        Form1->lable_kill_counter->Caption = "Ducks killed -- "+IntToStr(kill_count);
        image->SendToBack(); Form1->img_sky_bg->SendToBack();
    }
}
//--------------------------------------
void objective_lable() // draw label relatively center of lable and screen
{
    Form1->lable_objective->Font->Color = level[current_level].title_color;
    Form1->lable_objective->Caption = level[current_level].title;
    Form1->lable_objective->Left = (Form1->Width/2) - (Form1->lable_objective->Width/2);
}
//===========================================================================

void __fastcall TForm1::img_sky_bgClick(TObject *Sender)
{
    miss();
}
void __fastcall TForm1::img_grass_bgClick(TObject *Sender)
{
    miss();
}
void __fastcall TForm1::img_ammoClick(TObject *Sender)
{
    miss();
}
void __fastcall TForm1::ammo_countClick(TObject *Sender)
{
    miss();
}
//---------------------------------------------------------------------------
//===========================================================================
void pause_game(AnsiString text, AnsiString text2) // now you can make some tea between waves
{
    if (!on_pause) // works one time
    {
        Form1->lable_kill_counter->Font->Color = 0x907D23; // dark smooth blue
        Form1->label_aim->Font->Color = 0x136C54; // dark smooth green
        Form1->ReSpawner->Enabled = false;
        Form1->duck_timer->Enabled = false;
        Form1->TIME->Enabled = false;
        Form1->img_pause->BringToFront();
        Form1->pause_label->Caption = text;
        Form1->pause_label->Left = (Form1->Width/2) - (Form1->pause_label->Width/2);
        Form1->pause_label2->Caption = text2;
        Form1->pause_label2->Left = (Form1->Width/2) - (Form1->pause_label2->Width/2);
        Form1->pause_label->Visible = true; Form1->pause_label->BringToFront();
        Form1->pause_label2->Visible = true; Form1->pause_label2->BringToFront();
        Form1->img_pause->Visible = true;
    }
}
void unpause_game() //...and continue "Shootin 'em down!"
{
    Form1->lable_kill_counter->Font->Color = 0x000000; // clBlack
    Form1->label_aim->Font->Color = 0x000000;
    Form1->ReSpawner->Enabled = true;
    Form1->duck_timer->Enabled = true;
    Form1->TIME->Enabled = true;
    Form1->img_pause->SendToBack();
    Form1->img_pause->Visible = false;
    Form1->pause_label->Visible = false;
    Form1->pause_label2->Visible = false;
    Form1->lable_kill_counter->Caption = "Ducks killed -- "+IntToStr(kill_count);
}
//----------------------------------------------------
void __fastcall TForm1::img_pauseClick(TObject *Sender)
{
    unpause_game(); // works one time when pause image was clicked
}
void __fastcall TForm1::FormKeyPress(TObject *Sender, char &Key) // pause the game!
{
    if (Key == 27) pause_game("GAME PAUSED","click to continue");
}
//===========================================================================
//---------------------------------------------------------------------------
void __fastcall TForm1::TIMETimer(TObject *Sender)
{
    srand(time(NULL)); // why random is too slow
    objective_lable();
    label_aim->Caption = "You need to kill "+IntToStr(level[current_level].duck_count)+" ducks";

    if (!Form1->Active) pause_game("GAME PAUSED","click to continue"); // if form isn't active => you need a break

    if (ammo>0)
    {
        ammo_update();
        ammo_count->Caption = IntToStr(ammo);
    }
    for (int i=0; i<max_duck-1; i++)
    {
        if (duck[i].can_hit)
        {
            ammo_update();
        }
    }
}
//---------------------------------------------------------------------------
//===========================================================================

void __fastcall TForm1::img_duck1Click(TObject *Sender)
{
    if (duck[1].can_hit) duck_kill(duck_img[1].image, 1);
    else miss();
}
void __fastcall TForm1::img_duck2Click(TObject *Sender)
{
    if (duck[2].can_hit) duck_kill(duck_img[2].image, 2);
    else miss();
}
void __fastcall TForm1::img_duck3Click(TObject *Sender)
{
    if (duck[3].can_hit) duck_kill(duck_img[3].image, 3);
    else miss();
}
void __fastcall TForm1::img_duck4Click(TObject *Sender)
{
    if (duck[4].can_hit) duck_kill(duck_img[4].image, 4);
    else miss();
}
void __fastcall TForm1::img_duck5Click(TObject *Sender)
{
    if (duck[5].can_hit) duck_kill(duck_img[5].image, 5);
    else miss();
}
void __fastcall TForm1::img_duck6Click(TObject *Sender)
{
    if (duck[6].can_hit) duck_kill(duck_img[6].image, 6);
    else miss();
}
void __fastcall TForm1::img_duck7Click(TObject *Sender)
{
    if (duck[7].can_hit) duck_kill(duck_img[7].image, 7);
    else miss();
}
void __fastcall TForm1::img_duck8Click(TObject *Sender)
{
    if (duck[8].can_hit) duck_kill(duck_img[8].image, 8);
    else miss();
}
//===========================================================================
//---------------------------------------------------------------------------
void __fastcall TForm1::img_golden_duckClick(TObject *Sender)
{
    if (duck[9].can_hit)
    {
        if (ammo>0)
        {
            duck[9].can_hit = false;
            if (ammo+5 < 32) ammo+=5;
            else ammo = 32;
            change_image(duck_img[9].image, Form1->gold_duck_list, duck[9].dirX+2);
            duck_img[9].image->SendToBack(); Form1->img_sky_bg->SendToBack();
        }
    }
}
//---------------------------------------------------------------------------
//===========================================================================

void duck_quickmove(TImage *img, int step, bool &need_move, duck_stat *duck, int id)
{
    int midX = img->Left + img->Width/2;
    int midY = img->Top + img->Height/2;
    //--------------------
    int destX, destY;

    if (duck[id].dirX==1) destX = midX+60; // sorta AI
    else destX = midX-60;
    if (duck[id].dirY==1) destY = midY-40;
    else destY = midY+30;

    int dx = destX - midX;
    int dy = destY - midY;
    //--------------------
    if (id<9) change_image(duck_img[id].image, Form1->duck_list, duck[id].dirX); // change image from direction
    else change_image(duck_img[9].image, Form1->gold_duck_list, duck[9].dirX);

    if (id == 9) step*=1.5; // gold duck flies faster than other ducks in 1.5 times
    //--------------------
    if(abs(dx)<step && abs(dy)<step)
    {
        need_move = false;
        return;
    }
    need_move = true;
    double angle = atan2(dy, dx);
    img->Left += step*cos(angle);
    img->Top += step*sin(angle);
}
void duck_downmove(TImage *img, bool &need_move, duck_stat *duck, int id)
{
    if (img->Top >= 400)
    {
        need_move = false;
        duck[id].can_respawn = true;
        return;
    }
    need_move = true;
    if (id<9) img->Top +=10;
    else img->Top +=15; // gold duck falls faster
}
//===========================================================================
//---------------------------------------------------------------------------
void NextLevel()
{
    kill_count = 0;
    pause_game("LEVEL COMPLETE","click to go next level");
    Default(duck); current_level++;
    ammo = level[current_level].ammo;
    gold_duck_count = level[current_level].gold_duck_count;
}
void ReStart()
{
    kill_count = 0;
    pause_game("OUT OF AMMO","click to restart");
    Default(duck);
    ammo = level[current_level].ammo;
    gold_duck_count = level[current_level].gold_duck_count;
}
//---------------------------------------------------------------------------
//===========================================================================
void duck_respawn(TImage *img, duck_stat *duck, int id)
{
    duck[id].can_hit = true; img->Visible = true;
    duck[id].can_respawn = false;
    duck[id].dirX = 1; duck[id].dirY = 1;
    img->Top = 400; img->Left = 100+random(1000)-img->Width/2;
}
int one_duck_was_hitted (duck_stat *duck)
{
    bool duck_hitted = false;
    for (int i=1; i<max_duck-1; i++)
    {
        duck_hitted = duck_hitted && duck[i].can_respawn;
        if (duck[i].can_respawn == true) return i;
    }
    return 0;
}
void __fastcall TForm1::ReSpawnerTimer(TObject *Sender)
{
    bool duck_hitted = false;
    for (int i=1; i<max_duck-1; i++)
    {
        duck_hitted = duck_hitted || duck[i].can_respawn;
    }
    if (duck_hitted) // if at least one duck hitted
    {
        if (one_duck_was_hitted(duck) != 0) // which duck is hitted
        {
            if (level[current_level].duck_count >= kill_count+8) // if max active ducks are lower than duck objective
            {
                int d = one_duck_was_hitted(duck);
                duck_respawn(duck_img[d].image, duck, d);
            }
        }
    }
    // trying to spawn gold duck
    if (level[current_level].allow_gold_duck) // are golden duck allowed
    {
        if (gold_duck_count > 0) // how much golden ducks can spawn
        {
            if (duck[9].can_respawn) // can current gold duck respawn
            {
                if (random(10)>8) // gold duck respawn randomized
                {
                    gold_duck_count--;
                    duck_respawn(duck_img[9].image, duck, 9);
                }
            }
        }
    }
}
//===========================================================================
//---------------------------------------------------------------------------

void __fastcall TForm1::duck_timerTimer(TObject *Sender) // movin' ducks. will cause bugs
{
    bool need_move;
    srand(time(NULL));

    if (kill_count >= level[current_level].duck_count)
    {
        kill_count = 0;
        NextLevel();
    }
    if (ammo == 0) ReStart();

    for (int i=1; i<max_duck; i++)
    {

        //================ main move
        if (duck[i].can_hit) duck_quickmove(duck_img[i].image, level[current_level].duck_speed, need_move, duck, i);
        else duck_downmove(duck_img[i].image, need_move, duck, i);
        //================ left and right borders
        if (duck_img[i].image->Left+duck_img[i].image->Width >= 1100) duck[i].dirX = 0;
        else if (duck_img[i].image->Left <= 100) duck[i].dirX = 1;
        //================ up and down dorders
        if (duck_img[i].image->Top >= 300) duck[i].dirY = 1;
        else if (duck_img[i].image->Top <= 50) duck[i].dirY = 0;
        //================ random direction change
        if (random(10)<4)
        {
            duck[i].dirX = 0+random(2);
            // prevent duck fly out of bounds
            if (duck_img[i].image->Left+duck_img[i].image->Width >= 1100) duck[i].dirX = 0;
            else if (duck_img[i].image->Left <= 100) duck[i].dirX = 1; // can cause bugs
        }
        if (random(10)>4)
        {
            duck[i].dirY = 0+random(2);
            // prevent duck fly out of bounds
            if (duck_img[i].image->Top >= 300) duck[i].dirY = 1;
            else if (duck_img[i].image->Top <= 50) duck[i].dirY = 0; // causes bugs too
        }
        // working move
        // as good as useless
    }
}
//---------------------------------------------------------------------------
//===========================================================================
