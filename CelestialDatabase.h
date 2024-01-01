#ifndef celestialdatabase_h
#define celestialdatabase_h

// Celestial Database
#include "Arduino.h"

struct SkyPosition
{
  // stupid naming, but useful to reuse structure
  double alt_ra;
  double az_dec;  
};

class CelestialGotoObject {
  public:
    CelestialGotoObject();
    SkyPosition getCurrentAltAzPosition(int hour, int minute);
    SkyPosition getRaDec(int hour, int minute);
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

    int RA_hour;
    int RA_minute;
    int decSign;
    int DEC_hour;
    int DEC_minute;

    bool isPlanet;
private:
    const static double i[10];
    const static double o[10];
    const static double p[10];
    const static double a[10];
    const static double n[10];
    const static double e[10];
    const static double L[10];

    double M, v, r, x, y, z, X, Y, Z, Xq, Yq, Zq;
    double ra, dec;
    double M_e, v_e, r_e, x_e, y_e, z_e, X_e, Y_e, Z_e;
    double Xq_e, Yq_e, Zq_e, ra_e, dec_e;
    double daynumber, dfrac, dno;
    int dd, mm, yy, hh, mu;
    double pi;
    double c_rads;
    double c_degs;

    double mylatitude = 52.6027972;
    double mylongitude = -3.0954659;
    double myYear = 2024;
    double myMonth = 1;
    double myDay = 1;
};
  
class CelestialDatabase 
{
  public:

    bool FindCelestialGotoObject(String ID, CelestialGotoObject* obj); 
};

#endif // celestialdatabase_h