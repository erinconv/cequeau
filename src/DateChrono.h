#ifndef DATE_CHRONO_H
#define DATE_CHRONO_H

#include <iostream>
#include <chrono>
#include <cmath>
#include <ctime>

class DateChrono {
private:
    std::chrono::system_clock::time_point timePoint;

public:
    // Constructors
    DateChrono(int year, int month, int day);
    DateChrono(std::chrono::system_clock::time_point timePoint);
    DateChrono();

    // Display the date
    void display() const;

    // Comparison operators
    bool operator==(const DateChrono& other) const;
    bool operator!=(const DateChrono& other) const;
    bool operator<(const DateChrono& other) const;
    bool operator<=(const DateChrono& other) const;
    bool operator>(const DateChrono& other) const;
    bool operator>=(const DateChrono& other) const;

    DateChrono operator+(int days) const;
    DateChrono operator-(int days) const;
    DateChrono& operator+=(int days);

    

    // Conversion to MATLAB datenum
    double toMatlabDatenum() const;

    // Conversion from MATLAB datenum
    static DateChrono fromMatlabDatenum(double matlabDatenum);

    std::chrono::system_clock::time_point getTimePoint();
    int getYear() const;
    int getMonth() const;
    int getDay() const;

    int getDayNumber() const;
    int getDayOfYear() const;
    int getJulianDay() const;

    friend std::ostream& operator<<(std::ostream& os, const DateChrono& date);

};

#endif // DATE_CHRONO_H
