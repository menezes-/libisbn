#include "libisbn.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <array>



constexpr int to_int(char x) noexcept {
    return x - '0';
}

constexpr char to_char(int x) noexcept {
    return x + '0';
}

constexpr std::array<int, 12> ISBN13_W {{1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3}};

constexpr std::array<int, 9> ISBN10_W {{10, 9, 8, 7, 6, 5, 4, 3, 2}};

constexpr auto VALID_ISBN13_PREFIX = "978"; // only ISBN13 with this prefix can be converted to ISBN10

char _digit13(const std::string &twelve) {
    int digit = 0;

    for (std::size_t i = 0; i < 12; ++i) {
        digit += to_int(twelve[i]) * ISBN13_W[i];
    }
    digit = 10 - (digit % 10);
    return digit == 10 ? '0' : to_char(digit);

}

char _digit10(const std::string &nine) {
    int digit = 0;

    for (int i = 0; i < 9; ++i) {
        digit += to_int(nine[i]) * ISBN10_W[i];
    }

    digit = (11 - (digit % 11)) % 11;

    return digit == 10 ? 'X' : to_char(digit);


}

bool libisbn::is_isbn13(std::string string) {
    static std::string a1{VALID_ISBN13_PREFIX}, a2{"979"};

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

    return _digit10(string.substr(0, 9)) == string.back();

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

    static std::string valid_prefix{VALID_ISBN13_PREFIX};

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

    std::string isbn13{VALID_ISBN13_PREFIX + isbn.substr(0, 9)};

    isbn13.append(1, _digit13(isbn13));

    return isbn13;

}
