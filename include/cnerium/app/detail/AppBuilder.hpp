/**
 * @file AppBuilder.hpp
 * @brief cnerium::app::detail - Build low-level server/runtime objects from AppConfig
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines small internal helpers used by the App layer to translate the
 * high-level AppConfig into the lower-level configuration objects required
 * by:
 *   - cnerium::server
 *   - cnerium::runtime
 *
 * Responsibilities:
 *   - build server::Config from AppConfig
 *   - build runtime::RuntimeConfig from AppConfig
 *   - centralize config translation logic
 *   - keep App.hpp small and focused
 *
 * Design goals:
 *   - Header-only
 *   - Deterministic
 *   - No hidden allocation
 *   - Internal use only (detail namespace)
 *
 * Notes:
 *   - AppConfig is the public user-facing configuration
 *   - server::Config and runtime::RuntimeConfig remain low-level internals
 *   - This file is an internal bridge between the app layer and lower layers
 */

#pragma once

#include <cnerium/app/AppConfig.hpp>

#include <cnerium/runtime/RuntimeConfig.hpp>
#include <cnerium/server/Config.hpp>

namespace cnerium::app::detail
{
  /**
   * @brief Build a server configuration from an app configuration.
   *
   * This performs a full projection of the high-level app configuration
   * onto the low-level HTTP server configuration so no server field is
   * silently left at a mismatched default.
   *
   * @param config High-level app configuration
   * @return cnerium::server::Config Low-level server configuration
   */
  [[nodiscard]] inline cnerium::server::Config
  make_server_config(const cnerium::app::AppConfig &config)
  {
    cnerium::server::Config out;
    out.host = config.host;
    out.port = config.port;
    out.backlog = config.backlog;
    out.read_buffer_size = config.read_buffer_size;
    out.max_request_body_size = config.max_request_body_size;
    out.max_header_size = config.max_header_size;
    out.max_requests_per_connection = config.max_requests_per_connection;
    out.read_timeout_ms = config.read_timeout_ms;
    out.write_timeout_ms = config.write_timeout_ms;
    out.keep_alive_timeout_ms = config.keep_alive_timeout_ms;
    return out;
  }

  /**
   * @brief Build a runtime configuration from an app configuration.
   *
   * @param config High-level app configuration
   * @return cnerium::runtime::RuntimeConfig Low-level runtime configuration
   */
  [[nodiscard]] inline cnerium::runtime::RuntimeConfig
  make_runtime_config(const cnerium::app::AppConfig &config)
  {
    cnerium::runtime::RuntimeConfig out;
    out.thread_count = config.thread_count;
    return out;
  }

} // namespace cnerium::app::detail
