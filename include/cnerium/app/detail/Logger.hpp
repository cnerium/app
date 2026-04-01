/**
 * @file Logger.hpp
 * @brief cnerium::app::detail — Application logger
 *
 * @version 0.1.0
 *
 * @details
 * Provides a small, header-only logging helper for the Cnerium app layer.
 *
 * Features:
 *   - simple log levels
 *   - colored output
 *   - consistent startup logs
 *   - zero dependency
 *
 * Design goals:
 *   - header-only
 *   - minimal
 *   - fast
 *   - developer-friendly
 */

#pragma once

#include <cnerium/app/detail/Style.hpp>
#include <cnerium/app/detail/Ui.hpp>

#include <iostream>
#include <ostream>
#include <string>
#include <string_view>

namespace cnerium::app::detail
{
  enum class LogLevel
  {
    info,
    warn,
    error,
    success
  };

  inline const char *log_color(LogLevel level)
  {
    using namespace cnerium::app::detail::style;

    switch (level)
    {
    case LogLevel::info:
      return CYAN;
    case LogLevel::warn:
      return YELLOW;
    case LogLevel::error:
      return RED;
    case LogLevel::success:
      return GREEN;
    }

    return RESET;
  }

  inline const char *log_label(LogLevel level)
  {
    switch (level)
    {
    case LogLevel::info:
      return "INFO";
    case LogLevel::warn:
      return "WARN";
    case LogLevel::error:
      return "ERROR";
    case LogLevel::success:
      return "OK";
    }

    return "";
  }

  inline std::ostream &log_stream(LogLevel level)
  {
    return (level == LogLevel::error) ? std::cerr : std::cout;
  }

  inline void log(LogLevel level, std::string_view message)
  {
    using namespace cnerium::app::detail::style;

    auto &os = log_stream(level);

    os << "  "
       << log_color(level) << BOLD
       << "[" << log_label(level) << "]"
       << RESET
       << " "
       << message
       << "\n";
  }

  inline void info(std::string_view msg)
  {
    log(LogLevel::info, msg);
  }

  inline void warn(std::string_view msg)
  {
    log(LogLevel::warn, msg);
  }

  inline void error(std::string_view msg)
  {
    log(LogLevel::error, msg);
  }

  inline void success(std::string_view msg)
  {
    log(LogLevel::success, msg);
  }

  inline void startup_banner(
      std::string_view name,
      std::string_view version,
      std::string_view host,
      std::string_view port)
  {
    using namespace cnerium::app::detail::style;

    section(std::cout, name);
    kv(std::cout, "version", version);
    kv(std::cout, "url", (std::string("http://") + std::string(host) + ":" + std::string(port)));
    std::cout << GRAY << "• " << RESET << "Press Ctrl+C to stop\n";
  }

} // namespace cnerium::app::detail
