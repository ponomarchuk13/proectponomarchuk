//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <jpeg.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TImage *img_sky_bg;
    TImage *img_duck1;
    TImage *img_duck2;
    TImage *img_grass_bg;
    TImage *img_duck3;
    TImage *img_duck4;
    TImage *img_ammo;
    TImageList *bullet_list;
    TTimer *duck_timer;
    TTimer *ReSpawner;
    TTimer *TIME;
    TImageList *duck_list;
    TLabel *ammo_count;
    TLabel *lable_kill_counter;
    TImage *img_duck5;
    TImage *img_duck6;
    TImage *img_duck7;
    TImage *img_duck8;
    TImage *img_golden_duck;
    TImage *img_ammo2;
    TLabel *lable_objective;
    TLabel *Label1;
    TImage *img_pause;
    TLabel *label_aim;
    void __fastcall img_sky_bgClick(TObject *Sender);
    void __fastcall img_grass_bgClick(TObject *Sender);
    void __fastcall TIMETimer(TObject *Sender);
    void __fastcall img_ammoClick(TObject *Sender);
    void __fastcall ammo_countClick(TObject *Sender);
    void __fastcall img_duck2Click(TObject *Sender);
    void __fastcall img_duck1Click(TObject *Sender);
    void __fastcall img_duck3Click(TObject *Sender);
    void __fastcall img_duck4Click(TObject *Sender);
    void __fastcall img_duck5Click(TObject *Sender);
    void __fastcall img_duck6Click(TObject *Sender);
    void __fastcall img_duck7Click(TObject *Sender);
    void __fastcall img_duck8Click(TObject *Sender);
    void __fastcall img_golden_duckClick(TObject *Sender);
    void __fastcall ReSpawnerTimer(TObject *Sender);
    void __fastcall duck_timerTimer(TObject *Sender);
    void __fastcall img_pauseClick(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
