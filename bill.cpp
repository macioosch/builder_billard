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
bool placingwhite = false;
bool canplacewhite = false;
int mousex=0,mousey=0;
int swingt = 0;
bool allballshalted = true;
WEK swing;
double EKC = 0.0;
double maxEKC = 0.0;

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

  ball(void): r(WEK()), v(WEK()) {};
  ball(double _x, double _y): r(WEK(_x,_y)), v(WEK()) {};
  ball(WEK _r): r(_r), v(WEK()) {};

  inline ball& operator=(const ball &A){r=A.r;  return( *this);}
  inline ball& operator=(const WEK &_r){r=_r;  return( *this);}
};

ball b[16];
void correctpos(ball &b1, ball &b2);
void correctpos(ball &b, WEK A);
void correctpos(ball &b, double x, double y);
void simcorrectpos(ball &b1, ball b2);

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
    SBChange(Sender); 
    PlaceBalls();
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
    if (canshoot) simshot();
    drawballs();
    if (canshoot) drawcue();
    maitainplacingwhiteball();

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

    double d = 2.0*ballr*1.05;
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
		b[i].v = WEK();
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
        if (placingwhite) {
            WEK mr = WEK(rx(X),ry(Y));
            if (canplaceballhere(mr)) {
                placingwhite = false;
                canplacewhite = false;
                b[0].r = mr;
                b[0].v = WEK();
                b[0].ontable = true;
                canshoot = true;
            }
        }
		else if (canshoot) {
			shooting = true;
			ShootingTimer->Enabled = true;
		}
    }
    if(Shift.Contains(ssRight)) {
        shooting = false;
        ShootingTimer->Enabled = false;
	   	swingt = 0;
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
        maxEKC = 0.0;

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
    collideballs();
    updateballpositions();
}
//---------------------------------------------------------------------------
void TForm1::updateballpositions()
{
    allballshalted = true;
    double eta = 5*pow(10,-4);
    WEK a;

    EKC = 0.0;

    for (int i=0; i<16; i++)
    if (b[i].ontable)
    {
		b[i].r += b[i].v*dt;
		a = -wersor(b[i].v)*(400.0+eta*pow(modul(b[i].v),2));
		if (modul(b[i].v)<modul(a*dt)) b[i].v = WEK();
		else b[i].v += a*dt;                 
		if (modul(b[i].v)<2000.0) b[i].v = b[i].v*0.99;
		if (modul(b[i].v)<1000.0) b[i].v = b[i].v*0.99;
		if (modul(b[i].v)<20.0) b[i].v = WEK();

        EKC += 0.05*pow(modul(b[i].v)/1000.,2.)/2.0;

		if (b[i].v != WEK()) allballshalted = false;
    }
           
    if (allballshalted && ! placingwhite){
		ComputeTimer->Enabled = false;
		canshoot = true;
    }

    if (EKC > maxEKC) maxEKC = EKC;
    if (maxEKC > 0) EnergyBar->Position = EnergyBar->Max*EKC/maxEKC;
    else EnergyBar->Position = 0;
}
//---------------------------------------------------------------------------
void TForm1::collideballs()
{
    double t_xb = tablew/2.0 - ballr;  // vertical boundry
    double t_yb = tableh/2.0 - ballr;  // horizontal boundry

    double socr = ballr*socketratio;   // half of a socket's side length

    for (int i=0; i<16; i++)
    if (b[i].ontable)
    {
        {  // don't want to use x,y,vx,vy in ball-ball collision!
		double x = b[i].r.x;
		double y = b[i].r.y;
		double vx = b[i].v.x;
		double vy = b[i].v.y;

        if (abs(y)>t_yb) {                  // jest blisko kraw�dzi
            if (abs(x)<socr) {              // jest blisko �uzy
                if (abs(y)>tableh/2.0) {    // wpad�a do �uzy!
                    b[i].ontable = false;
                    continue;
                }
                double distfromvertex = modul( WEK( abs(b[i].r.x), abs(b[i].r.y) )
                                               - WEK(socr, tableh/2.) );
                if (distfromvertex < ballr) {   // collision with vertex
                    WEK rvi;
                    for (int up=-1; up<=2; up+=2)
                    for (int right=-1; right<=2; right+=2) {
                        rvi = b[i].r - WEK(right*socr,up*tableh/2.);
                        if (modul(rvi) < ballr)
                        if (MS(b[i].v,wersor(rvi)) < 0.0) {
                            correctpos(b[i], WEK(right*socr,up*tableh/2.));
                            b[i].v -= 2*MS(b[i].v,wersor(rvi))*wersor(rvi);
                        }
                    }
                }
            }
		    else if (y*vy>0.0) {
                correctpos(b[i], tablew/2.0, tableh/2.0);
                b[i].v.y = -vy;  // collision with wall
            }                    // if not with vertex
        }
        else if (abs(x)>tablew/2.0-socr*sqrt(2) && abs(y)>tableh/2.0-socr*sqrt(2)) {
            // czy wpadnie do k�towej �uzy?
            if (abs(y)>-abs(x)+(tablew+tableh)/2.0-socr*sqrt(2)) { // wpad�a do �uzy!
                    b[i].ontable = false;
                    continue;
            }
            double dfromv1 = modul( WEK( abs(b[i].r.x), abs(b[i].r.y) )
                                    - WEK( tablew/2.-socr*sqrt(2), tableh/2. ) );
            double dfromv2 = modul( WEK( abs(b[i].r.x), abs(b[i].r.y) )
                                    - WEK( tablew/2., tableh/2.-socr*sqrt(2) ) );
            if (dfromv1<ballr||dfromv2<ballr) {   // collision with vertex
                WEK rvi;
                for (int up=-1; up<=2; up+=2)
                for (int right=-1; right<=2; right+=2)
                for (int second=0; second<=1; second++) {
                    rvi = b[i].r - WEK(right*(tablew/2.-second*socr*sqrt(2)),
                                       up*(tableh/2.-(1-second)*socr*sqrt(2)) );
                    if (modul(rvi) < ballr)
                    if (MS(b[i].v,wersor(rvi)) < 0.0) {
                        correctpos(b[i], WEK( right*(tablew/2.-second*socr*sqrt(2)),
                                              up*(tableh/2.-(1-second)*socr*sqrt(2)) ));
                        b[i].v -= 2*MS(b[i].v,wersor(rvi))*wersor(rvi);
                    }
                }
            }
        }
		else if (abs(x)>t_xb && x*vx>0.0) {
            correctpos(b[i], tablew/2.0, tableh/2.0);
            b[i].v.x = -vx;
        }
        }   // end of local namespace

		for (int j=i+1; j<16; j++)
        if (b[j].ontable)
		{
			WEK rij = b[j].r - b[i].r;

			if (modul(rij) < 2.*ballr) {
                correctpos(b[i],b[j]);
				WEK vjb = b[j].v - b[i].v;
				double ms_vif = MS(vjb,wersor(rij));
				if (ms_vif < 0.0) {
					WEK vif = ms_vif*wersor(rij);
					WEK wer_prost = MW(wersor(rij),WEK(0.,0.,1.));
					WEK vjf = MS(vjb,wer_prost)*wer_prost;
					b[j].v = b[i].v+vjf;
					b[i].v = b[i].v+vif;
				}
			}
		}
	}
}       
//---------------------------------------------------------------------------
void TForm1::drawwek(WEK A, double x, double y, double scale)
{
    Obraz->Canvas->MoveTo(ex(x),ey(y));  
    Obraz->Canvas->Pen->Width = 2;
    Obraz->Canvas->LineTo(ex(x+A.x*scale),ey(y+A.y*scale));
    Obraz->Canvas->Pen->Width = 4;
    Obraz->Canvas->Pen->Color = rgb(255,0,0);
    Obraz->Canvas->LineTo(ex(x+A.x*scale),ey(y+A.y*scale)); 
    Obraz->Canvas->Pen->Width = 1;
    PaintBox->Canvas->Draw(0,0,Obraz);
    Sleep(10000);
}
//---------------------------------------------------------------------------
void TForm1::maitainplacingwhiteball()
{
    if (b[0].ontable == false) {
        canshoot = false;
        placingwhite = true;

        WEK mr = WEK(rx(mousex),ry(mousey));

        canplacewhite = canplaceballhere(mr);

        if (canplacewhite)
		    Obraz->Canvas->Brush->Color = b[0].clr;
        else
		    Obraz->Canvas->Brush->Color = rgb(255,0,0);

		Obraz->Canvas->Ellipse( ex(mr.x-ballr), ey(mr.y-ballr),
			                    ex(mr.x+ballr), ey(mr.y+ballr) );
    }
}
//---------------------------------------------------------------------------
bool TForm1::canplaceballhere(WEK A)
{
        double t_xb = tablew/2.0 - ballr;  // vertical boundry
        double t_yb = tableh/2.0 - ballr;  // horizontal boundry

        double socr = ballr*socketratio;   // half of a socket's side length
        
    	double x = A.x;
		double y = A.y;

        if (abs(y)>t_yb) {                  // jest blisko kraw�dzi
            if (abs(x)<socr) {              // jest blisko �uzy
                if (abs(y)>tableh/2.0) return false;
                double distfromvertex = modul( WEK(abs(A.x),abs(A.y))
                                               - WEK(socr,tableh/2.) );
                if (distfromvertex < ballr) return false;
            }
            else return false;
        }
        else if (abs(x)>tablew/2.0-socr*sqrt(2) && abs(y)>tableh/2.0-socr*sqrt(2))
        {
            // czy wpadnie do k�towej �uzy?
            if (abs(y)>-abs(x)+(tablew+tableh)/2.0-socr*sqrt(2)) return false;
            double dfromv1 = modul( WEK( abs(A.x), abs(A.y) )
                                    - WEK( tablew/2.-socr*sqrt(2), tableh/2. ) );
            double dfromv2 = modul( WEK( abs(A.x), abs(A.y) )
                                    - WEK( tablew/2., tableh/2.-socr*sqrt(2) ) );
            if (dfromv1<ballr||dfromv2<ballr) return false;
        }
		else if (abs(x)>t_xb) return false;

		for (int j=1; j<16; j++)
        if (b[j].ontable) {
			WEK rij = b[j].r - A;
            if (modul(rij) < 2.*ballr) return false;
		}

        return true;
}
//---------------------------------------------------------------------------
void TForm1::simshot()
{
    WEK mr = WEK(rx(mousex),ry(mousey));
    WEK mr_w = wersor(mr-b[0].r);

    double t_xb = tablew/2.0 - ballr;  // vertical boundry
    double t_yb = tableh/2.0 - ballr;  // horizontal boundry

    double socr = ballr*socketratio;   // half of a socket's side length
    
    double ds = ballr*0.5*ScrollBarDS->Position/ScrollBarDS->Max;
    int steps = 100*ScrollBarSteps->Position/ds+0.5;

    WEK r0 = b[0].r;
    WEK r1 = r0 - mr_w*300;
    WEK kier = -mr_w;
    
    Obraz->Canvas->Pen->Color = rgb(255,255,255);
    Obraz->Canvas->Pen->Width = 2;
    Obraz->Canvas->MoveTo(ex(r0.x), ey(r0.y));

    for (int i=0; i<steps; i++) {
        r1 = r0+kier*ds;
        Obraz->Canvas->LineTo(ex(r1.x), ey(r1.y));

        double x = r1.x;
		double y = r1.y;
        WEK dr = r1-r0;
        
        ball virtb;
        virtb.r = r1;
        virtb.v = dr;

        if (abs(y)>t_yb) {                  // jest blisko kraw�dzi
            if (abs(x)<socr) {              // jest blisko �uzy
                if (abs(y)>tableh/2.0) {    // wpad�a do �uzy!
                    break;
                }
                double distfromvertex = modul( WEK(abs(r1.x),abs(r1.y))
                                               - WEK(socr,tableh/2.) );
                if (distfromvertex < ballr) {   // collision with vertex
                    WEK rvi;
                    for (int up=-1; up<=2; up+=2)
                    for (int right=-1; right<=2; right+=2) {
                        rvi = r1 - WEK(right*socr,up*tableh/2.);
                        if (modul(rvi) < ballr)
                        if (MS(r1-r0,wersor(rvi)) < 0.0) {
                            correctpos(virtb,WEK(abs(r1.x),abs(r1.y))
                                               - WEK(socr,tableh/2.));
                            r1 = virtb.r;
                            kier = wersor(kier - 2*MS(dr,wersor(rvi))*wersor(rvi));
                        }
                    }
                }
            }
		    else if (y*dr.y>0.0) {
                correctpos(virtb,tablew/2.0,tableh/2.0);
                r1 = virtb.r;
                kier.y = -kier.y;  // collision with wall
            }                      // if not with vertex
        }
        else if (abs(x)>tablew/2.0-socr*sqrt(2) && abs(y)>tableh/2.0-socr*sqrt(2)) {
            // czy wpadnie do k�towej �uzy?
            if (abs(y)>-abs(x)+(tablew+tableh)/2.0-socr*sqrt(2)) { // wpad�a do �uzy!
                    break;
            }
            double dfromv1 = modul( WEK( abs(r1.x), abs(r1.y) )
                                    - WEK( tablew/2.-socr*sqrt(2), tableh/2. ) );
            double dfromv2 = modul( WEK( abs(r1.x), abs(r1.y) )
                                    - WEK( tablew/2., tableh/2.-socr*sqrt(2) ) );
            if (dfromv1<ballr||dfromv2<ballr) {   // collision with vertex
                WEK rvi;
                for (int up=-1; up<=2; up+=2)
                for (int right=-1; right<=2; right+=2)
                for (int second=0; second<=1; second++) {
                    rvi = r1 - WEK(right*(tablew/2.-second*socr*sqrt(2)),
                                   up*(tableh/2.-(1-second)*socr*sqrt(2)) );
                    if (modul(rvi) < ballr)
                    if (MS(dr,wersor(rvi)) < 0.0) {
                        correctpos(virtb,WEK(right*(tablew/2.-second*socr*sqrt(2)),
                                             up*(tableh/2.-(1-second)*socr*sqrt(2))) );
                        r1 = virtb.r;
                        kier = wersor(kier - 2*MS(dr,wersor(rvi))*wersor(rvi));
                    }
                }
            }
        }
		else if (abs(x)>t_xb && x*dr.x>0.0) {
            correctpos(virtb,tablew/2.0,tableh/2.0);
            r1 = virtb.r;
            kier.x = -kier.x;
        }

		for (int j=1; j<16; j++)
        if (b[j].ontable)
		{
			WEK rij = b[j].r - r1;

			if (modul(rij) < 2.*ballr) {
				WEK vjb = b[j].v - dr;
				double ms_vif = MS(vjb,wersor(rij));
				if (ms_vif < 0.0) {
                    simcorrectpos(virtb,b[j]);
					WEK vif = ms_vif*wersor(rij); 
					WEK wer_prost = MW(wersor(rij),WEK(0.,0.,1.));
					WEK vjf = MS(vjb,wer_prost)*wer_prost;

                    Obraz->Canvas->Pen->Color = b[j].clr;
                    Obraz->Canvas->MoveTo(ex(b[j].r.x),ey(b[j].r.y));
                    WEK odbicie = b[j].r+10000*wersor(vjf+dr);
                    Obraz->Canvas->LineTo(ex(odbicie.x),ey(odbicie.y));
                    Obraz->Canvas->MoveTo(ex(r1.x),ey(r1.y));
                    Obraz->Canvas->Pen->Color = rgb(255,255,255);

					kier = wersor(dr+vif);
				}
			}
		}

        r0 = r1;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SBChange(TObject *Sender)
{
    LabelDS->Caption = 0.5*ScrollBarDS->Position/ScrollBarDS->Max;
    LabelSteps->Caption = int(100*ScrollBarSteps->Position+0.5)/1000.0;
}
//---------------------------------------------------------------------------
void correctpos(ball &b1, ball &b2)
{
    WEK r12 = b2.r-b1.r;
    double korekta = 2.0*ballr-modul(r12);
    if (korekta > 0.0) {
        WEK r12_w = wersor(r12);
        double v1r = abs(MS(b1.v,r12_w));
        double v2r = abs(MS(b2.v,r12_w));
        if (v1r+v2r != 0.0) {
            b1.r -= wersor(b1.v)*(korekta*v1r/(v1r+v2r));
            b2.r -= wersor(b2.v)*(korekta*v2r/(v1r+v2r));
        }
    }
}
//---------------------------------------------------------------------------
void correctpos(ball &b, WEK A)
{
    WEK r = b.r-A;
    double korekta = ballr-modul(r);
    if (korekta > 0.0) {
        b.r -= wersor(b.v)*korekta;
    }
}
//---------------------------------------------------------------------------
void correctpos(ball &b, double x, double y)
{
    for (int i=-1; i<=1; i+=2) {
        WEK rx = b.r + WEK(i*x, b.r.y);
        WEK ry = b.r + WEK(b.r.x, i*y);
        if (modul(rx) < ballr && b.v.x!=0.0) {
            double korekta = ballr-modul(rx);
            b.r -= wersor(b.v)*(korekta*modul(b.v)/abs(b.v.x));
        }
        if (modul(ry) < ballr && b.v.y!=0.0) {   
            double korekta = ballr-modul(ry);
            b.r -= wersor(b.v)*(korekta*modul(b.v)/abs(b.v.y));
        }
    }
}
//---------------------------------------------------------------------------
void simcorrectpos(ball &b1, ball b2)
{
    WEK r12 = b2.r-b1.r;
    double korekta = 2.0*ballr-modul(r12);
    if (korekta > 0.0) {
        WEK r12_w = wersor(r12);
        double v1r = abs(MS(b1.v,r12_w));
        double v2r = abs(MS(b2.v,r12_w));
        if (v1r+v2r != 0.0) {
            b1.r -= wersor(b1.v)*(korekta*v1r/(v1r+v2r));
            b2.r -= wersor(b2.v)*(korekta*v2r/(v1r+v2r));
        }
    }
}
//---------------------------------------------------------------------------
