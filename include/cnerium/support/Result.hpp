/**
 *
 *  @file Result.hpp
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

#ifndef CNERIUM_SUPPORT_RESULT_HPP
#define CNERIUM_SUPPORT_RESULT_HPP

#include <softadastra/core/types/Result.hpp>
#include <cnerium/support/Error.hpp>

namespace cnerium::support
{
  /**
   * @brief Public Cnerium result type.
   *
   * Result<T, E> represents either a successful value of type T or an error of
   * type E.
   *
   * Cnerium reuses the Softadastra core Result implementation instead of
   * duplicating the same error-handling primitive.
   *
   * @tparam T Success value type.
   * @tparam E Error value type.
   */
  template <typename T, typename E = Error>
  using Result = softadastra::core::types::Result<T, E>;

} // namespace cnerium::support

namespace cnerium
{
  /**
   * @brief Public convenience alias for Cnerium results.
   *
   * This alias allows application code to use cnerium::Result<T> while keeping
   * the implementation backed by the shared Softadastra core Result type.
   *
   * @tparam T Success value type.
   * @tparam E Error value type.
   */
  template <typename T, typename E = support::Error>
  using Result = support::Result<T, E>;

} // namespace cnerium

#endif // CNERIUM_SUPPORT_RESULT_HPP
