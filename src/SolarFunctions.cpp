
#include "SolarFunctions.h"

SolarFunctions::SolarFunctions()
{
}

SolarFunctions::SolarFunctions(DateChrono pasTemp, const CarreauEntier &carreauEntier)
{
	pasTemp_ = pasTemp;
	udtLocation_.dLatitude = carreauEntier.latitude();
	udtLocation_.dLongitude = carreauEntier.longitude();
}

SolarFunctions::~SolarFunctions()
{
}

void SolarFunctions::computeZenithAngle(float hours)
{

	udtTime_.iYear = pasTemp_.getYear();
	udtTime_.iDay = pasTemp_.getDay();
	udtTime_.iMonth = pasTemp_.getMonth();
	// Chosen
	udtTime_.dHours = hours;
	udtTime_.dMinutes = 0.0;
	udtTime_.dSeconds = 0.0;
	if (udtTime_.iYear >=2010)
	{
		sunpos_2010(udtTime_, udtLocation_, &udtSunCoordinates_);
	}
	else
	{
		sunpos_1950(udtTime_, udtLocation_, &udtSunCoordinates_);
	}
	
	// udtSunCoordinates_.dZenithAngle
	// return udtSunCoordinates.dZenithAngle;
}

float SolarFunctions::getZenithAngle()
{
	return udtSunCoordinates_.dZenithAngle;
}

float SolarFunctions::deg2rad(float valInDegrees)
{
	return pi * valInDegrees / 180.0f;
}

float SolarFunctions::rad2deg(float valInDegrees)
{
	return 180.0f * valInDegrees / pi;
}

void SolarFunctions::sunpos_2010(cTime udtTime, cLocation udtLocation, cSunCoordinates *udtSunCoordinates)
{
	// // Parse time
	// auto timeConv = std::gmtime(&timeStamp);
	// // Time vectors:
	// float year = timeConv->tm_year + 1900.0;
	// float month = timeConv->tm_mon + 1.0;
	// float day = timeConv->tm_mday;
	// float dayHours = float(timeConv->tm_hour) + float(timeConv->tm_min) / 60.0 + float(timeConv->tm_sec) / 3600.0;
	// https://www.sciencedirect.com/science/article/pii/S0038092X12000400?via%3Dihub
	// https://github.com/david-salac/Fast-SZA-and-SAA-computation/blob/master/CPP_version/SolarAngles.cpp
	// Time transformation:
	float yearVal = floor(365.25 * (udtTime.iYear - 2000.0));
	float monthVal = floor(30.6001 * (udtTime.iMonth + 1.0));
	float yearValPc = floor(0.01 * udtTime.iYear);
	float timeVec = yearVal + monthVal - yearValPc + udtTime.iDay + 0.0416667 * udtTime.dHours - 21958.0;

	// Transform latitude/longitude to radians
	float latRad = deg2rad(udtLocation.dLatitude);
	float lonRad = deg2rad(udtLocation.dLongitude);

	// Reasonable estimates for pressure and temperature
	float pressure = 1.0;	  // unit: atmosphere
	float temperature = 20.0; // unit: degree of celsius

	// Computation
	float dT = 96.4 + 0.567 * (udtTime.iYear - 2061.0);
	float te = timeVec + 1.1574e-5 * dT;
	float wte = 0.0172019715 * te;

	float lambdaDecl = -1.388803 + 1.720279216e-2 * te + 3.3366e-2 * sin(wte - 0.06172) + 3.53e-4 * sin(2.0 * wte - 0.1163);

	float epsilon = 4.089567e-1 - 6.19e-9 * te;

	float sl = sin(lambdaDecl);
	float cl = cos(lambdaDecl);
	float se = sin(epsilon);

	float ce = sqrt(1.0 - se * se);

	float rAsc = atan2(sl * ce, cl);
	// Get rid of negative values in rAsc
	if (rAsc < 0)
	{
		rAsc += 2.0 * pi;
	}

	float hAng = fmod((1.7528311 + 6.300388099 * timeVec + lonRad - rAsc + pi), (2.0 * pi)) - pi;
	// Get rid of negative values in hAng
	if (hAng < 0)
	{
		hAng += 2.0 * pi;
	}

	float sp = sin(latRad);
	float cp = sqrt(1.0 - sp * sp);
	float sd = sl * se;
	float cd = sqrt(1.0 - sd * sd);
	float sH = sin(hAng);
	float cH = cos(hAng);

	float se0 = sp * sd + cp * cd * cH;

	float ep = asin(se0) - 4.26e-5 * sqrt(1.0 - se0 * se0);

	float dE = (0.08422 * pressure) / ((273.0 + temperature) * tan(ep + 0.003138 / (ep + 0.08919)));

	// Outputs
	udtSunCoordinates->dZenithAngle = rad2deg(pi / 2.0f - ep - dE);
	udtSunCoordinates->dAzimuth = rad2deg(pi + atan2(sH, cH * sp - sd * cp / cd));
	// return std::make_tuple(rad2deg(M_PI_2 - ep - dE), rad2deg(M_PI + atan2(sH, cH * sp - sd * cp / cd)));
}

