/**
 *
 *  @file App.hpp
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

#ifndef CNERIUM_APP_APP_HPP
#define CNERIUM_APP_APP_HPP

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

#include <vix/app/App.hpp>

#include <cnerium/app/AppConfig.hpp>
#include <cnerium/app/AppRuntime.hpp>
#include <cnerium/http/DurableHandler.hpp>
#include <cnerium/http/DurableRoute.hpp>
#include <cnerium/realtime/Event.hpp>
#include <cnerium/realtime/EventPayload.hpp>

namespace cnerium::app
{
  /**
   * @brief Main Cnerium application facade.
   *
   * App is the public entry point for building reliability-first applications
   * on top of Vix and the Softadastra SDK.
   *
   * Cnerium does not create its own HTTP server, router, middleware engine,
   * session system, WebSocket protocol, or durability engine.
   *
   * Responsibilities:
   * - expose durable route helpers
   * - connect durable handlers to Vix HTTP routes
   * - use Softadastra SDK-backed storage through AppRuntime
   * - expose simple realtime event emission through Vix WebSocket
   */
  class App
  {
  public:
    /**
     * @brief Creates a Cnerium app with default development configuration.
     */
    App();

    /**
     * @brief Creates a Cnerium app with explicit configuration.
     *
     * @param config Cnerium application configuration.
     */
    explicit App(AppConfig config);

    /**
     * @brief Destroys the application and stops runtime resources.
     */
    ~App();

    App(const App &) = delete;
    App &operator=(const App &) = delete;

    /**
     * @brief Moves an application.
     */
    App(App &&other) noexcept;

    /**
     * @brief Move-assigns an application.
     */
    App &operator=(App &&other) noexcept;

    /**
     * @brief Registers a durable POST route.
     *
     * The route is registered into the underlying Vix app.
     *
     * @param path HTTP path.
     * @param operation Stable operation name used by idempotency.
     * @param handler User-defined durable route handler.
     * @return Current application.
     */
    App &durable_post(
        std::string path,
        std::string operation,
        http::DurableHandler handler);

    /**
     * @brief Enables realtime WebSocket support.
     *
     * @param endpoint Public WebSocket endpoint.
     * @param host WebSocket bind host.
     * @param port WebSocket bind port.
     * @return Current application.
     */
    App &realtime(
        std::string endpoint = "/ws",
        std::string host = "0.0.0.0",
        std::uint16_t port = 9090);

    /**
     * @brief Emits a realtime event to all connected clients.
     *
     * @param event Event to emit.
     * @return true if the event was emitted.
     */
    [[nodiscard]] bool emit(const realtime::Event &event);

    /**
     * @brief Emits a realtime event to all connected clients.
     *
     * @param type Event type.
     * @param payload Event payload.
     * @return true if the event was emitted.
     */
    [[nodiscard]] bool emit(
        std::string type,
        realtime::EventPayload payload = support::object());

    /**
     * @brief Emits a realtime event to a room.
     *
     * @param room Room name.
     * @param event Event to emit.
     * @return true if the event was emitted.
     */
    [[nodiscard]] bool emit_to(
        const std::string &room,
        const realtime::Event &event);

    /**
     * @brief Emits a realtime event to a room.
     *
     * @param room Room name.
     * @param type Event type.
     * @param payload Event payload.
     * @return true if the event was emitted.
     */
    [[nodiscard]] bool emit_to(
        const std::string &room,
        std::string type,
        realtime::EventPayload payload = support::object());

    /**
     * @brief Starts the Cnerium runtime and runs the underlying Vix app.
     *
     * @return Process exit code.
     */
    [[nodiscard]] int run();

    /**
     * @brief Starts runtime resources without blocking on the HTTP app.
     *
     * @return true if started successfully.
     */
    [[nodiscard]] bool start();

    /**
     * @brief Stops runtime resources and closes the underlying Vix app.
     */
    void stop() noexcept;

    /**
     * @brief Returns true if the runtime is running.
     *
     * @return true if running.
     */
    [[nodiscard]] bool is_running() const noexcept;

    /**
     * @brief Returns the Cnerium runtime.
     *
     * @return Application runtime.
     */
    [[nodiscard]] AppRuntime &runtime() noexcept;

    /**
     * @brief Returns the Cnerium runtime.
     *
     * @return Application runtime.
     */
    [[nodiscard]] const AppRuntime &runtime() const noexcept;

    /**
     * @brief Returns the underlying Vix app.
     *
     * @return Vix app.
     */
    [[nodiscard]] vix::App &vix_app() noexcept;

    /**
     * @brief Returns the underlying Vix app.
     *
     * @return Vix app.
     */
    [[nodiscard]] const vix::App &vix_app() const noexcept;

    /**
     * @brief Returns the Cnerium application configuration.
     *
     * @return Application configuration.
     */
    [[nodiscard]] const AppConfig &config() const noexcept;

    /**
     * @brief Returns the Cnerium application configuration.
     *
     * @return Mutable application configuration.
     */
    [[nodiscard]] AppConfig &config() noexcept;

  private:
    vix::App app_;
    AppRuntime runtime_;
  };

} // namespace cnerium::app

namespace cnerium
{
  /**
   * @brief Public convenience alias for the Cnerium application.
   */
  using App = app::App;

} // namespace cnerium

#endif // CNERIUM_APP_APP_HPP
