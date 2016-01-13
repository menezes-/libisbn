#include "libisbn.hpp"
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <stdexcept>

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>

#endif

//enum class ReturnCode;

constexpr int to_int(char x) noexcept {
    return x - '0';
}

constexpr char to_char(int x) noexcept {
    return x + '0';
}


// taken from http://stackoverflow.com/a/10501533/482238
inline __m128i muly(const __m128i &a, const __m128i &b) {
#ifdef __SSE4_1__  // modern CPU - use SSE 4.1
    return _mm_mullo_epi32(a, b);
#else               // old CPU - use SSE 2
    __m128i tmp1 = _mm_mul_epu32(a,b); /* mul 2,0*/
    __m128i tmp2 = _mm_mul_epu32( _mm_srli_si128(a,4), _mm_srli_si128(b,4)); /* mul 3,1 */
    return _mm_unpacklo_epi32(_mm_shuffle_epi32(tmp1, _MM_SHUFFLE (0,0,2,0)), _mm_shuffle_epi32(tmp2, _MM_SHUFFLE (0,0,2,0))); /* shuffle results to [63..0] and pack */
#endif
}


inline std::int32_t dot_product(const std::int32_t* arr, const std::int32_t* coeff){

    __m128i temp_sum = _mm_setzero_si128();

    for (size_t i = 0; i < 12; i += 4) {
        __m128i v0 = _mm_load_si128((__m128i *) (arr + i));
        __m128i v1 = _mm_load_si128((__m128i *) (coeff + i));
        __m128i temp_products = muly(v0, v1);
        temp_sum = _mm_add_epi32(temp_sum, temp_products);
    }

    __m128i r1 = _mm_hadd_epi32(temp_sum, temp_sum);
    int32_t sum = _mm_cvtsi128_si32(_mm_hadd_epi32(r1, r1));
    return sum;

}



char _digit13_sse(const std::string &twelve){
    static const std::int32_t coeffs[12]  __attribute__ ((aligned (16))) = {1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3};
    std::int32_t digits[12] __attribute__ ((aligned (16))) = {0};
    for (std::size_t i = 0; i < 12; ++i) {
        digits[i] = to_int(twelve[i]);

    }

    std::int32_t digit = dot_product(digits, coeffs);
    digit = 10 - (digit % 10);
    return digit == 10 ? '0' : to_char(digit);


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


    return _digit13_sse(string.substr(0, 12)) == string.back();


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
