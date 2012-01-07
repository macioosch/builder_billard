//---------------------------------------------------------------------------

#ifndef billH
#define billH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "wektor.h"
#include <ComCtrls.hpp>
#include <CheckLst.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TButton *Button1;
        TPaintBox *PaintBox;
        TPaintBox *CanShootPaintBox;
        TLabel *CanShootLabel;
        TTimer *PaintTimer;
        TTimer *ShootingTimer;
        TTimer *ComputeTimer;
    TProgressBar *EnergyBar;
    TLabel *Label1;
    TScrollBar *ScrollBarDS;
    TScrollBar *ScrollBarSteps;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *LabelDS;
    TLabel *LabelSteps;
    TCheckBox *CheckBox1;
    TPaintBox *PaintBoxBalls;
    TButton *Button2;
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall PaintBoxPaint(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall CanShootPaintBoxPaint(TObject *Sender);
        void __fastcall PaintTimerTimer(TObject *Sender);
        void __fastcall PaintBoxMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall PaintBoxMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ShootingTimerTimer(TObject *Sender);
        void __fastcall PaintBoxMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ComputeTimerTimer(TObject *Sender);
    void __fastcall SBChange(TObject *Sender);
    void __fastcall PaintBoxBallsPaint(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
private:	// User declarations   
        double Scale();
        void drawtable();
        void PlaceBalls();
        void drawballs();
        void drawcue();
        void updateballpositions();
        void collideballs();
        void maitainplacingwhiteball();
        bool canplaceballhere(WEK A);
        void drawwek(WEK A, double x, double y, double scale=1.0);
        void simshot();
        void drawsocketedballs();
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
