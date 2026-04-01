/**
 * @file RouteHelpers.hpp
 * @brief cnerium::app::detail — Small helpers for app route registration
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines small internal helpers used by the App layer to register
 * routes on top of the lower-level server API.
 *
 * Responsibilities:
 *   - normalize route patterns
 *   - adapt AppHandler to server::Handler
 *   - centralize route registration helpers
 *   - reduce duplication inside App
 *
 * Design goals:
 *   - Header-only
 *   - Minimal overhead
 *   - No hidden ownership
 *   - Internal use only (detail namespace)
 *
 * Notes:
 *   - These helpers are intentionally small
 *   - The real HTTP execution still belongs to cnerium::server
 *   - App uses these helpers to expose a cleaner user-facing API
 */

#pragma once

#include <string>
#include <utility>

#include <cnerium/app/AppHandler.hpp>
#include <cnerium/app/detail/HandlerAdapter.hpp>

#include <cnerium/http/Method.hpp>
#include <cnerium/server/Server.hpp>

namespace cnerium::app::detail
{
  /**
   * @brief Return a normalized route pattern.
   *
   * Rules:
   *   - empty pattern becomes "/"
   *   - pattern always starts with '/'
   *
   * @param pattern Input route pattern
   * @return std::string Normalized route pattern
   */
  [[nodiscard]] inline std::string normalize_route_pattern(std::string pattern)
  {
    if (pattern.empty())
    {
      return "/";
    }

    if (pattern.front() != '/')
    {
      pattern.insert(pattern.begin(), '/');
    }

    return pattern;
  }

  /**
   * @brief Register one app route on the underlying server.
   *
   * @param server Target server
   * @param method HTTP method
   * @param pattern Route pattern
   * @param handler App-level handler
   */
  inline void add_route(
      cnerium::server::Server &server,
      cnerium::http::Method method,
      std::string pattern,
      cnerium::app::AppHandler handler)
  {
    server.add(method,
               normalize_route_pattern(std::move(pattern)),
               make_server_handler(std::move(handler)));
  }

  /**
   * @brief Register a GET route on the underlying server.
   *
   * @param server Target server
   * @param pattern Route pattern
   * @param handler App-level handler
   */
  inline void add_get(
      cnerium::server::Server &server,
      std::string pattern,
      cnerium::app::AppHandler handler)
  {
    add_route(server,
              cnerium::http::Method::Get,
              std::move(pattern),
              std::move(handler));
  }

  /**
   * @brief Register a POST route on the underlying server.
   *
   * @param server Target server
   * @param pattern Route pattern
   * @param handler App-level handler
   */
  inline void add_post(
      cnerium::server::Server &server,
      std::string pattern,
      cnerium::app::AppHandler handler)
  {
    add_route(server,
              cnerium::http::Method::Post,
              std::move(pattern),
              std::move(handler));
  }

  /**
   * @brief Register a PUT route on the underlying server.
   *
   * @param server Target server
   * @param pattern Route pattern
   * @param handler App-level handler
   */
  inline void add_put(
      cnerium::server::Server &server,
      std::string pattern,
      cnerium::app::AppHandler handler)
  {
    add_route(server,
              cnerium::http::Method::Put,
              std::move(pattern),
              std::move(handler));
  }

  /**
   * @brief Register a DELETE route on the underlying server.
   *
   * @param server Target server
   * @param pattern Route pattern
   * @param handler App-level handler
   */
  inline void add_delete(
      cnerium::server::Server &server,
      std::string pattern,
      cnerium::app::AppHandler handler)
  {
    add_route(server,
              cnerium::http::Method::Delete,
              std::move(pattern),
              std::move(handler));
  }

} // namespace cnerium::app::detail
