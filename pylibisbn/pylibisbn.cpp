#include "pybind11/pybind11.h"
#include "libisbn.hpp"

namespace py = pybind11;
using namespace libisbn;

/**
 * More pythonic function. Work on a copy of the string
 */
std::string clean_py(std::string s){
    clean(s);
    return s;
}

PYBIND11_PLUGIN(pylibisbn) {

    py::module m("pylibisbn", "Simple module, written in C++, to work with ISBNs");

    m.def("clean", &clean_py, "Cleans isbn string leaving only valid characters");

    m.def("is_isbn10", &is_isbn10, "Check if string is a valid ISBN10");

    m.def("is_isbn13", &is_isbn13, "Check if string is a valid ISBN13");

    m.def("validate", &validate, "Check if string is a valid ISBN (either ISBN13 or ISBN10)");

    m.def("to_isbn10", &to_isbn10, "Convert ISBN13 string to ISBN10 string");

    m.def("to_isbn13", &to_isbn13, "Convert ISBN10 string to ISBN13 string");

    return m.ptr();


}

