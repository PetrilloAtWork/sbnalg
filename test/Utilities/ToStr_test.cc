/**
 * @file   ToStr_test.cc
 * @brief  Unit test for `util::ToStr` class.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   November 6, 2025
 * @see    sbnalg/Utilities/ToStr.h
 */


// Boost libraries
#define BOOST_TEST_MODULE ToStr
#include <boost/test/unit_test.hpp>

// ICARUS libraries
#include "sbnalg/Utilities/ToStr.h"

// // C/C++ standard libraries
// #include <iostream>
// #include <utility> // std::pair<>
// #include <array>
// #include <type_traits> // std::is_same_v, std::remove_reference_t



// -----------------------------------------------------------------------------
void TestToStrImplicitConversion() {
  
  using util::ToStr;
  
  auto printStr = [](std::string s){ return s; };
  auto printCRStr = [](std::string const& s){ return s; };
  auto printRVStr = [](std::string&& s){ return s; };
  
  BOOST_TEST(printStr  (ToStr{} << "test #" << 1) == "test #1");
  BOOST_TEST(printCRStr(ToStr{} << "test #" << 2) == "test #2");
  BOOST_TEST(printRVStr(ToStr{} << "test #" << 3) == "test #3");
  
  // this is fundamentally different from the above,
  // where Helper objects were pulled in
  BOOST_TEST(printStr  (ToStr{}) == "");
  BOOST_TEST(printCRStr(ToStr{}) == "");
  BOOST_TEST(printRVStr(ToStr{}) == "");
  
  // abusing the temporary:
  auto temp = ToStr{} << "test";
  // attempt to insert into temp would give a compilation error
  BOOST_TEST(printStr(temp) == "test");
  
  
} // TestToStrImplicitConversion()


// -----------------------------------------------------------------------------
void TesttoStrImplicitConversion() {
  
  using util::toStr;
  
  auto printStr = [](std::string s){ return s; };
  auto printCRStr = [](std::string const& s){ return s; };
  auto printRVStr = [](std::string&& s){ return s; };
  
  BOOST_TEST(printStr  (toStr << "test #" << 1) == "test #1");
  BOOST_TEST(printCRStr(toStr << "test #" << 2) == "test #2");
  BOOST_TEST(printRVStr(toStr << "test #" << 3) == "test #3");
  
  // this is fundamentally different from the above,
  // where Helper objects were pulled in
  BOOST_TEST(printStr  (toStr) == "");
  BOOST_TEST(printCRStr(toStr) == "");
  BOOST_TEST(printRVStr(toStr) == "");
  
  // abusing the temporary:
  auto temp = toStr << "test";
  // attempt to insert into temp would give a compilation error
  BOOST_TEST(printStr(temp) == "test");
  
} // TesttoStrImplicitConversion()


// -----------------------------------------------------------------------------
void TesttoStrDocumentation() {
  /*
   * The promise:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * using util::toStr;
   * std::string const s = toStr << "Channel 0x" << std::hex << 0x10;
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   */
  
  using util::toStr;
  std::string const s = toStr << "Channel 0x" << std::hex << 0x10;
  
  BOOST_CHECK(s == "Channel 0x10");
  
} // TesttoStrDocumentation()


// -----------------------------------------------------------------------------
void TestToStrDocumentation() {
  /*
   * The promise:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * using util::ToStr;
   * std::string const s = ToStr{} << "Channel 0x" << std::hex << 0x10;
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   */
  
  using util::ToStr;
  std::string const s = ToStr{} << "Channel 0x" << std::hex << 0x10;
  
  BOOST_CHECK(s == "Channel 0x10");
  
} // TestToStrDocumentation()


//------------------------------------------------------------------------------
//---  The tests
//---
BOOST_AUTO_TEST_CASE( BasicTestCase ) {
  
  TestToStrImplicitConversion();
  TesttoStrImplicitConversion();
  
} // BOOST_AUTO_TEST_CASE( BasicTestCase )


BOOST_AUTO_TEST_CASE( DocumentationTestCase ) {
  
  TestToStrDocumentation();
  TesttoStrDocumentation();
  
} // BOOST_AUTO_TEST_CASE( DocumentationTestCase )


//------------------------------------------------------------------------------
