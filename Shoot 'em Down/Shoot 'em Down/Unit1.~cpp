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
const int complete_levels = 5;
int current_level = 1;

int ammo;

const int max_duck = 9;
int kill_count = 0;
int duck_count;

struct duck_stat
{   int dirX; // 0 - left, 1 - right
    int dirY; // 0 - down, 1 - up
    bool can_hit; };
duck_stat duck[max_duck];

struct img_duck
{   TImage *image; };
img_duck duck_img[max_duck];

bool on_pause = false;


//---------------------------------------------------------------------------
void file_read(char *filename, lvl_stats *level, int lvl_id)
{
    ifstream f(filename);

    //f>>level[lvl_id].gamemode; //========= do not reading a file
    level[lvl_id].gamemode = 2;

    if (level[lvl_id].gamemode>0) // 1 - easy, 2 - medium, 3 - hard, 4 - insane
    {
        switch (level[lvl_id].gamemode) // 1-5 => default presets
        {
            case 1:
            {
                level[lvl_id].title = "Easy mode";
                level[lvl_id].title_color = 0x60FF00; // HEX RGB color palette   //light smooth green
                level[lvl_id].duck_count = 8; level[lvl_id].ammo = 16;
                level[lvl_id].allow_gold_duck = true; level[lvl_id].gold_duck_count = 1;
                level[lvl_id].duck_speed = 6; // length in pixels per tick
                break;
            }
            case 2:
            {
                level[lvl_id].title = "Normal mode";
                level[lvl_id].title_color = 0x60EEEE; // HEX RGB color palette   //light yellow
                level[lvl_id].duck_count = 12; level[lvl_id].ammo = 24;
                level[lvl_id].allow_gold_duck = true; level[lvl_id].gold_duck_count = 2;
                level[lvl_id].duck_speed = 10; // length in pixels per tick
                break;
            }
            case 3:
            {
                level[lvl_id].title = "Hard mode";
                level[lvl_id].title_color = 0x4040FF; // HEX RGB color palette   //red
                level[lvl_id].duck_count = 16; level[lvl_id].ammo = 24;
                level[lvl_id].allow_gold_duck = true; level[lvl_id].gold_duck_count = 1;
                level[lvl_id].duck_speed = 15; // length in pixels per tick
                break;
            }
            case 4:
            {
                level[lvl_id].title = "Insane mode";
                level[lvl_id].title_color = 0xFF30AA; // HEX RGB color palette   //lit up violet
                level[lvl_id].duck_count = 24; level[lvl_id].ammo = 32;
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
            default:     // more than 6 => custom difficulty
            {
                f>>level[lvl_id].duck_count>>level[lvl_id].ammo;
                f>>level[lvl_id].duck_speed;

                f>>level[lvl_id].allow_gold_duck; //if gold duck allowed, next value is number of ducks, else next
                if (level[lvl_id].allow_gold_duck) f>>level[lvl_id].gold_duck_count;
            }
        }
    }
    else  // 0 - Infinite mode
    {

    }

    f.close();
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

    for (int i=0; i<max_duck; i++)
    {
        duck[i].can_hit = true;
        duck[i].dirX = 1; // 0 = left, 1 = right;
        duck[i].dirY = 1; // 0 = down, 1 = up;
    }

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
        //char *id, *lvl;
        //AnsiString a = IntToStr(i);
        //id = a.c_str();
        //lvl = strcat(".\\levels\\level_", id);
        file_read(".\\levels\\level_1", level, i); // file_read doesn't read the file
    } // */
    file_read(".\\levels\\level_1", level, 1); //
    /*file_read(".\\levels\\level_2", level, 2);
    file_read(".\\levels\\level_3", level, 3);
    file_read(".\\levels\\level_4", level, 4);
    file_read(".\\levels\\level_5", level, 5);
    file_read(".\\levels\\level_6", level, 6); // */

    ammo = level[current_level].ammo;
}
//---------------------------------------------------------------------------
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
//------------------------------------
void miss()
{
    ammo--;
    if (ammo>=0)
    {
        ammo_update();
    }
    Form1->ammo_count->Caption = IntToStr(ammo);
}
//------------------------------------
void duck_kill(TImage *image, int duckID)
{
    miss();
    if (ammo>=0)
    {
        //image->Visible = false;
        duck[duckID].can_hit = false; kill_count++;
        change_image(image, Form1->duck_list, duck[duckID].dirX+2);
        Form1->lable_kill_counter->Caption = "Ducks killed -- "+IntToStr(kill_count);
        image->SendToBack(); Form1->img_sky_bg->SendToBack();
    }
} // */
//------------------------------------
void duck_REspawn(TImage *image, int duckID)
{
    image->Visible = true;
    duck[duckID].can_hit = true;
}
//------------------------------------
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
void pause_game() // now you can make some tea between waves
{
    if (!on_pause)
    {
        Form1->ReSpawner->Enabled = false;
        Form1->duck_timer->Enabled = false;
        Form1->TIME->Enabled = false;
        Form1->img_pause->BringToFront();
        //change_image(Form1->img_pause, Form1->screen_list, 1);
        Form1->img_pause->Visible = true;
    }
}
void unpause_game() //...and continue "Shootin 'em down!"
{
    Form1->ReSpawner->Enabled = true;
    Form1->duck_timer->Enabled = true;
    Form1->TIME->Enabled = true;
    Form1->img_pause->SendToBack();
    Form1->img_pause->Visible = false;
    //change_image(Form1->img_pause, Form1->screen_list, 0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::img_pauseClick(TObject *Sender)
{
    unpause_game();
}
void __fastcall TForm1::FormKeyPress(TObject *Sender, char &Key)// pause the game!
{
    if (Key = 27) pause_game();
}
//===========================================================================
//---------------------------------------------------------------------------
void __fastcall TForm1::TIMETimer(TObject *Sender)
{
    srand(time(NULL)); // i hate ya, slow randomizer
    //Label1->Caption = IntToStr(0+random(100));
    objective_lable();
    label_aim->Caption = "You need to kill "+IntToStr(level[current_level].duck_count)+" ducks";

    if (!Form1->Active) pause_game(); // you need a break

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
        //miss();
        if (ammo>0)
        {
            //img_golden_duck->Visible = false;
            //duck[8].can_hit = false;
            if (ammo+5 < 32) ammo+=5;
            else ammo = 32;
            //change_image(image, Form1->duck_list, duck[duckID].dir+2);
            //Form1->lable_kill_counter->Caption = "Ducks killed -- "+IntToStr(kill_count);
        }
    }
}
//---------------------------------------------------------------------------
//===========================================================================

