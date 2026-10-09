#ifndef __UTILS__H__
#define __UTILS__H__

#include <cctype>

inline bool isAlpha(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

inline bool isAlnum(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c =='_';
}

#endif // __UTILS__H__
