#include "Arduino.h"
#include "CelestialDatabase.h"
#include "readfile.h"
#include "SkyMap.h"

const double CelestialGotoObject::i[] = { 0.0, 7.0052, 3.3949, 0.0, 1.8496, 1.3033, 2.4869, 0.7728, 1.7692, 17.1695 };
const double CelestialGotoObject::o[] = { 0.0, 48.493, 76.804, 0.0, 49.668, 100.629, 113.732, 73.989, 131.946, 110.469 };
const double CelestialGotoObject::p[] = { 0.0, 77.669, 131.99, 103.147, 336.322, 14.556, 91.500,  169.602, 6.152, 223.486 };
const double CelestialGotoObject::a[] = { 0.0, 0.387098, 0.723327, 1.0000, 1.523762, 5.20245, 9.52450, 19.1882, 29.9987, 39.2766 };
const double CelestialGotoObject::n[] = { 0.0, 4.09235, 1.60215, 0.985611, 0.523998, 0.083099, 0.033551, 0.011733, 0.006002, 0.004006 };
const double CelestialGotoObject::e[] = { 0.0, 0.205645 , 0.006769, 0.016679, 0.093346, 0.048892, 0.055724, 0.047874, 0.009816, 0.246211 };
const double CelestialGotoObject::L[] = { 0.0, 93.8725, 233.5729, 324.5489, 82.9625, 87.9728, 216.6279, 11.9756, 335.0233, 258.8717 };

// planetary prediction from https://github.com/samhita-ganguly/Real-Time-Planet-Tracking-System-and-Trajectory-Prediction

CelestialDatabase::CelestialDatabase()
{
  InitSDCard(); 
}

bool CelestialDatabase::FindCelestialGotoObject(CelestialGotoObject* obj)
{
  return FindCelestialObjectRecord(obj);
}
     
CelestialGotoObject::CelestialGotoObject() : isValid(false)
{
}

AltAzPosition CelestialGotoObject::getCurrentAltAzPosition(int myYear, int myMonth, int myDay, int hh, double mm)
{
  RaDecPosition radecpsn = getRaDec(myYear, myMonth, myDay, hh, mm);
  Star me(radecpsn.ra, radecpsn.dec); 
  Skymap.DateTime(myYear, myMonth, myDay, (double)hh + mm/60.0);
  Skymap.my_location(mylatitude , mylongitude);
  Skymap.star_ra_dec(me);
  Skymap.Calculate_all();
  AltAzPosition psn;
  psn.alt = Skymap.get_star_Altitude();
  psn.az = Skymap.get_star_Azimuth();
  return psn;
}

RaDecPosition CelestialGotoObject::getRaDec(int myYear, int myMonth, int myDay, int hh, double mm)
{
  RaDecPosition psn;
  if (!isPlanet)
  {
    psn.ra = rightascension;
    psn.dec = declination;
  } 
  else {
    dfrac = (hh + (mm / 60.0)) / 24.0;
    daynumber = dayno(myDay, myMonth, myYear, dfrac);
    earth();
    int j=id-9990;

    M = (n[j] * c_rads * (daynumber)) + (L[j] - p[j]) * c_rads;

    M = frange(M);
    v = fkep(M, e[j]);

    r = a[j] * ((1 - pow(e[j], 2)) / (1 + (e[j] * cos(v))));
    x = r * (cos(o[j] * c_rads) * cos(v + p[j] * c_rads - o[j] * c_rads) - sin(o[j] * c_rads) * sin(v + p[j] * c_rads - o[j] * c_rads) * cos(i[j] * c_rads));
    y = r * (sin(o[j] * c_rads) * cos(v + p[j] * c_rads - o[j] * c_rads) + cos(o[j] * c_rads) * sin(v + p[j] * c_rads - o[j] * c_rads) * cos(i[j] * c_rads));
    z = r * (sin(v + p[j] * c_rads - o[j] * c_rads)) * sin(i[j] * c_rads);
    X = x - x_e;
    Y = y - y_e;
    Z = z;

    double ec = 23.439292 * c_rads;
    Xq = X;
    Yq = (Y * cos(ec)) - (Z * sin(ec));
    Zq = (Y * sin(ec)) + (Z * cos(ec));

    double ra_ = fnatan(Xq, Yq);
    double dec_ = atan(Zq / sqrt(pow(Xq, 2.0) + pow(Yq, 2.0)));

    psn.ra = FNdegmin((ra_ * c_degs));
    psn.dec = FNdegmin(dec_ * c_degs);
  }
  
  return psn; 
}

bool CelestialGotoObject::isAboveHorizon(int myYear, int myMonth, int myDay, int hh, double mm)
{
  RaDecPosition psn = getRaDec(myYear, myMonth, myDay, hh, mm);
  Star me(psn.ra * 15.0, psn.dec); 
  Skymap.DateTime(myYear, myMonth, myDay, hh);
  Skymap.my_location(mylatitude , mylongitude);
  Skymap.star_ra_dec(me);
  Skymap.Calculate_all();
  
  return Skymap.IsVisible();
}

double CelestialGotoObject::FNdegmin(double xx)
{
    double a = (double)(int)(xx);
    double b = xx - a;
    double e = (double)(int)(60 * b);
    //   deal with carry on minutes
    if (e >= 60)
    {
        e = 0;
        a = a + 1;
    }
    return (a + (e / 100));
}

double CelestialGotoObject::dayno(int dx, int mx, int yx, double fx)
{
    dno = (367.0 * yx) - (int)(7.0 * (yx + (int)((mx + 9.0) / 12.0)) / 4.0) + (int)(275.0 * mx / 9.0) + dx - 730531.5 + fx;
    dno -= 4975.5;
    return dno;
}

double CelestialGotoObject::frange(double x)
{
    x = x / (2 * pi);
    x = (2 * pi) * (x - (double)(int)(x));
    if (x < 0)
        x = x + (2 * pi);
    return x;
}

double CelestialGotoObject::fkep(double m, double ecc)
{
    double e = ecc;

    double v = m + (2 * e - 0.25 * pow(e, 3) + 5 / 96 * pow(e, 5)) * sin(m) + (1.25 * pow(e, 2) - 11 / 24 * pow(e, 4)) * sin(2 * m) + (13 / 12 * pow(e, 3) - 43 / 64 * pow(e, 5)) * sin(3 * m) + 103 / 96 * pow(e, 4) * sin(4 * m) + 1097 / 960 * pow(e, 5) * sin(5 * m);

    if (v < 0)
        v = v + (2 * pi);
    return v;
}
double CelestialGotoObject::fnatan(double x, double y)
{
    double a = atan(y / x);
    if (x < 0)
        a = a + pi;
    if ((y < 0) && (x > 0))
        a = a + (2 * pi);
    return a;
}

void CelestialGotoObject::earth()
{
    M_e = ((n[3] * c_rads) * (daynumber)) + (L[3] - p[3]) * c_rads;
    M_e = frange(M_e);
    v_e = fkep(M_e, e[3]);
    r_e = a[3] * ((1 - (pow(e[3], 2))) / (1 + (e[3] * cos(v_e))));

    x_e = r_e * cos(v_e + p[3] * c_rads);
    y_e = r_e * sin(v_e + p[3] * c_rads);
    z_e = 0;
}

