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
bool canshoot = true;
bool shooting = false;
int mousex=0,mousey=0;
int swingt = 0;
bool allballshalted = true;
WEK swing;

double dt;

double minborder = 50.0;    // on screen, around the table

// all following dimensions in mm

double tablew = 2540.0;     // other popular values: 2240, 1980
double tableh = tablew/2.0; // all American tables are 2:1
double ballr = 28.6;        // radius
double socketratio = 2.0;   // radius of socket / ballr

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
    if (w*th > h*tw) return h/th;
    else return w/tw;
}
//---------------------------------------------------------------------------
int rgb(int r, int g, int b) { return r+256*(g+256*b); }
//---------------------------------------------------------------------------
double ex(double x) { return pbw+x*s; }
double ey(double y) { return pbh-y*s;  }
double rx(double x) { return (x-pbw)/s; }
double ry(double y) { return -(y-pbh)/s; }
//---------------------------------------------------------------------------
void __fastcall TForm1::FormActivate(TObject *Sender)
{
    dt = ComputeTimer->Interval/1000.0;

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

    double r = ballr*socketratio;

    Obraz->Canvas->Pen->Color = rgb(80,50,30);
    Obraz->Canvas->Pen->Width = 4;
    Obraz->Canvas->Brush->Color = rgb(160,100,60);
    Obraz->Canvas->Rectangle( ex(-tablew/2-3*r), ey(-tableh/2-3*r),
							  ex( tablew/2+3*r), ey( tableh/2+3*r) );

    Obraz->Canvas->Pen->Color = rgb(35,50,70);
    Obraz->Canvas->Pen->Width = 4;
    Obraz->Canvas->Brush->Color = rgb(67,103,135);
    Obraz->Canvas->Rectangle( ex(-tablew/2), ey(-tableh/2),
							  ex( tablew/2), ey( tableh/2) );

    Obraz->Canvas->Pen->Color = rgb(35,50,70);
    Obraz->Canvas->Pen->Width = 4;
    Obraz->Canvas->Brush->Color = rgb(17,25,35);
    TPoint points[4];
    for (int i=0;i<3;i++)
    for (int j=0;j<2;j++){
		double x = ex(tablew*(-0.5 + i/2.0));
		double y = ey(tableh*(-0.5 + j));
		if (i==1) {
			points[0] = Point(x-s*r,y);
			points[1] = Point(x+s*r,y);
			points[2] = Point(x+s*r,y-s*(2*j-1)*2*r);
			points[3] = Point(x-s*r,y-s*(2*j-1)*2*r);
		}
		else {
			points[0] = Point(x-s*r*sqrt(2),y);
			points[1] = Point(x,y+s*r*sqrt(2));
			points[2] = Point(x+s*r*sqrt(2),y);
			points[3] = Point(x,y-s*r*sqrt(2));
		}
		Obraz->Canvas->Polygon(points,3);
    }

    // draw helper markings

    Obraz->Canvas->Pen->Color = rgb(232,168,41);
    Obraz->Canvas->Pen->Width = 1;
    Obraz->Canvas->Brush->Color = rgb(255,203,48);
    for (int rig=0; rig<2; rig++)
    {
		for (int xi=1; xi<4; xi++)
		for (int yi=0; yi<2; yi++)
		{
			double x = (rig-xi/4.0)*tablew/2.0;
			double y = (2*yi-1)*(tableh/2.0+r);

			points[0] = Point(ex(x-15),ey(y));
			points[1] = Point(ex(x),ey(y+25));
			points[2] = Point(ex(x+15),ey(y));
			points[3] = Point(ex(x),ey(y-25));

			Obraz->Canvas->Polygon(points,3);
		}

		for (int yi =0; yi <3; yi ++)
		{
			double x = (2*rig-1)*(tablew/2.0+r);
			double y = (yi-1)*tableh/4.0;
			
			points[0] = Point(ex(x-25),ey(y));
			points[1] = Point(ex(x),ey(y+15));
			points[2] = Point(ex(x+25),ey(y));
			points[3] = Point(ex(x),ey(y-15));
			Obraz->Canvas->Polygon(points,3);
		}
    }
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
    if (canshoot) drawcue();

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
    b[0].r   = WEK(-tablew/4.0, 0);
    b[0].v   = WEK();
    b[0].clr = getballcolor(0);
    b[0].full = false;
    b[0].ontable = true;

    double d = 2.0*ballr*1.01;
    double dx = d*sqrt(3.0)/2.0;
    double dy = d/2.0;

    WEK r0 = WEK(tablew/4.0, 0);
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
		b[i].v   = WEK();
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
    double ws = ballr*0.5;   // radius of the inner white circle in some balls

    Obraz->Canvas->Pen->Color = rgb(40,40,40);
    Obraz->Canvas->Pen->Width = 1;

    for (int i=0; i<=15; i++)
    if (b[i].ontable)
    {
		Obraz->Canvas->Brush->Color = b[i].clr;
		Obraz->Canvas->Ellipse( ex(b[i].r.x-ballr), ey(b[i].r.y-ballr),
			ex(b[i].r.x+ballr), ey(b[i].r.y+ballr) );
		if (b[i].full) {
				Obraz->Canvas->Pen->Width = 0;
				Obraz->Canvas->Pen->Color = rgb(255,248,204);
				Obraz->Canvas->Brush->Color = rgb(255,248,204);
				Obraz->Canvas->Ellipse( ex(b[i].r.x-ws), ey(b[i].r.y-ws),
										ex(b[i].r.x+ws), ey(b[i].r.y+ws) );
				Obraz->Canvas->Pen->Width = 1;
				Obraz->Canvas->Pen->Color = rgb(40,40,40);
		}
    }
}
//---------------------------------------------------------------------------
void TForm1::drawcue()
{
    WEK mr = WEK(rx(mousex),ry(mousey));
    WEK mr_w = wersor(mr-b[0].r);

    double maxswing = 500;
    double swingspeed = 5;
    swing = mr_w*maxswing*(1.0-exp(-swingt*swingspeed/maxswing));

    WEK rcue1 = swing + b[0].r + mr_w*ballr*1;
    WEK rcue0 = swing + b[0].r + mr_w*1500;

    WEK rgrip1 = swing + b[0].r + mr_w*1100;
    WEK rgrip0 = swing + b[0].r + mr_w*1475;

    // cue
    Obraz->Canvas->Pen->Color = rgb(181,152,112);
    Obraz->Canvas->Pen->Width = s*25;
    Obraz->Canvas->MoveTo(ex(rcue0.x),ey(rcue0.y));
    Obraz->Canvas->LineTo(ex(rcue1.x),ey(rcue1.y));

    // tip
    Obraz->Canvas->Pen->Color = rgb(216,238,255);
    Obraz->Canvas->MoveTo(ex(rcue1.x),ey(rcue1.y));
    Obraz->Canvas->LineTo(ex(rcue1.x),ey(rcue1.y));

    // grip
    Obraz->Canvas->Pen->Color = rgb(88,86,73);
    Obraz->Canvas->Pen->Width = s*35;
    Obraz->Canvas->MoveTo(ex(rgrip0.x),ey(rgrip0.y));
    Obraz->Canvas->LineTo(ex(rgrip1.x),ey(rgrip1.y));

}
//---------------------------------------------------------------------------
void __fastcall TForm1::CanShootPaintBoxPaint(TObject *Sender)
{
    int imw = CanShootPaintBox->Width; 
    int imh = CanShootPaintBox->Height;

    if (canshoot) {
		CanShootPaintBox->Canvas->Brush->Color = rgb(0,255,0);
		CanShootLabel->Caption = "Shoot!";
    }
    else {
		CanShootPaintBox->Canvas->Brush->Color = rgb(255,0,0);
		CanShootLabel->Caption = "Wait...";
    }
    
    CanShootPaintBox->Canvas->Ellipse(0,0,imw,imh);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintTimerTimer(TObject *Sender)
{       
    CanShootPaintBoxPaint(Sender);
    PaintBoxPaint(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PaintBoxMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    mousex = X;
    mousey = Y;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PaintBoxMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Shift.Contains(ssLeft))
    {
		if (canshoot) {
			shooting = true;
			ShootingTimer->Enabled = true;
		}
    }
}
//--------------------------------------------------------------------------- 
void __fastcall TForm1::PaintBoxMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (shooting) {
		shooting = false;
		canshoot = false;
		ShootingTimer->Enabled = false;

		WEK mr = WEK(rx(mousex),ry(mousey));
		WEK mr_w = wersor(mr-b[0].r);

		double vinswing = 50;
		b[0].v = -swing*vinswing;

		swingt = 0;

		ComputeTimer->Enabled = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ShootingTimerTimer(TObject *Sender)
{
    swingt++;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ComputeTimerTimer(TObject *Sender)
{
    updateballpositions();
    collideballs();        
}
//---------------------------------------------------------------------------
void TForm1::updateballpositions()
{
    allballshalted = true;
    double eta = 5*pow(10,-4);
    WEK a;

    for (int i=0; i<16; i++)
    if (b[i].ontable)
    {
		b[i].r += b[i].v*dt;
		a = -wersor(b[i].v)*(200.0+eta*pow(modul(b[i].v),2));
		if (modul(b[i].v)<modul(a*dt)) b[i].v = WEK();
		else b[i].v += a*dt;                 
		if (modul(b[i].v)<1000.0) b[i].v = b[i].v*0.995;
		if (modul(b[i].v)<200.0) b[i].v = b[i].v*0.995;
		if (modul(b[i].v)<20.0) b[i].v = WEK();

		if (b[i].v != WEK()) allballshalted = false;
    }
           
    if (allballshalted){
		ComputeTimer->Enabled = false;
		canshoot = true;
    }
}
//---------------------------------------------------------------------------
void TForm1::collideballs()
{
    double t_xb = tablew/2.0 - ballr;  // vertical boundry
    double t_yb = tableh/2.0 - ballr;  // horizontal boundry

    for (int i=0; i<16; i++)
    if (b[i].ontable)
    {
		double x = b[i].r.x;
		double y = b[i].r.y;
		double vx = b[i].v.x;
		double vy = b[i].v.y;

		if (abs(x)>t_xb && x*vx>0.0) b[i].v.x = -vx;
		if (abs(y)>t_yb && y*vy>0.0) b[i].v.y = -vy;

		for (int j=i+1; j<16; j++)
		{
			WEK rij = b[j].r - b[i].r;

			if (modul(rij) < 2*ballr) {
				WEK vjb = b[j].v - b[i].v;
				double ms_vif = MS(vjb,wersor(rij));
				Button1->Caption = ms_vif;
				if (ms_vif < 0.0) {
					WEK vif = ms_vif*wersor(rij);
					double ms_vjf = sqrt(abs( pow(modul(vjb),2)-pow(ms_vif,2) ));
					WEK vjf = ms_vjf*WEK( sqrt(rij.y/(rij.x+rij.y)),
										  sqrt(rij.x/(rij.x+rij.y)) );
					b[j].v = b[i].v+vjf;
					b[i].v = b[i].v+vif;
				}
			}
		}
	}
}       
//---------------------------------------------------------------------------
void TForm1::drawwek(WEK A, double x, double y, double scale=1.0)
{
    Obraz->Canvas->MoveTo(ex(x),ey(y));
    Obraz->Canvas->LineTo(ex(x+A.x*scale),ey(y+A.y*scale));
}
//---------------------------------------------------------------------------

