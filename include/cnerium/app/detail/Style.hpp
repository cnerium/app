/**
 * @file Style.hpp
 * @brief cnerium::app::detail — Terminal style helpers
 *
 * @version 0.1.0
 *
 * @details
 * Provides ANSI terminal styling helpers for the Cnerium app layer.
 *
 * Features:
 *   - colors
 *   - emphasis helpers
 *   - section titles
 *   - small console helpers
 *
 * Design goals:
 *   - header-only
 *   - zero dependency
 *   - minimal and reusable
 */

#pragma once

#include <iostream>
#include <string>
#include <string_view>

namespace cnerium::app::detail::style
{
  inline constexpr const char *RESET = "\033[0m";
  inline constexpr const char *BOLD = "\033[1m";
  inline constexpr const char *UNDERLINE = "\033[4m";

  inline constexpr const char *RED = "\033[31m";
  inline constexpr const char *GREEN = "\033[32m";
  inline constexpr const char *YELLOW = "\033[33m";
  inline constexpr const char *CYAN = "\033[36m";
  inline constexpr const char *GRAY = "\033[90m";

  inline constexpr const char *PAD = "  ";

  inline void error(std::string_view msg)
  {
    std::cerr << PAD << RED << "✖ " << msg << RESET << "\n";
  }

  inline void success(std::string_view msg)
  {
    std::cout << PAD << GREEN << "✔ " << msg << RESET << "\n";
  }

  inline void info(std::string_view msg)
  {
    std::cout << PAD << msg << "\n";
  }

  inline void hint(std::string_view msg)
  {
    std::cout << PAD << GRAY << "➜ " << msg << RESET << "\n";
  }

  inline void step(std::string_view msg)
  {
    std::cout << PAD << "• " << msg << "\n";
  }

  inline void section_title(std::ostream &out, std::string_view label)
  {
    out << PAD << BOLD << CYAN << label << RESET << "\n";
  }

  inline void blank(std::ostream &out = std::cout)
  {
    out << "\n";
  }

  inline void dim_note(std::ostream &out, std::string_view label)
  {
    out << PAD << GRAY << label << RESET << "\n";
  }

  inline std::string link(std::string_view url)
  {
    return std::string(GREEN) + std::string(url) + RESET;
  }

} // namespace cnerium::app::detail::style
