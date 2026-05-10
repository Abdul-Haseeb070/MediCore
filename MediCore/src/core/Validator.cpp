// Class: Validator  

#include "Validator.h"
#include "../utils/MyString.h"
#include <ctime>

const char* const Validator::SLOTS[8] = {
    "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"
};

bool Validator::parseDate(const char* date, int& d, int& m, int& y) {
    if (!date) {
        return false;
    }

    if (myStrlen(date) != 10) {
        return false;
    }

    if (date[2] != '-' || date[5] != '-') {
        return false;
    }

    char dayBuf[3];
    char monthBuf[3];
    char yearBuf[5];

    dayBuf[0] = date[0];
    dayBuf[1] = date[1];
    dayBuf[2] = '\0';

    monthBuf[0] = date[3];
    monthBuf[1] = date[4];
    monthBuf[2] = '\0';

    yearBuf[0] = date[6];
    yearBuf[1] = date[7];
    yearBuf[2] = date[8];
    yearBuf[3] = date[9];
    yearBuf[4] = '\0';

    if (!myIsDigit(dayBuf[0]) || !myIsDigit(dayBuf[1]) || !myIsDigit(monthBuf[0]) ||
        !myIsDigit(monthBuf[1]) || !myIsDigit(yearBuf[0]) || !myIsDigit(yearBuf[1]) ||
        !myIsDigit(yearBuf[2]) || !myIsDigit(yearBuf[3])) {
        return false;
    }

    d = myAtoi(dayBuf);
    m = myAtoi(monthBuf);
    y = myAtoi(yearBuf);

    if (m < 1 || m > 12) {
        return false;
    }

    if (d < 1 || d > 31) {
        return false;
    }

    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) {
        return false;
    }

    bool leap = ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0));
    if (m == 2) {
        if (leap && d > 29) {
            return false;
        }
        if (!leap && d > 28) {
            return false;
        }
    }

    return true;
}

bool Validator::isValidDate(const char* date) {
    int d = 0;
    int m = 0;
    int y = 0;
    if (!parseDate(date, d, m, y)) {
        return false;
    }

    std::time_t now = std::time(0);
    std::tm* local = std::localtime(&now);
    int currentYear = local->tm_year + 1900;

    return y >= currentYear;
}

bool Validator::isValidTimeSlot(const char* slot) {
    if (!slot) {
        return false;
    }
    for (int i = 0; i < 8; ++i) {
        if (myStrcmp(slot, SLOTS[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool Validator::isValidContact(const char* c) {
    if (!c || myStrlen(c) != 11) {
        return false;
    }
    for (int i = 0; i < 11; ++i) {
        if (!myIsDigit(c[i])) {
            return false;
        }
    }
    return true;
}

bool Validator::isValidPassword(const char* p) {
    return p && myStrlen(p) >= 6;
}

bool Validator::isPositiveFloat(const char* s) {
    if (!s) {
        return false;
    }
    return myAtof(s) > 0.0f;
}

bool Validator::isPositiveInt(const char* s) {
    if (!s) {
        return false;
    }
    int value = myAtoi(s);
    return value > 0;
}

bool Validator::isValidMenuChoice(const char* s, int min, int max) {
    if (!isPositiveInt(s)) {
        return false;
    }
    int value = myAtoi(s);
    return value >= min && value <= max;
}

void Validator::getCurrentDate(char* buf) {
    if (!buf) {
        return;
    }

    std::time_t now = std::time(0);
    std::tm* local = std::localtime(&now);

    char day[3];
    char month[3];
    char year[5];

    int d = local->tm_mday;
    int m = local->tm_mon + 1;
    int y = local->tm_year + 1900;

    day[0] = static_cast<char>('0' + (d / 10));
    day[1] = static_cast<char>('0' + (d % 10));
    day[2] = '\0';

    month[0] = static_cast<char>('0' + (m / 10));
    month[1] = static_cast<char>('0' + (m % 10));
    month[2] = '\0';

    myItoa(y, year);

    buf[0] = '\0';
    myStrcat(buf, day);
    myStrcat(buf, "-");
    myStrcat(buf, month);
    myStrcat(buf, "-");
    myStrcat(buf, year);
}

bool Validator::isDateBefore(const char* a, const char* b) {
    int da = 0;
    int ma = 0;
    int ya = 0;
    int db = 0;
    int mb = 0;
    int yb = 0;

    if (!parseDate(a, da, ma, ya) || !parseDate(b, db, mb, yb)) {
        return false;
    }

    if (ya != yb) {
        return ya < yb;
    }
    if (ma != mb) {
        return ma < mb;
    }
    return da < db;
}

int Validator::daysBetween(const char* dateA, const char* dateB) {
    int da = 0;
    int ma = 0;
    int ya = 0;
    int db = 0;
    int mb = 0;
    int yb = 0;

    if (!parseDate(dateA, da, ma, ya) || !parseDate(dateB, db, mb, yb)) {
        return 0;
    }

    std::tm tma = {};
    tma.tm_mday = da;
    tma.tm_mon = ma - 1;
    tma.tm_year = ya - 1900;
    tma.tm_isdst = -1;

    std::tm tmb = {};
    tmb.tm_mday = db;
    tmb.tm_mon = mb - 1;
    tmb.tm_year = yb - 1900;
    tmb.tm_isdst = -1;

    std::time_t ta = std::mktime(&tma);
    std::time_t tb = std::mktime(&tmb);
    if (ta == static_cast<std::time_t>(-1) || tb == static_cast<std::time_t>(-1)) {
        return 0;
    }

    double diff = std::difftime(tb, ta);
    if (diff < 0) {
        diff = -diff;
    }
    return static_cast<int>(diff / (60.0 * 60.0 * 24.0));
}