void SolarFunctions::sunpos_1950(cTime udtTime, cLocation udtLocation, cSunCoordinates *udtSunCoordinates)
{
	// Main variables
	float dElapsedJulianDays;
	float dDecimalHours;
	float dEclipticLongitude;
	float dEclipticObliquity;
	float dRightAscension;
	float dDeclination;

	// Auxiliary variables
	float dY;
	float dX;

	// Calculate difference in days between the current Julian Day
	// and JD 2451545.0, which is noon 1 January 2000 Universal Time
	{
		float dJulianDate;
		long int liAux1;
		long int liAux2;
		// Calculate time of the day in UT decimal hours
		dDecimalHours = udtTime.dHours + (udtTime.dMinutes + udtTime.dSeconds / 60.0) / 60.0;
		// Calculate current Julian Day
		liAux1 = (udtTime.iMonth - 14) / 12;
		liAux2 = (1461 * (udtTime.iYear + 4800 + liAux1)) / 4 + (367 * (udtTime.iMonth - 2 - 12 * liAux1)) / 12 - (3 * ((udtTime.iYear + 4900 + liAux1) / 100)) / 4 + udtTime.iDay - 32075;
		dJulianDate = (float)(liAux2)-0.5 + dDecimalHours / 24.0;
		// Calculate difference between current Julian Day and JD 2451545.0
		dElapsedJulianDays = dJulianDate - 2451545.0;
	}

	// Calculate ecliptic coordinates (ecliptic longitude and obliquity of the
	// ecliptic in radians but without limiting the angle to be less than 2*Pi
	// (i.e., the result may be greater than 2*Pi)
	{
		float dMeanLongitude;
		float dMeanAnomaly;
		float dOmega;
		dOmega = 2.1429 - 0.0010394594 * dElapsedJulianDays;
		dMeanLongitude = 4.8950630 + 0.017202791698 * dElapsedJulianDays; // Radians
		dMeanAnomaly = 6.2400600 + 0.0172019699 * dElapsedJulianDays;
		dEclipticLongitude = dMeanLongitude + 0.03341607 * sin(dMeanAnomaly) + 0.00034894 * sin(2 * dMeanAnomaly) - 0.0001134 - 0.0000203 * sin(dOmega);
		dEclipticObliquity = 0.4090928 - 6.2140e-9 * dElapsedJulianDays + 0.0000396 * cos(dOmega);
	}

	// Calculate celestial coordinates ( right ascension and declination ) in radians
	// but without limiting the angle to be less than 2*Pi (i.e., the result may be
	// greater than 2*Pi)
	{
		float dSin_EclipticLongitude;
		dSin_EclipticLongitude = sin(dEclipticLongitude);
		dY = cos(dEclipticObliquity) * dSin_EclipticLongitude;
		dX = cos(dEclipticLongitude);
		dRightAscension = atan2(dY, dX);
		if (dRightAscension < 0.0)
			dRightAscension = dRightAscension + twopi;
		dDeclination = asin(sin(dEclipticObliquity) * dSin_EclipticLongitude);
	}

	// Calculate local coordinates ( azimuth and zenith angle ) in degrees
	{
		float dGreenwichMeanSiderealTime;
		float dLocalMeanSiderealTime;
		float dLatitudeInRadians;
		float dHourAngle;
		float dCos_Latitude;
		float dSin_Latitude;
		float dCos_HourAngle;
		float dParallax;
		dGreenwichMeanSiderealTime = 6.6974243242 +
									 0.0657098283 * dElapsedJulianDays + dDecimalHours;
		dLocalMeanSiderealTime = (dGreenwichMeanSiderealTime * 15 + udtLocation.dLongitude) * rad;
		dHourAngle = dLocalMeanSiderealTime - dRightAscension;
		dLatitudeInRadians = udtLocation.dLatitude * rad;
		dCos_Latitude = cos(dLatitudeInRadians);
		dSin_Latitude = sin(dLatitudeInRadians);
		dCos_HourAngle = cos(dHourAngle);
		udtSunCoordinates->dZenithAngle = (acos(dCos_Latitude * dCos_HourAngle * cos(dDeclination) + sin(dDeclination) * dSin_Latitude));
		dY = -sin(dHourAngle);
		dX = tan(dDeclination) * dCos_Latitude - dSin_Latitude * dCos_HourAngle;
		udtSunCoordinates->dAzimuth = atan2(dY, dX);
		if (udtSunCoordinates->dAzimuth < 0.0)
			udtSunCoordinates->dAzimuth = udtSunCoordinates->dAzimuth + twopi;
		udtSunCoordinates->dAzimuth = udtSunCoordinates->dAzimuth / rad;
		// Parallax Correction
		dParallax = (dEarthMeanRadius / dAstronomicalUnit) * sin(udtSunCoordinates->dZenithAngle);
		udtSunCoordinates->dZenithAngle = (udtSunCoordinates->dZenithAngle + dParallax) / rad;
		// udtSunCoordinates->dZenithAngle = (udtSunCoordinates->dZenithAngle + dParallax);
	}
}
