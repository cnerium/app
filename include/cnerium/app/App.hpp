/**
 * @file App.hpp
 * @brief cnerium::app - High-level application facade
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines the App class, the main user-facing entry point of the
 * Cnerium App layer.
 *
 * App:
 *   - owns the high-level application configuration
 *   - owns the underlying server
 *   - owns the underlying runtime
 *   - exposes a simple route registration API
 *   - runs the HTTP application through ServerRunner
 *
 * Responsibilities:
 *   - provide a simple framework-style API
 *   - register routes using AppHandler
 *   - register middleware
 *   - configure server and runtime from AppConfig
 *   - start the application
 *
 * Design goals:
 *   - Header-only
 *   - Clear ownership model
 *   - Minimal public API
 *   - Easy integration with server and runtime
 *
 * Notes:
 *   - App is the preferred entry point for end users
 *   - Lower-level server/runtime objects remain accessible when needed
 *   - Middleware currently uses the lower-level server middleware signature
 *   - Server configuration is re-applied before listen/run so host/port and
 *     other server limits remain consistent with AppConfig
 */

#pragma once

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

#include <cnerium/app/AppConfig.hpp>
#include <cnerium/app/AppHandler.hpp>
#include <cnerium/app/detail/AppBuilder.hpp>
#include <cnerium/app/detail/Logger.hpp>
#include <cnerium/app/detail/RouteHelpers.hpp>

#include <cnerium/http/Method.hpp>
#include <cnerium/middleware/Middleware.hpp>
#include <cnerium/runtime/Runtime.hpp>
#include <cnerium/runtime/ServerRunner.hpp>
#include <cnerium/server/Config.hpp>
#include <cnerium/server/Context.hpp>
#include <cnerium/server/ErrorHandler.hpp>
#include <cnerium/server/Handler.hpp>
#include <cnerium/server/Server.hpp>

namespace cnerium::app
{
  /**
   * @brief High-level application facade.
   *
   * Owns the runtime and server layers and exposes a simpler API.
   */
  class App
  {
  public:
    using config_type = cnerium::app::AppConfig;
    using handler_type = cnerium::app::AppHandler;

    using runtime_type = cnerium::runtime::Runtime;
    using server_type = cnerium::server::Server;
    using runner_type = cnerium::runtime::ServerRunner;

    using middleware_type = cnerium::middleware::Middleware;
    using error_handler_type = cnerium::server::ErrorHandler;
    using not_found_handler_type = cnerium::server::Handler;
    using listen_callback_type = std::function<void()>;

    /**
     * @brief Construct an application with default configuration.
     */
    App()
        : App(config_type{})
    {
    }

    /**
     * @brief Construct an application with explicit configuration.
     *
     * @param config High-level app configuration
     */
    explicit App(config_type config)
        : config_(std::move(config)),
          runtime_(detail::make_runtime_config(config_)),
          server_(detail::make_server_config(config_))
    {
    }

    App(const App &) = delete;
    App &operator=(const App &) = delete;
    App(App &&) = delete;
    App &operator=(App &&) = delete;

    /**
     * @brief Returns mutable access to the app configuration.
     *
     * @return config_type& App configuration
     */
    [[nodiscard]] config_type &config() noexcept
    {
      return config_;
    }

    /**
     * @brief Returns const access to the app configuration.
     *
     * @return const config_type& App configuration
     */
    [[nodiscard]] const config_type &config() const noexcept
    {
      return config_;
    }

    /**
     * @brief Returns mutable access to the underlying runtime.
     *
     * @return runtime_type& Runtime
     */
    [[nodiscard]] runtime_type &runtime() noexcept
    {
      return runtime_;
    }

    /**
     * @brief Returns const access to the underlying runtime.
     *
     * @return const runtime_type& Runtime
     */
    [[nodiscard]] const runtime_type &runtime() const noexcept
    {
      return runtime_;
    }

    /**
     * @brief Returns mutable access to the underlying server.
     *
     * @return server_type& Server
     */
    [[nodiscard]] server_type &server() noexcept
    {
      return server_;
    }

    /**
     * @brief Returns const access to the underlying server.
     *
     * @return const server_type& Server
     */
    [[nodiscard]] const server_type &server() const noexcept
    {
      return server_;
    }

    /**
     * @brief Register a GET route.
     *
     * @param pattern Route pattern
     * @param handler App-level handler
     * @return App& Self
     */
    App &get(std::string pattern, handler_type handler)
    {
      detail::add_get(server_, std::move(pattern), std::move(handler));
      return *this;
    }

    /**
     * @brief Register a POST route.
     *
     * @param pattern Route pattern
     * @param handler App-level handler
     * @return App& Self
     */
    App &post(std::string pattern, handler_type handler)
    {
      detail::add_post(server_, std::move(pattern), std::move(handler));
      return *this;
    }

    /**
     * @brief Register a PUT route.
     *
     * @param pattern Route pattern
     * @param handler App-level handler
     * @return App& Self
     */
    App &put(std::string pattern, handler_type handler)
    {
      detail::add_put(server_, std::move(pattern), std::move(handler));
      return *this;
    }

