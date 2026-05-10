#ifndef VALIDATOR_H
#define VALIDATOR_H

class Validator {
public:
    static const char* const SLOTS[8];

    /// @brief Validate DD-MM-YYYY date and year threshold.
    /// @param date Date string.
    /// @return True when valid and year >= current year.
    static bool isValidDate(const char* date);

    /// @brief Validate against fixed slot list.
    /// @param slot Time slot string.
    /// @return True when slot exists.
    static bool isValidTimeSlot(const char* slot);

    /// @brief Validate 11-digit contact.
    /// @param c Contact string.
    /// @return True when numeric and size is 11.
    static bool isValidContact(const char* c);

    /// @brief Validate password length.
    /// @param p Password string.
    /// @return True when length >= 6.
    static bool isValidPassword(const char* p);

    /// @brief Validate positive float text.
    /// @param s Number string.
    /// @return True when parsed value > 0.
    static bool isPositiveFloat(const char* s);

    /// @brief Validate positive integer text.
    /// @param s Integer string.
    /// @return True when parsed value > 0.
    static bool isPositiveInt(const char* s);

    /// @brief Validate menu choice bounds.
    /// @param s Input text.
    /// @param min Lower bound.
    /// @param max Upper bound.
    /// @return True when parsed integer in range.
    static bool isValidMenuChoice(const char* s, int min, int max);

    /// @brief Get current date in DD-MM-YYYY.
    /// @param buf Destination buffer.
    /// @return Nothing.
    static void getCurrentDate(char* buf);

    /// @brief Compare chronological order.
    /// @param a Left date.
    /// @param b Right date.
    /// @return True when a < b.
    static bool isDateBefore(const char* a, const char* b);

    /// @brief Compute absolute day difference.
    /// @param dateA First date.
    /// @param dateB Second date.
    /// @return Number of days.
    static int daysBetween(const char* dateA, const char* dateB);

private:
    /// @brief Parse DD-MM-YYYY date text.
    /// @param date Input date.
    /// @param d Parsed day.
    /// @param m Parsed month.
    /// @param y Parsed year.
    /// @return True on parse success.
    static bool parseDate(const char* date, int& d, int& m, int& y);
};

#endif
