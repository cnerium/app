/**
 *
 *  @file Version.hpp
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

#ifndef CNERIUM_VERSION_HPP
#define CNERIUM_VERSION_HPP

#include <string>
#include <string_view>

namespace cnerium
{
  /**
   * @brief Cnerium major version.
   */
  inline constexpr int VERSION_MAJOR = 0;

  /**
   * @brief Cnerium minor version.
   */
  inline constexpr int VERSION_MINOR = 1;

  /**
   * @brief Cnerium patch version.
   */
  inline constexpr int VERSION_PATCH = 0;

  /**
   * @brief Cnerium version suffix.
   *
   * Empty means a stable release.
   * Example values: "alpha", "beta", "rc.1".
   */
  inline constexpr std::string_view VERSION_SUFFIX = "alpha";

  /**
   * @brief Cnerium full version string.
   */
  inline constexpr std::string_view VERSION = "0.1.0-alpha";

  /**
   * @brief Cnerium project name.
   */
  inline constexpr std::string_view NAME = "Cnerium";

  /**
   * @brief Cnerium project description.
   */
  inline constexpr std::string_view DESCRIPTION =
      "Reliability-first application framework for Vix and Softadastra.";

  /**
   * @brief Returns the Cnerium full version string.
   *
   * @return Version string.
   */
  [[nodiscard]] constexpr std::string_view version() noexcept
  {
    return VERSION;
  }

  /**
   * @brief Returns the Cnerium project name.
   *
   * @return Project name.
   */
  [[nodiscard]] constexpr std::string_view name() noexcept
  {
    return NAME;
  }

  /**
   * @brief Returns the Cnerium project description.
   *
   * @return Project description.
   */
  [[nodiscard]] constexpr std::string_view description() noexcept
  {
    return DESCRIPTION;
  }

  /**
   * @brief Builds the Cnerium version string as std::string.
   *
   * @return Version string.
   */
  [[nodiscard]] inline std::string version_string()
  {
    return std::string{VERSION};
  }

} // namespace cnerium

#endif // CNERIUM_VERSION_HPP
