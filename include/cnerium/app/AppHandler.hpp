/**
 * @file AppHandler.hpp
 * @brief cnerium::app — High-level handler abstraction
 *
 * @version 0.1.0
 *
 * @details
 * Defines the AppHandler type used by the Cnerium App layer.
 *
 * An AppHandler:
 *   - operates on AppContext
 *   - represents user-defined route logic
 *   - is later adapted to server::Handler internally
 *
 * Responsibilities:
 *   - provide a clean user-facing handler signature
 *   - decouple user code from server internals
 *
 * Design goals:
 *   - Header-only
 *   - Simple and expressive
 *   - Lambda-friendly
 *
 * Notes:
 *   - This is the primary handler type used in App::get/post/etc.
 *   - Adapted internally via HandlerAdapter
 *
 * Usage:
 * @code
 *   AppHandler handler = [](AppContext &ctx)
 *   {
 *     ctx.text("Hello");
 *   };
 * @endcode
 */

#pragma once

#include <functional>

#include <cnerium/app/AppContext.hpp>

namespace cnerium::app
{
  /**
   * @brief High-level application handler type.
   *
   * Signature:
   *   void(AppContext&)
   */
  using AppHandler = std::function<void(AppContext &)>;

} // namespace cnerium::app
