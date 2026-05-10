#ifndef MYSTRING_H
#define MYSTRING_H

#include <cctype>
#include <cstdlib>

inline int myStrlen(const char* s) {
    if (!s) {
        return 0;
    }
    int len = 0;
    while (s[len] != '\0') {
        ++len;
    }
    return len;
}

inline int myStrcmp(const char* a, const char* b) {
    if (!a && !b) {
        return 0;
    }
    if (!a) {
        return -1;
    }
    if (!b) {
        return 1;
    }

    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return static_cast<unsigned char>(a[i]) - static_cast<unsigned char>(b[i]);
        }
        ++i;
    }
    return static_cast<unsigned char>(a[i]) - static_cast<unsigned char>(b[i]);
}

inline void myStrcpy(char* dst, const char* src) {
    if (!dst) {
        return;
    }
    if (!src) {
        dst[0] = '\0';
        return;
    }
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
}

inline void myStrcat(char* dst, const char* src) {
    if (!dst || !src) {
        return;
    }
    int i = myStrlen(dst);
    int j = 0;
    while (src[j] != '\0') {
        dst[i] = src[j];
        ++i;
        ++j;
    }
    dst[i] = '\0';
}

inline bool myIsDigit(char c) {
    return c >= '0' && c <= '9';
}

inline bool myIsAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

inline int myAtoi(const char* s) {
    if (!s) {
        return 0;
    }

    int i = 0;
    while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r') {
        ++i;
    }

    int sign = 1;
    if (s[i] == '-') {
        sign = -1;
        ++i;
    } else if (s[i] == '+') {
        ++i;
    }

    int value = 0;
    while (myIsDigit(s[i])) {
        value = value * 10 + (s[i] - '0');
        ++i;
    }
    return sign * value;
}

inline float myAtof(const char* s) {
    if (!s) {
        return 0.0f;
    }

    int i = 0;
    while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r') {
        ++i;
    }

    float sign = 1.0f;
    if (s[i] == '-') {
        sign = -1.0f;
        ++i;
    } else if (s[i] == '+') {
        ++i;
    }

    float whole = 0.0f;
    while (myIsDigit(s[i])) {
        whole = whole * 10.0f + static_cast<float>(s[i] - '0');
        ++i;
    }

    float frac = 0.0f;
    float base = 1.0f;
    if (s[i] == '.') {
        ++i;
        while (myIsDigit(s[i])) {
            frac = frac * 10.0f + static_cast<float>(s[i] - '0');
            base *= 10.0f;
            ++i;
        }
    }

    return sign * (whole + frac / base);
}

inline void myItoa(int n, char* buf) {
    if (!buf) {
        return;
    }

    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    int temp = n;
    int sign = 0;
    if (temp < 0) {
        sign = 1;
        temp = -temp;
    }

    char rev[32];
    int idx = 0;
    while (temp > 0) {
        rev[idx] = static_cast<char>('0' + (temp % 10));
        ++idx;
        temp /= 10;
    }

    int out = 0;
    if (sign == 1) {
        buf[out] = '-';
        ++out;
    }
    for (int i = idx - 1; i >= 0; --i) {
        buf[out] = rev[i];
        ++out;
    }
    buf[out] = '\0';
}

inline void myFtoa(float f, char* buf, int decimals) {
    if (!buf) {
        return;
    }

    if (decimals < 0) {
        decimals = 0;
    }

    int intPart = static_cast<int>(f);
    float frac = f - static_cast<float>(intPart);
    if (f < 0 && frac != 0.0f) {
        frac = -frac;
    }

    char intBuf[64];
    myItoa(intPart, intBuf);
    myStrcpy(buf, intBuf);

    if (decimals == 0) {
        return;
    }

    myStrcat(buf, ".");
    for (int i = 0; i < decimals; ++i) {
        frac *= 10.0f;
        int digit = static_cast<int>(frac);
        char d[2];
        d[0] = static_cast<char>('0' + (digit < 0 ? -digit : digit));
        d[1] = '\0';
        myStrcat(buf, d);
        frac -= static_cast<float>(digit);
    }
}

inline bool myStrContains(const char* haystack, const char* needle) {
    if (!haystack || !needle) {
        return false;
    }
    const int n = myStrlen(haystack);
    const int m = myStrlen(needle);
    if (m == 0) {
        return true;
    }
    if (m > n) {
        return false;
    }

    for (int i = 0; i <= n - m; ++i) {
        bool ok = true;
        for (int j = 0; j < m; ++j) {
            if (haystack[i + j] != needle[j]) {
                ok = false;
                break;
            }
        }
        if (ok) {
            return true;
        }
    }
    return false;
}

inline void myStrTrim(char* s) {
    if (!s) {
        return;
    }

    int start = 0;
    int end = myStrlen(s) - 1;

    while (s[start] == ' ' || s[start] == '\t' || s[start] == '\n' || s[start] == '\r') {
        ++start;
    }

    while (end >= start && (s[end] == ' ' || s[end] == '\t' || s[end] == '\n' || s[end] == '\r')) {
        --end;
    }

    int out = 0;
    for (int i = start; i <= end; ++i) {
        s[out] = s[i];
        ++out;
    }
    s[out] = '\0';
}

inline void myToLower(char* dst, const char* src) {
    if (!dst) {
        return;
    }
    if (!src) {
        dst[0] = '\0';
        return;
    }

    int i = 0;
    while (src[i] != '\0') {
        char c = src[i];
        if (c >= 'A' && c <= 'Z') {
            dst[i] = static_cast<char>(c - 'A' + 'a');
        } else {
            dst[i] = c;
        }
        ++i;
    }
    dst[i] = '\0';
}

inline void mySubstr(char* dst, const char* src, int start, int len) {
    if (!dst) {
        return;
    }
    if (!src || start < 0 || len <= 0) {
        dst[0] = '\0';
        return;
    }

    int n = myStrlen(src);
    if (start >= n) {
        dst[0] = '\0';
        return;
    }

    int out = 0;
    int i = start;
    while (i < n && out < len) {
        dst[out] = src[i];
        ++out;
        ++i;
    }
    dst[out] = '\0';
}

inline int myTokenize(const char* src, char delim, char** tokens, int maxTokens) {
    if (!src || !tokens || maxTokens <= 0) {
        return 0;
    }

    int count = 0;
    int i = 0;
    int start = 0;

    while (true) {
        if (src[i] == delim || src[i] == '\0') {
            if (count < maxTokens) {
                int len = i - start;
                if (len < 0) {
                    len = 0;
                }
                tokens[count] = new char[len + 1];
                int k = 0;
                for (int j = start; j < i; ++j) {
                    tokens[count][k] = src[j];
                    ++k;
                }
                tokens[count][k] = '\0';
                ++count;
            }
            start = i + 1;
        }

        if (src[i] == '\0' || count >= maxTokens) {
            break;
        }
        ++i;
    }

    return count;
}

#endif
