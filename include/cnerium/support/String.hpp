/**
 *
 *  @file String.hpp
 *  @author Softadastra
 *
 *  Copyright 2026, Softadastra.
 *  All rights reserved.
 *  https://github.com/softadastra/cnerium.git
 *
 *  Licensed under the MIT License.
 *
 *  Cnerium
 *
 */

#ifndef CNERIUM_SUPPORT_STRING_HPP
#define CNERIUM_SUPPORT_STRING_HPP

#include <string>
#include <string_view>

namespace cnerium::support
{
  /**
   * @brief Returns true if the string is empty or contains only whitespace.
   *
   * Whitespace is checked using ASCII whitespace characters.
   *
   * @param value String to inspect.
   * @return true if the string is blank.
   */
  [[nodiscard]] bool is_blank(std::string_view value) noexcept;

  /**
   * @brief Returns true if the string contains at least one non-whitespace character.
   *
   * @param value String to inspect.
   * @return true if the string is not blank.
   */
  [[nodiscard]] bool is_not_blank(std::string_view value) noexcept;

  /**
   * @brief Removes leading ASCII whitespace from a string view.
   *
   * The returned view references the original input.
   *
   * @param value String view to trim.
   * @return Trimmed string view.
   */
  [[nodiscard]] std::string_view trim_left_view(std::string_view value) noexcept;

  /**
   * @brief Removes trailing ASCII whitespace from a string view.
   *
   * The returned view references the original input.
   *
   * @param value String view to trim.
   * @return Trimmed string view.
   */
  [[nodiscard]] std::string_view trim_right_view(std::string_view value) noexcept;

  /**
   * @brief Removes leading and trailing ASCII whitespace from a string view.
   *
   * The returned view references the original input.
   *
   * @param value String view to trim.
   * @return Trimmed string view.
   */
  [[nodiscard]] std::string_view trim_view(std::string_view value) noexcept;

  /**
   * @brief Returns a copy of the string without leading ASCII whitespace.
   *
   * @param value String to trim.
   * @return Trimmed string.
   */
  [[nodiscard]] std::string trim_left(std::string_view value);

  /**
   * @brief Returns a copy of the string without trailing ASCII whitespace.
   *
   * @param value String to trim.
   * @return Trimmed string.
   */
  [[nodiscard]] std::string trim_right(std::string_view value);

  /**
   * @brief Returns a copy of the string without leading or trailing ASCII whitespace.
   *
   * @param value String to trim.
   * @return Trimmed string.
   */
  [[nodiscard]] std::string trim(std::string_view value);

  /**
   * @brief Converts ASCII letters to lowercase.
   *
   * Non-ASCII bytes are preserved as-is.
   *
   * @param value String to convert.
   * @return Lowercase string.
   */
  [[nodiscard]] std::string to_lower(std::string_view value);

  /**
   * @brief Converts ASCII letters to uppercase.
   *
   * Non-ASCII bytes are preserved as-is.
   *
   * @param value String to convert.
   * @return Uppercase string.
   */
  [[nodiscard]] std::string to_upper(std::string_view value);

  /**
   * @brief Returns true if two strings are equal ignoring ASCII case.
   *
   * @param left Left string.
   * @param right Right string.
   * @return true if both strings are equal ignoring ASCII case.
   */
  [[nodiscard]] bool equals_ignore_case(
      std::string_view left,
      std::string_view right) noexcept;

  /**
   * @brief Returns true if a string starts with a prefix.
   *
   * @param value String to inspect.
   * @param prefix Prefix to match.
   * @return true if value starts with prefix.
   */
  [[nodiscard]] bool starts_with(
      std::string_view value,
      std::string_view prefix) noexcept;

  /**
   * @brief Returns true if a string starts with a prefix ignoring ASCII case.
   *
   * @param value String to inspect.
   * @param prefix Prefix to match.
   * @return true if value starts with prefix ignoring ASCII case.
   */
  [[nodiscard]] bool starts_with_ignore_case(
      std::string_view value,
      std::string_view prefix) noexcept;

  /**
   * @brief Returns true if a string ends with a suffix.
   *
   * @param value String to inspect.
   * @param suffix Suffix to match.
   * @return true if value ends with suffix.
   */
  [[nodiscard]] bool ends_with(
      std::string_view value,
      std::string_view suffix) noexcept;

  /**
   * @brief Returns true if a string ends with a suffix ignoring ASCII case.
   *
   * @param value String to inspect.
   * @param suffix Suffix to match.
   * @return true if value ends with suffix ignoring ASCII case.
   */
  [[nodiscard]] bool ends_with_ignore_case(
      std::string_view value,
      std::string_view suffix) noexcept;

  /**
   * @brief Returns true if a string contains another string.
   *
   * @param value String to inspect.
   * @param needle Substring to find.
   * @return true if needle exists inside value.
   */
  [[nodiscard]] bool contains(
      std::string_view value,
      std::string_view needle) noexcept;

  /**
   * @brief Returns true if a string contains another string ignoring ASCII case.
   *
   * @param value String to inspect.
   * @param needle Substring to find.
   * @return true if needle exists inside value ignoring ASCII case.
   */
  [[nodiscard]] bool contains_ignore_case(
      std::string_view value,
      std::string_view needle) noexcept;

} // namespace cnerium::support

#endif // CNERIUM_SUPPORT_STRING_HPP
