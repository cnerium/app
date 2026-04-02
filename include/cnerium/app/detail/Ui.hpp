/**
 * @file Ui.hpp
 * @brief cnerium::app::detail — Console UI helpers
 *
 * @version 0.1.0
 *
 * @details
 * Provides small reusable console UI helpers for the Cnerium app layer.
 *
 * Features:
 *   - key/value lines
 *   - section titles
 *   - info / success / warning / error lines
 *   - emphasis helpers
 *
 * Design goals:
 *   - header-only
 *   - minimal
 *   - readable console output
 */

#pragma once

#include <cnerium/app/detail/Style.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace cnerium::app::detail
{
  using namespace cnerium::app::detail::style;

  inline std::string quote(std::string_view s)
  {
    return std::string("\"") + std::string(s) + "\"";
  }

  inline void kv(std::ostream &os,
                 std::string_view key,
                 std::string_view value,
                 int pad = 10)
  {
    std::ostringstream line;
    line << key;

    std::string k = line.str();
    if (static_cast<int>(k.size()) < pad)
    {
      k.append(static_cast<std::size_t>(pad - static_cast<int>(k.size())), ' ');
    }

    os << GRAY << "• " << RESET
       << GRAY << k << RESET
       << GRAY << ": " << RESET
       << YELLOW << BOLD << value << RESET << "\n";
  }

  inline void section(std::ostream &os, std::string_view title)
  {
    section_title(os, title);
  }

  inline void ok_line(std::ostream &os, std::string_view msg)
  {
    os << "  " << GREEN << "✔" << RESET << " " << msg << "\n";
  }

  inline void warn_line(std::ostream &os, std::string_view msg)
  {
    os << "  " << YELLOW << "!" << RESET << " " << msg << "\n";
  }

  inline void tip_line(std::ostream &os, std::string_view msg)
  {
    os << "\n"
       << BOLD << CYAN << "TIP:" << RESET << " " << msg << "\n\n";
  }

  inline void err_line(std::ostream &os, std::string_view msg)
  {
    os << "  " << RED << "✖" << RESET << " " << msg << "\n";
  }

  inline std::string dim(std::string_view s)
  {
    return std::string(GRAY) + std::string(s) + RESET;
  }

  inline std::string strong(std::string_view s)
  {
    return std::string(BOLD) + std::string(s) + RESET;
  }

  inline void info_line(std::ostream &os, std::string_view msg)
  {
    os << "  " << CYAN << "•" << RESET << " " << msg << "\n";
  }

  inline void info(std::ostream &os, std::string_view msg)
  {
    os << "  " << GRAY << "•" << RESET << " " << msg << "\n";
  }

  inline std::string faint_sep()
  {
    return std::string(GRAY) + "────────────────────────────────────────" + RESET;
  }

  inline void one_line_spacer(std::ostream &os)
  {
    os << "\n";
  }

} // namespace cnerium::app::detail
