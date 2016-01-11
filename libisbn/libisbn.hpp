#ifndef LIBISBN_HPP
#define LIBISBN_HPP

#include <string>

namespace libisbn {
    /**
     * remove everything that isn't a digit or "x" (only check for "x" if string len is 10)
     */
    std::string clean(std::string);

    bool is_isbn13(std::string string);

    bool is_isbn10(std::string string);

    /**
     * verify if it is either a isbn10 or 13
     */
    bool verify(std::string string);

    /**
     * convert isbn13 to isbn10
     */
    std::string to_isbn10(std::string string);

    /**
     * convert isbn10 to isbn13
     */
    std::string to_isbn13(std::string string);



}


#endif //LIBISBN_HPP
