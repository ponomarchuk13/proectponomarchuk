//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <iostream.h>
#include <fstream.h>
#include <math.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
struct lvl_stats
{
    int gamemode;
    int ammo; int duck_count;
    bool allow_gold_duck;
    int gold_duck_count;
};
lvl_stats level[1];
int ammo = 8;

const int max_duck = 9;
int kill_count = 0;
int duck_count;

struct duck_stat
{   int x; int y; int dir;
    bool can_hit; };
duck_stat duck[max_duck];

struct img_duck
{   TImage *image; };
img_duck duck_img[max_duck];

//---------------------------------------------------------------------------
void file_read(char *filename, lvl_stats *level, int lvl_id)
{
    ifstream f(filename);

    f>>level[lvl_id].gamemode;
    if (level[lvl_id].gamemode>0) //1 - easy, 2 - medium, 3 - hard, 4 - insane
    {
        if (level[lvl_id].gamemode == 5) //HARDCORE => insane speed + one shot-one kill
        {

        }
        else // more than 6 => custom difficulty
        {
            f>>level[lvl_id].duck_count>>level[lvl_id].ammo;

            f>>level[lvl_id].allow_gold_duck; //if gold duck allowed, next value is number of ducks, else next
            if (level[lvl_id].allow_gold_duck) f>>level[lvl_id].gold_duck_count;
        }
        switch (level[lvl_id].gamemode)
        {
            case 1:
            {
                level[lvl_id].duck_count = 5;
                level[lvl_id].ammo = 16;
                break;
            }
            case 2:
            {

            }
            case 3:
            {

            }
            case 4:
            {

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
    Screen->Cursors[crNoDrop] = LoadCursorFromFile(".\\cursor\\RB-cross.cur");
    Screen->Cursor = crNoDrop;

    for (int i=0; i<max_duck; i++)
    {
        duck[i].can_hit = true;
        duck[i].dir = 0; //0 = left, 1 = right;
    }

    duck_img[0].image = img_duck1;
    duck_img[1].image = img_duck2;
    duck_img[2].image = img_duck3;
    duck_img[3].image = img_duck4;
    duck_img[4].image = img_duck5;
    duck_img[5].image = img_duck6;
    duck_img[6].image = img_duck7;
    duck_img[7].image = img_duck8;
    duck_img[8].image = img_golden_duck;

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
void miss()
{
    ammo--;
    if (ammo>=0)
    {
        ammo_update();
    }
    Form1->ammo_count->Caption = IntToStr(ammo);
}

void duck_kill(TImage *image, int duckID)
{
    miss();
    if (ammo>=0)
    {
        //image->Visible = false;
        duck[duckID].can_hit = false; kill_count++;
        change_image(image, Form1->duck_list, duck[duckID].dir+2);
        Form1->lable_kill_counter->Caption = "Ducks killed -- "+IntToStr(kill_count);
    }
} // */

void duck_respawn(TImage *image, int duckID)
{
    image->Visible = true;
    duck[duckID].can_hit = true;
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

void __fastcall TForm1::TIMETimer(TObject *Sender)
{
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
//===========================================================================
//---------------------------------------------------------------------------

void __fastcall TForm1::img_duck1Click(TObject *Sender)
{
    if (duck[0].can_hit) duck_kill(duck_img[0].image, 0);
    else miss();
}
void __fastcall TForm1::img_duck2Click(TObject *Sender)
{
    if (duck[1].can_hit) duck_kill(duck_img[1].image, 1);
    else miss();
}
void __fastcall TForm1::img_duck3Click(TObject *Sender)
{
    if (duck[2].can_hit) duck_kill(duck_img[2].image, 2);
    else miss();
}
void __fastcall TForm1::img_duck4Click(TObject *Sender)
{
    if (duck[3].can_hit) duck_kill(duck_img[3].image, 3);
    else miss();
}
void __fastcall TForm1::img_duck5Click(TObject *Sender)
{
    if (duck[4].can_hit) duck_kill(duck_img[4].image, 4);
    else miss();
}
void __fastcall TForm1::img_duck6Click(TObject *Sender)
{
    if (duck[5].can_hit) duck_kill(duck_img[5].image, 5);
    else miss();
}
void __fastcall TForm1::img_duck7Click(TObject *Sender)
{
    if (duck[6].can_hit) duck_kill(duck_img[6].image, 6);
    else miss();
}
void __fastcall TForm1::img_duck8Click(TObject *Sender)
{
    if (duck[7].can_hit) duck_kill(duck_img[7].image, 7);
    else miss();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::img_golden_duckClick(TObject *Sender)
{
    if (duck[8].can_hit)
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

/*void duck_move(TImage *img, const GraphicPoint &dest, int step, bool &need_move)
{
    int midX = img->Left + img->Width/2;
    int midY = img->Top + img->Height/2;
    int dx = dest.x - midX;
    int dy = dest.y - midY;
    if(abs(dx)<step && abs(dy)<step)
    {
        need_move = false;
        return;
    }
    need_move = true;
    double angle = atan2(dy, dx);
    img->Left += step*cos(angle);
    img->Top += step*sin(angle);
    //health->Left = img->Left+img->Width/2-health->Width/2;
    //health->Top = img->Top+img->Height+10;
} // */
//---------------------------------------------------------------------------
