//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "bill.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
Graphics::TBitmap *Obraz=NULL;
                            // all following dimensions in mm
double tablew = 2540.0;     // other popular values: 2240, 1980
double tableh = tablew/2.0; // all American tables are 2:1
double ballr = 28.6;        // radius

double minborder = 50.0;    // on screen, around the table

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
double TForm1::Scale()
{                        
        double w = PaintBox->ClientWidth - 2.0*minborder;
        double h = PaintBox->ClientHeight - 2.0*minborder;
        if (w*tableh > h*tablew) {
                return h / tableh;
        }
        else {
                return w / tablew;
        }
}
//---------------------------------------------------------------------------
int rgb(int r, int g, int b)
{
        return r+256*(g+256*b);
}
//---------------------------------------------------------------------------
double ex(double x)
{
        return (0.5+0.5*x/10.)*Form1->PaintBox->ClientWidth;
}
//---------------------------------------------------------------------------
double ey(double y)
{
        return (0.5-0.5*y/10.)*Form1->PaintBox->ClientHeight;
}
// --------------------------------------------------------------------------
double rx(double x)
{
        return 20*(x/Form1->PaintBox->ClientWidth - 0.5);
}
//---------------------------------------------------------------------------
double ry(double y)
{
        return -20*(y/Form1->PaintBox->ClientHeight - 0.5);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormActivate(TObject *Sender)
{
        Obraz = new Graphics::TBitmap();
        Obraz->Width=PaintBox->Width;
        Obraz->Height=PaintBox->Height;
        PaintBoxPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
        Obraz->Width=PaintBox->Width;
        Obraz->Height=PaintBox->Height;
        PaintBoxPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
        delete Obraz;
        Obraz=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBoxPaint(TObject *Sender)
{
        double pbw = Obraz->Width/2.0;
        double pbh = Obraz->Height/2.0;

        double s = Scale();
        
        Obraz->Canvas->Brush->Color = rgb(160,170,180);
        Obraz->Canvas->FillRect(PaintBox->ClientRect);

        Obraz->Canvas->Pen->Color = rgb(80,50,30);
        Obraz->Canvas->Pen->Width = 4;
        Obraz->Canvas->Brush->Color = rgb(67,103,135);
        Obraz->Canvas->Rectangle( pbw-s*tablew/2.0, pbh-s*tableh/2.0,
                                  pbw+s*tablew/2.0, pbh+s*tableh/2.0 );
        
        Obraz->Canvas->Pen->Color = rgb(35,50,70);
        Obraz->Canvas->Pen->Width = 2;
        Obraz->Canvas->Brush->Color = rgb(50,75,100);
        double r = s*ballr*2.0;
        for (int i=0;i<3;i++)
        for (int j=0;j<2;j++){
                double x = pbw + s*tablew*(-0.5 + i/2.0);
                double y = pbh + s*tableh*(-0.5 + j);
                Obraz->Canvas->Ellipse(x-r,y-r,x+r,y+r);
        }

        Obraz->Canvas->Pen->Color = rgb(0,0,0);
        Obraz->Canvas->Pen->Width = 1; 
        Obraz->Canvas->Brush->Color = rgb(255,255,255);

        PaintBox->Canvas->Draw(0,0,Obraz);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
        PaintBoxPaint(Sender);
}
//---------------------------------------------------------------------------


