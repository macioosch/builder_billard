//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "bill.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
Graphics::TBitmap *Obraz=NULL;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
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
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
        Obraz->Width=PaintBox->Width;
        Obraz->Height=PaintBox->Height;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
        delete Obraz;
        Obraz=NULL;
}
//---------------------------------------------------------------------------
