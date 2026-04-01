/**
 * @file AppConfig.hpp
 * @brief cnerium::app - High-level application configuration
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines the configuration object used by the Cnerium App layer.
 *
 * AppConfig is the single high-level configuration entry point for users.
 * It groups together the most common options needed to run an application:
 *   - network binding settings
 *   - runtime thread count
 *   - server backlog and buffer settings
 *   - request body and header size limits
 *   - socket timeout configuration
 *   - keep-alive connection limits
 *
 * This configuration is intentionally simple.
 * Internally, it can later be translated into:
 *   - cnerium::server::Config
 *   - cnerium::runtime::RuntimeConfig
 *
 * Design goals:
 *   - Header-only
 *   - Simple and explicit
 *   - Safe defaults
 *   - Easy to extend later
 *
 * Notes:
 *   - This is the public app-level config, not the low-level server/runtime config
 *   - Defaults are suitable for development and small deployments
 *   - thread_count = 0 means "use hardware concurrency"
 *
 * Usage:
 * @code
 *   using namespace cnerium::app;
 *
 *   AppConfig config;
 *   config.host = "0.0.0.0";
 *   config.port = 8080;
 *   config.thread_count = 4;
 *   config.read_timeout_ms = 5000;
 * @endcode
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <thread>

namespace cnerium::app
{
  /**
   * @brief High-level configuration object for a Cnerium application.
   *
   * This struct is the main user-facing configuration surface for the
   * Cnerium App layer. It centralizes the most useful runtime and
   * server-related settings behind one simple object.
   */
  struct AppConfig
  {
    /**
     * @brief Host address to bind the application server to.
     *
     * Examples:
     *   - "127.0.0.1" -> local only
     *   - "0.0.0.0"   -> all interfaces
     *
     * Default: "127.0.0.1"
     */
    std::string host{"127.0.0.1"};

    /**
     * @brief TCP port to listen on.
     *
     * Default: 8080
     */
    std::uint16_t port{8080};

    /**
     * @brief Number of runtime worker threads.
     *
     * 0 means "use hardware concurrency".
     *
     * Default: std::thread::hardware_concurrency()
     */
    std::size_t thread_count{std::thread::hardware_concurrency()};

    /**
     * @brief Maximum number of pending TCP connections.
     *
     * Passed to the server listen backlog.
     *
     * Default: 128
     */
    int backlog{128};

    /**
     * @brief Size of the read buffer for incoming data.
     *
     * Controls how much data is read per socket read operation.
     *
     * Default: 8 KB
     */
    std::size_t read_buffer_size{8 * 1024};

    /**
     * @brief Maximum allowed HTTP request body size.
     *
     * Used to prevent excessive memory usage and abuse.
     *
     * Default: 1 MB
     */
    std::size_t max_request_body_size{1024 * 1024};

    /**
     * @brief Maximum allowed HTTP header block size.
     *
     * Includes the request line and all header lines up to the
     * terminating CRLF CRLF sequence.
     *
     * Default: 16 KB
     */
    std::size_t max_header_size{16 * 1024};

    /**
     * @brief Maximum number of HTTP requests allowed on one persistent connection.
     *
     * Helps prevent unbounded reuse of a single keep-alive connection.
     *
     * Default: 100
     */
    std::size_t max_requests_per_connection{100};

    /**
     * @brief Socket read timeout in milliseconds.
     *
     * Applied to accepted client sockets when reading request data.
     *
     * Default: 5000 ms
     */
    std::uint32_t read_timeout_ms{5000};

    /**
     * @brief Socket write timeout in milliseconds.
     *
     * Applied to accepted client sockets when writing response data.
     *
     * Default: 5000 ms
     */
    std::uint32_t write_timeout_ms{5000};

    /**
     * @brief Idle keep-alive timeout in milliseconds.
     *
     * Indicates how long a persistent connection may remain open
     * while waiting for the next request.
     *
     * Default: 10000 ms
     */
    std::uint32_t keep_alive_timeout_ms{10000};

    /**
     * @brief Returns true if the configuration is valid.
     *
     * Performs basic validation checks on runtime and server limits.
     *
     * @return true if valid
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return !host.empty() &&
             port > 0 &&
             backlog > 0 &&
             read_buffer_size > 0 &&
             max_request_body_size > 0 &&
             max_header_size > 0 &&
             max_requests_per_connection > 0 &&
             read_timeout_ms > 0 &&
             write_timeout_ms > 0 &&
             keep_alive_timeout_ms > 0;
    }

    /**
     * @brief Reset the configuration to default values.
     */
    void reset() noexcept
    {
      host = "127.0.0.1";
      port = 8080;
      thread_count = std::thread::hardware_concurrency();
      backlog = 128;
      read_buffer_size = 8 * 1024;
      max_request_body_size = 1024 * 1024;
      max_header_size = 16 * 1024;
      max_requests_per_connection = 100;
      read_timeout_ms = 5000;
      write_timeout_ms = 5000;
      keep_alive_timeout_ms = 10000;
    }
  };

} // namespace cnerium::app
