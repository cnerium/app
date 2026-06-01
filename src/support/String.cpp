/**
 *
 *  @file String.cpp
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

#include <cnerium/support/String.hpp>

#include <algorithm>
#include <cctype>

namespace cnerium::support
{
  namespace
  {
    [[nodiscard]] bool is_ascii_space(char value) noexcept
    {
      return std::isspace(static_cast<unsigned char>(value)) != 0;
    }

    [[nodiscard]] char to_lower_ascii(char value) noexcept
    {
      return static_cast<char>(
          std::tolower(static_cast<unsigned char>(value)));
    }

    [[nodiscard]] char to_upper_ascii(char value) noexcept
    {
      return static_cast<char>(
          std::toupper(static_cast<unsigned char>(value)));
    }
  } // namespace

  bool is_blank(std::string_view value) noexcept
  {
    return std::all_of(
        value.begin(),
        value.end(),
        [](char character)
        {
          return is_ascii_space(character);
        });
  }

  bool is_not_blank(std::string_view value) noexcept
  {
    return !is_blank(value);
  }

  std::string_view trim_left_view(std::string_view value) noexcept
  {
    while (!value.empty() && is_ascii_space(value.front()))
    {
      value.remove_prefix(1);
    }

    return value;
  }

  std::string_view trim_right_view(std::string_view value) noexcept
  {
    while (!value.empty() && is_ascii_space(value.back()))
    {
      value.remove_suffix(1);
    }

    return value;
  }

  std::string_view trim_view(std::string_view value) noexcept
  {
    return trim_right_view(trim_left_view(value));
  }

  std::string trim_left(std::string_view value)
  {
    const auto trimmed = trim_left_view(value);
    return std::string{trimmed};
  }

  std::string trim_right(std::string_view value)
  {
    const auto trimmed = trim_right_view(value);
    return std::string{trimmed};
  }

  std::string trim(std::string_view value)
  {
    const auto trimmed = trim_view(value);
    return std::string{trimmed};
  }

  std::string to_lower(std::string_view value)
  {
    std::string result{value};

    std::transform(
        result.begin(),
        result.end(),
        result.begin(),
        [](char character)
        {
          return to_lower_ascii(character);
        });

    return result;
  }

  std::string to_upper(std::string_view value)
  {
    std::string result{value};

    std::transform(
        result.begin(),
        result.end(),
        result.begin(),
        [](char character)
        {
          return to_upper_ascii(character);
        });

    return result;
  }

  bool equals_ignore_case(
      std::string_view left,
      std::string_view right) noexcept
  {
    if (left.size() != right.size())
    {
      return false;
    }

    for (std::size_t index = 0; index < left.size(); ++index)
    {
      if (to_lower_ascii(left[index]) != to_lower_ascii(right[index]))
      {
        return false;
      }
    }

    return true;
  }

  bool starts_with(
      std::string_view value,
      std::string_view prefix) noexcept
  {
    if (prefix.size() > value.size())
    {
      return false;
    }

    return value.substr(0, prefix.size()) == prefix;
  }

  bool starts_with_ignore_case(
      std::string_view value,
      std::string_view prefix) noexcept
  {
    if (prefix.size() > value.size())
    {
      return false;
    }

    return equals_ignore_case(
        value.substr(0, prefix.size()),
        prefix);
  }

  bool ends_with(
      std::string_view value,
      std::string_view suffix) noexcept
  {
    if (suffix.size() > value.size())
    {
      return false;
    }

    return value.substr(value.size() - suffix.size()) == suffix;
  }

  bool ends_with_ignore_case(
      std::string_view value,
      std::string_view suffix) noexcept
  {
    if (suffix.size() > value.size())
    {
      return false;
    }

    return equals_ignore_case(
        value.substr(value.size() - suffix.size()),
        suffix);
  }

  bool contains(
      std::string_view value,
      std::string_view needle) noexcept
  {
    return value.find(needle) != std::string_view::npos;
  }

  bool contains_ignore_case(
      std::string_view value,
      std::string_view needle) noexcept
  {
    if (needle.empty())
    {
      return true;
    }

    if (needle.size() > value.size())
    {
      return false;
    }

    for (std::size_t index = 0; index + needle.size() <= value.size(); ++index)
    {
      if (equals_ignore_case(
              value.substr(index, needle.size()),
              needle))
      {
        return true;
      }
    }

    return false;
  }

} // namespace cnerium::support
