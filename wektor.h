#ifndef WEKTOR_SP
#define WEKTOR_SP
#include <math.h>
// ----------------------------------------------------------------------------
struct WEK
{
  double x, y, z;
  public:
  WEK( void):                             x(0.),  y(0.),  z(0.)  {}
  WEK( double px, double py, double pz):  x(px),  y(py),  z(pz)  {}
  WEK( double px, double py):             x(px),  y(py),  z(0.)  {}
  WEK( const WEK &P):                     x(P.x), y(P.y), z(P.z) {}
  //---------------------------------------------------------------
  inline WEK& operator+=(const WEK &A){x+=A.x; y+=A.y; z+=A.z; return( *this);}
  inline WEK& operator-=(const WEK &A){x-=A.x; y-=A.y; z-=A.z; return( *this);}
  inline WEK& operator*=(const WEK &A){x*=A.x; y*=A.y; z*=A.z; return( *this);}
  inline WEK& operator*=(const double a){x*=a; y*=a;   z*=a;   return( *this);}
  inline WEK& operator=(const WEK &A){x=A.x;  y=A.y;  z=A.z;  return( *this);}
  inline WEK operator+()           {                        return( *this);}
  inline WEK operator-()           {                return( WEK(-x,-y,-z));}
};
// ----------------------------------------------------------------------------
inline WEK operator+(const WEK &A, const WEK &B){return( WEK( A.x+B.x, A.y+B.y, A.z+B.z));}
inline WEK operator-(const WEK &A, const WEK &B){return( WEK( A.x-B.x, A.y-B.y, A.z-B.z));}
inline WEK operator*(const WEK &A, const WEK &B){return( WEK( A.x*B.x ,A.y*B.y ,A.z*B.z));}
// ----------------------------------------------------------------------------
inline WEK operator*(const WEK&A, double b){return( WEK( A.x*b, A.y*b, A.z*b));}
inline WEK operator*(double a, const WEK&B){return( WEK( a*B.x, a*B.y, a*B.z));}
// ----------------------------------------------------------------------------
inline int operator==(const WEK &A, const WEK &B){return int(A.x==B.x&&A.y==B.y&&A.z==B.z);}
inline int operator!=(const WEK &A, const WEK &B){return int(A.x!=B.x||A.y!=B.y||A.z!=B.z);}
// ----------------------------------------------------------------------------
inline double suma( const WEK &A)  { return(A.x+A.y+A.z);}
inline double sumakw( const WEK &A){ return(A.x*A.x +A.y*A.y+ A.z*A.z);}
inline double modul( const WEK &A) { return sqrt( sumakw( A));}
inline double faza( const WEK &A)  {  return ( atan2(A.y, A.x));}
// --------------------------------------------------------------------------
inline double MS(const WEK&A,const WEK &B){ return(A.x*B.x+A.y*B.y+A.z*B.z);}
inline WEK MW(const WEK &A, const WEK &B) { return WEK(A.y*B.z-A.z*B.y,
                                            A.z*B.x-A.x*B.z, A.x*B.y-A.y*B.x);}
// --------------------------------------------------------------------------
inline WEK wersor( WEK &A)
{
  double m=modul(A);
  if( m!=0) m=1/m;
  return WEK( m*A);
}
// --------------------------------------------------------------------------
#endif
