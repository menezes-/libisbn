#include "libisbn.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

//enum class ReturnCode;

constexpr int to_int(char x) noexcept {
    return x - '0';
}

constexpr char to_char(int x) noexcept {
    return x + '0';
}

char _digit13(const std::string &twelve) {

    static int weights[12] = {1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3};

    int digit = 0;

    for (std::size_t i = 0; i < 12; ++i) {
        digit += to_int(twelve[i]) * weights[i];
    }
    digit = 10 - (digit % 10);
    return digit == 10 ? '0' : to_char(digit);

}

char _digit10(const std::string &nine) {
    static int weights[9] = {10, 9, 8, 7, 6, 5, 4, 3, 2};

    int digit = 0;

    for (int i = 0; i < 9; ++i) {
        digit += to_int(nine[i]) * weights[i];
    }

    digit = (11 - (digit % 11)) % 11;

    return digit == 10 ? 'X' : to_char(digit);


}

bool libisbn::is_isbn13(std::string string) {
    static std::string a1{"978"}, a2{"979"};

    clean(string);
    if (string.length() != 13) {
        return false;
    }

    std::string p = string.substr(0, 3);


    if (p != a1 && p != a2) {

        return false;
    }


    return _digit13(string.substr(0, 12)) == string.back();


}


bool libisbn::is_isbn10(std::string string) {

    clean(string);

    if (string.length() != 10) {
        return false;
    }

    int i, s = 0, t = 0;

    for (i = 0; i < 10; ++i) {
        char c = string[i];
        int d = c == 'X' ? 10 : to_int(c);
        t += d;
        s += t;
    }
    return (11 - (s % 11)) % 11 == 0;

}


void libisbn::clean(std::string &s) {

    s.erase(std::remove_if(s.begin(), s.end(), [](char x) {
        return !std::isdigit(x) && !(x == 'x' || x == 'X');
    }), s.end());

    // if isbn10 make last character uppercase
    if (s.back() == 'x' && s.length() == 10) {
        s.back() = 'X';

    }

}

bool libisbn::validate(std::string string) {
    return is_isbn10(string) || is_isbn13(string);
}

std::string libisbn::to_isbn10(std::string isbn) {

    static std::string valid_prefix{"978"};

    clean(isbn);

    if (is_isbn10(isbn)) {
        return isbn;
    }

    if (!is_isbn13(isbn) || isbn.substr(0, 3) != valid_prefix) {

        throw std::invalid_argument(std::string(isbn + std::string{" is not a valid ISBN13"}));

    }

    std::string isbn10{isbn, 3, 9};

    isbn10.append(1, _digit10(isbn10));

    return isbn10;
}

std::string libisbn::to_isbn13(std::string isbn) {

    clean(isbn);

    if (is_isbn13(isbn)) {
        return isbn;
    }

    if (!is_isbn10(isbn)) {
        throw std::invalid_argument(std::string(isbn + std::string{" is not a valid ISBN10"}));
    }

    std::string isbn13{"978" + isbn.substr(0, 9)};

    isbn13.append(1, _digit13(isbn13));

    return isbn13;

}
