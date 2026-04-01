/**
 * @file HandlerAdapter.hpp
 * @brief cnerium::app::detail — Adapter: AppHandler → server::Handler
 *
 * @version 0.1.0
 *
 * @details
 * Provides a bridge between the high-level AppHandler (AppContext-based)
 * and the low-level server::Handler (server::Context-based).
 *
 * Responsibilities:
 *   - convert AppHandler into server::Handler
 *   - wrap server::Context into AppContext
 *   - ensure seamless integration between app and server layers
 *
 * Design goals:
 *   - Header-only
 *   - Zero dynamic allocation
 *   - Minimal overhead
 *   - Internal use only (detail namespace)
 *
 * Notes:
 *   - This is a critical integration point
 *   - Used internally by App to register routes
 */

#pragma once

#include <utility>

#include <cnerium/app/AppHandler.hpp>
#include <cnerium/app/detail/ContextAdapter.hpp>

#include <cnerium/server/Handler.hpp>
#include <cnerium/server/Context.hpp>

namespace cnerium::app::detail
{
  /**
   * @brief Adapt an AppHandler into a server::Handler.
   *
   * @param handler App-level handler
   * @return server::Handler compatible callable
   */
  [[nodiscard]] inline cnerium::server::Handler
  make_server_handler(cnerium::app::AppHandler handler)
  {
    return [h = std::move(handler)](cnerium::server::Context &ctx)
    {
      auto app_ctx = make_app_context(ctx);
      h(app_ctx);
    };
  }

} // namespace cnerium::app::detail
