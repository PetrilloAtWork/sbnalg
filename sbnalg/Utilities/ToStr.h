/**
 * @file   sbnalg/Utilities/ToStr.h
 * @brief  Small utility to ease creation of strings.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   November 6, 2025
 * @see    sbnalg/Utilities/ToStr.cxx
 */

#ifndef SBNALG_UTILITIES_TOSTR_H
#define SBNALG_UTILITIES_TOSTR_H

// C/C++ standard library
#include <sstream>
#include <string>
#include <utility> // std::forward()

// -----------------------------------------------------------------------------
namespace util { class ToStr; }
/**
 * @brief Stream-to-string conversion helper.
 * 
 * This object should allow statements like:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * using util::ToStr;
 * std::string const s = ToStr{} << "Channel 0x" << std::hex << 0x10;
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * waiting for better (C++20?) days.
 */
class util::ToStr {
  
  /*
   * Each `ToStr` on the first insertion yields a new helper with its own
   * buffer. This helper is the object that does the action (insertions and
   * final conversion to string), but we don't want it to be part of the
   * interface. So we make it very hard to use it:
   *  * it can't be constructed (constructors are private);
   *  * insertion is allowed only on r-values, and every variable we can insert
   *    into is considered a l-value;
   *  * but we can still insert into it casting it to r-value reference
   *    (via `std::move()`). Tough life.
   * 
   * So the implementation of this feature is made quite complicate because of
   * the feature of letting a variable to _start_ the insertion.
   */
  class Helper {
    std::ostringstream fStream; ///< Internal buffer.
    
    Helper() = default;
    Helper(Helper&&) = default; // made private
    Helper& operator= (Helper&&) = default; // made private
    
    friend class ToStr;
    
      public:
    
    /// Insertion operator: supports everything that `std::ostream` typically does.
    template <typename T>
    Helper operator<< (T&& v) &&
      { fStream << std::forward<T>(v); return std::move(*this); }
    
    template <typename T>
    Helper operator<< (T&& v) const&&
      { fStream << std::forward<T>(v); return std::move(*this); }
    
    template <typename T>
    Helper operator<< (T&& v) & = delete;
    
    template <typename T>
    Helper operator<< (T&& v) const& = delete;
    
    /// Returns a copy of the buffer as a string.
    std::string to_string() const& { return fStream.str(); }
    
    /// Yields the buffer as a string (as far as implementation allows).
    std::string to_string() && { return std::move(fStream).str(); }
    
    /// Implicit conversion to a C++ string with a copy of the buffer.
    operator std::string() const& { return to_string(); }
    
    /// Implicit conversion to a C++ string with content moved from the buffer.
    /// @note Move is correctly supported only from C++20 on.
    operator std::string() && { return std::move(fStream).str(); }
    
  }; // Helper
  
    public:
  
  
  /// Insertion operator: supports everything that `std::ostream` typically does.
  template <typename T>
  auto operator<< (T&& v) const { return Helper{} << std::forward<T>(v); }
  
  /// Returns a copy of the buffer as a string.
  std::string to_string() const& { return {}; }
  
  /// Yields the buffer as a string (as far as implementation allows).
  std::string to_string() && { return {}; }
  
  /// Implicit conversion to a C++ string with a copy of the buffer.
  operator std::string() const& { return {}; }
  
  /// Implicit conversion to a C++ string with content moved from the buffer.
  /// @note Move is correctly supported only from C++20 on.
  operator std::string() && { return {}; }
  
}; // util::ToStr


namespace util {
  
  /**
   * @brief Global variable to be used as starting point for `util::ToStr`.
   * 
   * This object should allow statements like:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * using util::toStr;
   * std::string const s = toStr << "Channel 0x" << std::hex << 0x10;
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * waiting for better (C++20?) days.
   * 
   */
  extern ToStr const toStr;
  
} // namespace util


// -----------------------------------------------------------------------------

#endif // SBNALG_UTILITIES_TOSTR_H