    /**
     * @brief Register a DELETE route.
     *
     * @param pattern Route pattern
     * @param handler App-level handler
     * @return App& Self
     */
    App &delete_(std::string pattern, handler_type handler)
    {
      detail::add_delete(server_, std::move(pattern), std::move(handler));
      return *this;
    }

    /**
     * @brief Register a route with explicit HTTP method.
     *
     * @param method HTTP method
     * @param pattern Route pattern
     * @param handler App-level handler
     * @return App& Self
     */
    App &route(cnerium::http::Method method,
               std::string pattern,
               handler_type handler)
    {
      detail::add_route(server_,
                        method,
                        std::move(pattern),
                        std::move(handler));
      return *this;
    }

    /**
     * @brief Register one global middleware on the underlying server.
     *
     * @param middleware Middleware callable
     * @return App& Self
     */
    App &use(middleware_type middleware)
    {
      server_.use(std::move(middleware));
      return *this;
    }

    /**
     * @brief Set the application error handler.
     *
     * @param handler Error handler callable
     * @return App& Self
     */
    App &set_error_handler(error_handler_type handler)
    {
      server_.set_error_handler(std::move(handler));
      return *this;
    }

    /**
     * @brief Set the application not-found handler.
     *
     * @param handler Not-found handler callable
     * @return App& Self
     */
    App &set_not_found_handler(not_found_handler_type handler)
    {
      server_.set_not_found_handler(std::move(handler));
      return *this;
    }

    /**
     * @brief Start the runtime explicitly.
     *
     * @return App& Self
     */
    App &start()
    {
      runtime_.start();
      return *this;
    }

    /**
     * @brief Request cooperative stop of the runtime.
     *
     * @return App& Self
     */
    App &stop() noexcept
    {
      runtime_.stop();
      return *this;
    }

    /**
     * @brief Wait for runtime workers to finish.
     *
     * @return App& Self
     */
    App &join() noexcept
    {
      runtime_.join();
      return *this;
    }

    /**
     * @brief Listen using the current application configuration.
     *
     * Equivalent to run().
     */
    void listen()
    {
      run_internal(nullptr);
    }

    /**
     * @brief Listen using the current application configuration.
     *
     * @param callback Optional callback executed just before entering
     * the blocking server loop.
     */
    void listen(listen_callback_type callback)
    {
      run_internal(&callback);
    }

    /**
     * @brief Listen on a specific port.
     *
     * Uses the current configured host.
     *
     * @param port TCP port
     */
    void listen(std::uint16_t port)
    {
      config_.port = port;
      run_internal(nullptr);
    }

    /**
     * @brief Listen on a specific port.
     *
     * Uses the current configured host.
     *
     * @param port TCP port
     * @param callback Optional callback executed just before entering
     * the blocking server loop.
     */
    void listen(std::uint16_t port, listen_callback_type callback)
    {
      config_.port = port;
      run_internal(&callback);
    }

    /**
     * @brief Listen on a specific host and port.
     *
     * @param host Bind host
     * @param port TCP port
     */
    void listen(std::string host, std::uint16_t port)
    {
      config_.host = std::move(host);
      config_.port = port;
      run_internal(nullptr);
    }

    /**
     * @brief Listen on a specific host and port.
     *
     * @param host Bind host
     * @param port TCP port
     * @param callback Optional callback executed just before entering
     * the blocking server loop.
     */
    void listen(std::string host,
                std::uint16_t port,
                listen_callback_type callback)
    {
      config_.host = std::move(host);
      config_.port = port;
      run_internal(&callback);
    }

    /**
     * @brief Run the application.
     *
     * This starts the runtime and enters the blocking server runner loop.
     */
    void run()
    {
      run_internal(nullptr);
    }

  private:
    config_type config_{};
    runtime_type runtime_;
    server_type server_;

    /**
     * @brief Apply the high-level configuration to the underlying server.
     *
     * Keeps the already-registered routes and middleware intact while
     * updating the server configuration before listen/run.
     *
     * This uses full assignment so all server-level settings remain
     * synchronized with the current AppConfig:
     *   - host
     *   - port
     *   - backlog
     *   - read buffer size
     *   - request body limits
     *   - header limits
     *   - timeouts
     *   - keep-alive limits
     */
    void apply_server_config()
    {
      server_.set_config(detail::make_server_config(config_));
    }

    /**
     * @brief Internal application runner shared by run() and listen().
     *
     * @param callback Optional startup callback
     */
    void run_internal(listen_callback_type *callback)
    {
      if (!config_.valid())
      {
        throw std::logic_error("cannot run app with invalid configuration");
      }

      apply_server_config();

      const auto &cfg = server_.config();

      cnerium::app::detail::startup_banner(
          "Cnerium app",
          "0.1.0",
          cfg.host,
          std::to_string(cfg.port));

      if (callback && *callback)
      {
        (*callback)();
      }

      runner_type runner(runtime_, server_);
      runner.run();
    }
  };

} // namespace cnerium::app
