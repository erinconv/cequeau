#include "DateChrono.h"
#include "log.h"

//const unsigned int OFFSET_DATENUM = 719529;
const unsigned int OFFSET_DATENUM = 719530;

DateChrono::DateChrono(int year, int month, int day) {
    std::tm date = {};
    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;

    timePoint = std::chrono::system_clock::from_time_t(std::mktime(&date));
}

DateChrono::DateChrono(std::chrono::system_clock::time_point timePoint)
    : timePoint(timePoint) {
}

DateChrono::DateChrono() {
    timePoint = std::chrono::system_clock::now();
}
void DateChrono::display() const {
}

bool DateChrono::operator==(const DateChrono& other) const {
    return timePoint == other.timePoint;
}

bool DateChrono::operator!=(const DateChrono& other) const {
    return !(*this == other);
}

bool DateChrono::operator<(const DateChrono& other) const {
    return timePoint < other.timePoint;
}

bool DateChrono::operator<=(const DateChrono& other) const {
    return timePoint <= other.timePoint;
}

bool DateChrono::operator>(const DateChrono& other) const {
    return timePoint > other.timePoint;
}

bool DateChrono::operator>=(const DateChrono& other) const {
    return timePoint >= other.timePoint;
}

DateChrono& DateChrono::operator+=(int days) {
    timePoint += std::chrono::hours(24 * days);
    return *this;
}

// Implementation of addition and subtraction operators
DateChrono DateChrono::operator+(int days) const {
    auto newTimePoint = timePoint + std::chrono::hours(24 * days);
    return DateChrono{ newTimePoint };
}

DateChrono DateChrono::operator-(int days) const {
    auto newTimePoint = timePoint - std::chrono::hours(24 * days);
    return DateChrono{ newTimePoint };
}

double DateChrono::toMatlabDatenum() const {
    auto duration = timePoint.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count() / 86400.0 + 719529.0;
}

DateChrono DateChrono::fromMatlabDatenum(double matlabDatenum) {

    auto duration = static_cast<long long>((matlabDatenum - 719529) * 86400.0);
    std::chrono::seconds secs(duration);
    std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::from_time_t(0) + secs;

    // Explicitly set the timezone to UTC
    std::time_t tt = std::chrono::system_clock::to_time_t(timePoint);
    std::tm* ptm = std::gmtime(&tt);
    timePoint = std::chrono::system_clock::from_time_t(std::mktime(ptm));

    DateChrono dc(timePoint);

    return dc;
}

std::chrono::system_clock::time_point DateChrono::getTimePoint() {
    return timePoint;
}

// Function to get the day of the year
int DateChrono::getDayOfYear() const {
    // Get the time in a time_point format
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm* timeInfo = std::localtime(&time);

    // Calculate the day of the year
    int dayOfYear = timeInfo->tm_yday + 1;  // tm_yday is 0-based

    return dayOfYear;
}

int DateChrono::getDayNumber() const {
    // Get the duration from epoch
    std::chrono::system_clock::duration duration = timePoint.time_since_epoch();

    // Convert the duration to days
    int days = std::chrono::duration_cast<std::chrono::hours>(duration).count() / 24;

    return days;
}

int DateChrono::getJulianDay() const {
    int year = getYear();
    int month = getMonth();
    int day = getDay();

    if (month <= 2) {
        year -= 1;
        month += 12;
    }


    int A = year / 100;
    int B = 2 - A + (A / 4);

    int julianDay = static_cast<int>(std::floor(365.25 * static_cast<double>(year + 4716)) + std::floor(30.6001 * static_cast<double>(month + 1)) + day + B - 1524.5);

    return julianDay;
}

// Getters for year, month, and day
int DateChrono::getYear() const {
    std::tm timeInfo = std::tm();
    time_t time = std::chrono::system_clock::to_time_t(timePoint);
    localtime_s(&timeInfo, &time);
    return timeInfo.tm_year + 1900;
}

int DateChrono::getMonth() const {
    std::tm timeInfo = std::tm();
    time_t time = std::chrono::system_clock::to_time_t(timePoint);
    localtime_s(&timeInfo, &time);
    return timeInfo.tm_mon + 1;
}

int DateChrono::getDay() const {
    std::tm timeInfo = std::tm();
    time_t time = std::chrono::system_clock::to_time_t(timePoint);
    localtime_s(&timeInfo, &time);
    return timeInfo.tm_mday;
}

std::ostream& operator<<(std::ostream& os, const DateChrono& date) {
    os << date.getYear() << '-' << date.getMonth() << '-' << date.getDay();
    return os;
}
