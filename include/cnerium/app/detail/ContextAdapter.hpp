/**
 * @file ContextAdapter.hpp
 * @brief cnerium::app::detail — Adapter: server::Context → AppContext
 *
 * @version 0.1.0
 *
 * @details
 * Provides a lightweight adapter to wrap a cnerium::server::Context
 * into a cnerium::app::AppContext.
 *
 * Responsibilities:
 *   - construct AppContext from server::Context
 *   - keep zero-cost abstraction
 *   - isolate adaptation logic from App layer
 *
 * Design goals:
 *   - Header-only
 *   - No allocation
 *   - Minimal overhead
 *   - Internal use only (detail namespace)
 *
 * Notes:
 *   - Does not own the underlying context
 *   - Lifetime is controlled by server execution
 */

#pragma once

#include <cnerium/app/AppContext.hpp>
#include <cnerium/server/Context.hpp>

namespace cnerium::app::detail
{
  /**
   * @brief Adapt a server context into an AppContext.
   *
   * @param ctx Server context
   * @return AppContext Wrapper
   */
  [[nodiscard]] inline cnerium::app::AppContext
  make_app_context(cnerium::server::Context &ctx) noexcept
  {
    return cnerium::app::AppContext(ctx);
  }

} // namespace cnerium::app::detail
