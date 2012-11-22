#pragma once
#include <UnitTest++/UnitTest++.h>

#define QUOTIFY(expr) "\"" #expr "\""

#define ASSERT_CHECK(expr) \
    if (! ( expr ) ) \
{ \
    throw UnitTest::AssertException( \
    "Assertion of expression " QUOTIFY(expr) " failed", \
    __FILE__, __LINE__ \
    );\
}

#define ASSERT_EQUAL(expected,actual) \
    if ( (expected) != (actual) ) \
{ \
    throw UnitTest::AssertException( \
    "Assertion of " QUOTIFY(actual) " == " QUOTIFY(expected) " failed", \
    __FILE__, __LINE__ \
    ); \
}