void duck_quickmove(TImage *img, int step, bool &need_move, duck_stat *duck, int id)
{
    int midX = img->Left + img->Width/2;
    int midY = img->Top + img->Height/2;
    int destX, destY;

    if (duck[id].dirX==1) destX = midX+60;
    else destX = midX-60;
    if (duck[id].dirY==1) destY = midY-40;
    else destY = midY+30;

    int dx = destX - midX;
    int dy = destY - midY;
    //   0-left 1-right
    /*if (duck[id].dirX==1) dx = (midX-50) - midX; // move right
    else dx = (midX+50) - midX;                   // move left
    if (duck[id].dirX==0) dy = (midY-80) - midY; // move up
    else dy = (midY+30) - midY; // */                  // move down

    change_image(duck_img[id].image, Form1->duck_list, duck[id].dirX);
    if(abs(dx)<step && abs(dy)<step)
    {
        need_move = false;
        return;
    }
    need_move = true;
    double angle = atan2(dy, dx);
    img->Left += step*cos(angle);
    img->Top += step*sin(angle);
} // */
void duck_downmove(TImage *img, bool &need_move, duck_stat *duck, int id)
{
    if (img->Top >= 400)
    {
        need_move = false;
        return;
    }
    need_move = true;
    img->Top +=10;
}
void gold_duck_move(TImage *img, duck_stat *duck, int id)
{
//
}
//---------------------------------------------------------------------------
void NextLevel()
{
    //Form1->duck_timer->Enabled = false;
    ShowMessage("Congrats, u make it");

}
//---------------------------------------------------------------------------
void duck_respawn(TImage *img, duck_stat *duck, int id)
{
    duck[id].can_hit = true; img->Visible = true;
    img->Top = 400; img->Left = 100+random(1000)-img->Width/2;
}
int one_duck_was_hitted (duck_stat *duck)
{
    bool duck_hitted = false;
    for (int i = 1; i<max_duck; i++)
    {
        duck_hitted = duck_hitted && duck[i].can_hit;
        if (duck[i].can_hit == false) return i;
    }
    return 0;
}
void __fastcall TForm1::ReSpawnerTimer(TObject *Sender)
{
//  something does not working again                  i'm so proud about it
    bool duck_hitted = false;
    for (int i = 1; i<max_duck; i++)
    {
        duck_hitted = duck_hitted && duck[i].can_hit;
    }
    if (duck_hitted)
    {
        if (level[current_level].duck_count-max_duck+1 >= max_duck )
        {
            int d = one_duck_was_hitted(duck);
            duck_respawn(duck_img[d].image, duck, d);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::duck_timerTimer(TObject *Sender) // movin' ducks
{
    bool need_move;
    srand(time(NULL));

    if (kill_count >= level[current_level].duck_count)
    {
        kill_count = 0;
        NextLevel();
    }
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
        if (random(10)==3)
        {
            duck[i].dirX = 0+random(2);
        }
        if (random(10)==9)
        {
            duck[i].dirY = 0+random(2); // does it even working?
        }
        // almost working move
    }
}
//---------------------------------------------------------------------------


