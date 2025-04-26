// This file is available in electronic form at http://www.psa.es/sdg/sunpos.htm
#include "stdafx.h"
#include "CarreauEntier.h"

// Declaration of some constants
#define pi 3.14159265358979323846f
#define twopi (2.0f * pi)
#define rad (pi / 180.0f)
#define dEarthMeanRadius 6371.01f      // In km
#define dAstronomicalUnit 149597890.0f // In km

class SolarFunctions
{
public:
    SolarFunctions();
    SolarFunctions(DateChrono pasTemp, const CarreauEntier &carreauEntier);
    ~SolarFunctions();
    void computeZenithAngle(float hours);
    float getZenithAngle();

private:
    DateChrono pasTemp_;
    float latitute_;
    float longitude_;
    float zenithAngle_;

    struct cTime
    {
        int iYear;
        int iMonth;
        int iDay;
        float dHours;
        float dMinutes;
        float dSeconds;
    } udtTime_;

    struct cLocation
    {
        float dLongitude;
        float dLatitude;
    } udtLocation_;

    struct cSunCoordinates
    {
        float dZenithAngle;
        float dAzimuth;
    } udtSunCoordinates_;

    void sunpos_1950(cTime udtTime, cLocation udtLocation, cSunCoordinates *udtSunCoordinates);
    void sunpos_2010(cTime udtTime, cLocation udtLocation, cSunCoordinates *udtSunCoordinates);
    float deg2rad(float valInDegrees);
    float rad2deg(float valInDegrees);
};
