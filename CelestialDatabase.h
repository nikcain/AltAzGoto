#ifndef celestialdatabase_h
#define celestialdatabase_h

// Celestial Database
#include "Arduino.h"

#define c_rads 0.01745329251994329576923690768489
#define c_degs 57.295779513082320876798154814105
#define pi 3.1415926535897932384626433832795

#define mylatitude 52.6027972
#define mylongitude -3.0954659

#define myYear 2024
#define myMonth 4
#define myDay  1

struct AltAzPosition
{
  float alt;
  float az;  
};
struct RaDecPosition
{
  float ra;
  float dec;  
};
class CelestialGotoObject {
  public:
    CelestialGotoObject();
    AltAzPosition getCurrentAltAzPosition(int hour, int minute);
    RaDecPosition getRaDec(int hour, int minute);
    bool isAboveHorizon(int hour, int minute);

    bool isValid;
    double FNdegmin(double xx);
    double dayno(int dx, int mx, int yx, double fx);
    double frange(double x);
    double fkep(double m, double ecc);
    double fnatan(double x, double y);
    void earth();

    String id;
    String name;

    float rightascension;
    float declination;

    bool isPlanet;
private:
    const static float i[10];
    const static float o[10];
    const static float p[10];
    const static float a[10];
    const static float n[10];
    const static float e[10];
    const static float L[10];

    double M, v, r, x, y, z, X, Y, Z, Xq, Yq, Zq;
    double ra, dec;
    double M_e, v_e, r_e, x_e, y_e, z_e, X_e, Y_e, Z_e;
    double Xq_e, Yq_e, Zq_e, ra_e, dec_e;
    double daynumber, dfrac, dno;
    int dd, mm, yy, hh, mu;

};
  
class CelestialDatabase 
{
  public:

    bool FindCelestialGotoObject(int ID, CelestialGotoObject* obj); 
};

#endif // celestialdatabase_h