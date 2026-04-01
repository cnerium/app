/**
 * @file version.hpp
 * @brief cnerium::app — App module version information
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines the public version constants for the Cnerium App module.
 *
 * Responsibilities:
 *   - expose compile-time version numbers
 *   - expose a human-readable version string
 *   - provide lightweight helpers for version inspection
 *
 * Design goals:
 *   - Header-only
 *   - Zero runtime overhead
 *   - Simple public API
 *   - Stable for tooling and diagnostics
 *
 * Notes:
 *   - This file is part of the public API
 *   - Version numbers follow semantic versioning
 *   - The string form is intended for logs, diagnostics, and display
 *
 * Usage:
 * @code
 *   #include <cnerium/app/version.hpp>
 *
 *   static_assert(cnerium::app::version_major == 0);
 *
 *   auto v = cnerium::app::version_string();
 * @endcode
 */

#pragma once

#include <string_view>

namespace cnerium::app
{
  /// @brief Major version number.
  inline constexpr int version_major = 0;

  /// @brief Minor version number.
  inline constexpr int version_minor = 1;

  /// @brief Patch version number.
  inline constexpr int version_patch = 0;

  /// @brief Full semantic version string.
  inline constexpr std::string_view version = "0.1.0";

  /**
   * @brief Returns the full semantic version string.
   *
   * @return std::string_view Version string
   */
  [[nodiscard]] constexpr std::string_view version_string() noexcept
  {
    return version;
  }

} // namespace cnerium::app
