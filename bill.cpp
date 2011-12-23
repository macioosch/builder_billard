//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "bill.h"
#include "wektor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
Graphics::TBitmap *Obraz=NULL;
int rgb(int r, int g, int b);
int getballcolor(int n);

double pbw,pbh,s;
                            // all following dimensions in mm
double tablew = 2540.0;     // other popular values: 2240, 1980
double tableh = tablew/2.0; // all American tables are 2:1
double ballr = 28.6;        // radius
double socketratio = 2.0;   // radius of socket / ballr

double minborder = 50.0;    // on screen, around the table

//---------------------------------------------------------------------------
struct ball
{
  WEK r,v;
  TColor clr;
  bool full;
  bool ontable;

  ball(): r(WEK()), v(WEK()) {};
  ball(double _x, double _y): r(WEK(_x,_y)), v(WEK()) {};
  ball(WEK _r): r(_r), v(WEK()) {};

  inline ball& operator=(const ball &A){r=A.r;  return( *this);}
  inline ball& operator=(const WEK &_r){r=_r;  return( *this);}
};

ball b[16];

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
double TForm1::Scale()
{
        double w = PaintBox->ClientWidth-2.0*minborder;
        double h = PaintBox->ClientHeight-2.0*minborder;
        double tw = tablew + 3*ballr*socketratio;
        double th = tableh + 3*ballr*socketratio;
        if (w*th > h*tw) {
                return h/th;
        }
        else {
                return w/tw;
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
        return pbw+x*s;
}
//---------------------------------------------------------------------------
double ey(double y)
{
        return pbh-y*s;
}
// --------------------------------------------------------------------------
double rx(double x)
{
        return (x-pbw)/s;
}
//---------------------------------------------------------------------------
double ry(double y)
{
        return (-y-pbh)/s;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormActivate(TObject *Sender)
{
        Obraz = new Graphics::TBitmap();
        Obraz->Width=PaintBox->Width;
        Obraz->Height=PaintBox->Height;
        PaintBoxPaint(Sender);
        PlaceBalls();
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
void TForm1::drawtable()
{

        double r = s*ballr*socketratio;

        Obraz->Canvas->Pen->Color = rgb(80,50,30);
        Obraz->Canvas->Pen->Width = 4;
        Obraz->Canvas->Brush->Color = rgb(160,100,60);
        Obraz->Canvas->Rectangle( pbw-s*tablew/2.0-3*r, pbh-s*tableh/2.0-3*r,
                                  pbw+s*tablew/2.0+3*r, pbh+s*tableh/2.0+3*r );

        Obraz->Canvas->Pen->Color = rgb(35,50,70);
        Obraz->Canvas->Pen->Width = 4;
        Obraz->Canvas->Brush->Color = rgb(67,103,135);
        Obraz->Canvas->Rectangle( pbw-s*tablew/2.0, pbh-s*tableh/2.0,
                                  pbw+s*tablew/2.0, pbh+s*tableh/2.0 );

        Obraz->Canvas->Pen->Color = rgb(35,50,70);
        Obraz->Canvas->Pen->Width = 4;
        Obraz->Canvas->Brush->Color = rgb(17,25,35);
        TPoint points[4];
        for (int i=0;i<3;i++)
        for (int j=0;j<2;j++){
                double x = pbw + s*tablew*(-0.5 + i/2.0);
                double y = pbh + s*tableh*(-0.5 + j);
                if (i==1) {
                        points[0] = Point(x-r,y);
                        points[1] = Point(x+r,y);
                        points[2] = Point(x+r,y+(2*j-1)*2*r);
                        points[3] = Point(x-r,y+(2*j-1)*2*r);
                }
                else {
                        points[0] = Point(x-r*sqrt(2),y);
                        points[1] = Point(x,y+r*sqrt(2));
                        points[2] = Point(x+r*sqrt(2),y);
                        points[3] = Point(x,y-r*sqrt(2));
                }
                Obraz->Canvas->Polygon(points,3);
        }

        Obraz->Canvas->Pen->Color = rgb(0,0,0);
        Obraz->Canvas->Pen->Width = 1;
        Obraz->Canvas->Brush->Color = rgb(255,255,255);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBoxPaint(TObject *Sender)
{
        pbw = Obraz->Width/2.0;
        pbh = Obraz->Height/2.0;

        s = Scale();

        Obraz->Canvas->Brush->Color = rgb(160,170,180);
        Obraz->Canvas->FillRect(PaintBox->ClientRect);

        drawtable();
        drawballs();

        PaintBox->Canvas->Draw(0,0,Obraz);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
        PlaceBalls();
        PaintBoxPaint(Sender);
}
//---------------------------------------------------------------------------
void TForm1::PlaceBalls()
{
        b[0].r   = WEK(pbw-s*tablew/4.0, pbh);
        b[0].clr = getballcolor(0);
        b[0].full = false;
        b[0].ontable = true;

        double d = s*ballr*1.05*2.0;
        double dx = d*sqrt(3.0)/2.0;
        double dy = d/2.0;

        WEK r0 = WEK(pbw+s*tablew/4.0, pbh);
        WEK r = r0;
        int tmp = 1;
        int max = 1;

        for (int i=1; i<=15; i++)
        {
                if (tmp>max) {
                        r.x += dx;
                        r.y = r0.y-max*dy;
                        tmp = 1;
                        max += 1;
                }
                else if (i>1) r.y += 2*dy;

                b[i].r = r;
                b[i].clr = getballcolor(i);

                if (i>8) b[i].full = true;
                else b[i].full = false;
                
                b[i].ontable = true;

                tmp++;
        }

}
//---------------------------------------------------------------------------
int getballcolor(int n)
{
        if (n>8) n -= 8;

        switch (n) {
        case 0:  return rgb(255,248,204); // white [only one]
        case 1:  return rgb(255,216,0);   // yellow
        case 2:  return rgb(45,157,255);  // blue
        case 3:  return rgb(224,0,0);     // red
        case 4:  return rgb(228,96,255);  // violet | pink
        case 5:  return rgb(255,127,0);   // orange
        case 6:  return rgb(124,168,58);  // green
        case 7:  return rgb(160,76,36);   // brown 
        case 8:  return rgb(50,25,0);     // black [only one]
        default: return rgb(255,0,255);   // invalid
        }
}
//---------------------------------------------------------------------------
void TForm1::drawballs()
{
        double ws = 0.6;        // radius of the inner white circle in some balls

        Obraz->Canvas->Pen->Color = rgb(40,40,40);
        Obraz->Canvas->Pen->Width = 1;

        for (int i=0; i<=15; i++)
        {
                Obraz->Canvas->Brush->Color = b[i].clr;
                Obraz->Canvas->Ellipse( b[i].r.x-s*ballr, b[i].r.y-s*ballr,
                                        b[i].r.x+s*ballr, b[i].r.y+s*ballr );
                if (b[i].full) {
                        Obraz->Canvas->Pen->Width = 0;
                        Obraz->Canvas->Pen->Color = rgb(255,248,204);
                        Obraz->Canvas->Brush->Color = rgb(255,248,204);
                        Obraz->Canvas->Ellipse( b[i].r.x-s*ballr*ws, b[i].r.y-s*ballr*ws,
                                                b[i].r.x+s*ballr*ws, b[i].r.y+s*ballr*ws );
                        Obraz->Canvas->Pen->Width = 1;
                        Obraz->Canvas->Pen->Color = rgb(40,40,40);
                }
                //Obraz->Canvas->TextOutA(b[i].r.x, b[i].r.y, i);
        }
}
//---------------------------------------------------------------------------

