#define CATCH_CONFIG_MAIN

#include <string>
#include <array>
#include "libisbn.hpp"
#include "Catch/single_include/catch.hpp"

using namespace libisbn;

TEST_CASE("clean") {
    REQUIRE(clean("978-1-891830-75-4") == "9781891830754");
    REQUIRE(clean("9781891830754") == "9781891830754");
    REQUIRE(clean("isbn 979-10-90636-07-1") == "9791090636071");
    REQUIRE(clean("073627099x") == "073627099X");
    REQUIRE(clean("ISBN: 07-362-7099x   ") == "073627099X");
    REQUIRE(clean("0-7362-7004-3") == "0736270043");


}

TEST_CASE("ISBN13 convert", "[convert]") {
    std::array<std::string, 4> isbns13 = {
            "978082-6497529",
            "978-1-891830-75-4",
            "978-1-60309-069-8",
            "9780071148160"
    };

    std::array<std::string, 4> isbns10 = {
            "0826497527",
            "1891830759",
            "160309069X",
            "0071148167"


    };

    REQUIRE(isbns13.size() == isbns10.size());

    for (std::size_t i = 0; i < isbns13.size(); ++i) {
        CHECK(to_isbn10(isbns13[i]) == isbns10[i]);
    }

    CHECK_THROWS(to_isbn10("979-10-90636-07-1"));

}


TEST_CASE("ISBN13 check", "[validate]") {
    std::array<std::string, 5> invalids = {
            "9780826497520",
            "9700000000000",
            "9000000000000",
            "9710000000000",
            "979-1-59940-599-X"
    };

    std::array<std::string, 6> valids = {
            "978082-6497529",
            "9791090636071",
            "isbn 979-10-90636-07-1",
            "978-1-891830-75-4",
            "978-1-60309-069-8",
            "9780071148160"
    };


    for (auto i : valids) {

        CHECK(is_isbn13(i));
        CHECK(validate(i));

    }


    for (auto i : invalids) {

        CHECK_FALSE(is_isbn13(i));
        CHECK_FALSE(validate(i));


    }


}

TEST_CASE("ISBN10 check", "[check]") {

    std::array<std::string, 6> valids = {
            "073627099x",
            "0736270078",
            "07362-70-043",
            "0736271414",
            "ISBN: 0736270140",
            "4413008480"

    };

    std::array<std::string, 3> invalids = {
            "888183718",
            "9887401392",
            "0-8436-1072-x"

    };

    for (auto i : valids) {

        CHECK(is_isbn10(i));
        CHECK(validate(i));


    }

    for (auto i : invalids) {

        CHECK_FALSE(is_isbn10(i));
        CHECK_FALSE(validate(i));

    }

}

TEST_CASE("ISBN10 convert", "[convert]") {

    std::array<std::string, 5> isbns13 = {
            "9780826497529",
            "9781891830754",
            "9781603090698",
            "9780071148160",
            "9784413008488"
    };

    std::array<std::string, 5> isbns10 = {
            "0826497527",
            "18918-30759",
            "160309069-X",
            "0071148167",
            "ISBN: 4413008480"


    };

    REQUIRE(isbns13.size() == isbns10.size());

    for (std::size_t i = 0; i < isbns10.size(); ++i) {
        CHECK(to_isbn13(isbns10[i]) == isbns13[i]);
    }


    CHECK_THROWS(to_isbn13("0-8436-1072-x"));


}





