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
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

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
   * @brief Cnerium layer attached to an existing vix::App.
   *
   * AttachedApp is the public Cnerium entry point.
   *
   * It does not own the Vix application.
   * It does not replace Vix.
   * It does not create a second backend framework.
   *
   * Instead, it attaches reliability-first features to an existing vix::App.
   *
   * Vix remains responsible for:
   * - HTTP server
   * - routing
   * - middleware
   * - request parsing
   * - response writing
   * - application runtime
   * - developer workflow
   *
   * Cnerium adds:
   * - durable routes
   * - idempotency
   * - replay protection
   * - stored responses
   * - Softadastra SDK-backed persistence
   * - realtime application events through Vix WebSocket
   */
  class AttachedApp
  {
  public:
    /**
     * @brief Attaches Cnerium to an existing Vix app with default configuration.
     *
     * @param app Existing Vix application.
     */
    explicit AttachedApp(vix::App &app);

    /**
     * @brief Attaches Cnerium to an existing Vix app with explicit configuration.
     *
     * @param app Existing Vix application.
     * @param config Cnerium configuration.
     */
    AttachedApp(
        vix::App &app,
        AppConfig config);

    /**
     * @brief Stops Cnerium runtime resources.
     */
    ~AttachedApp();

    AttachedApp(const AttachedApp &) = delete;
    AttachedApp &operator=(const AttachedApp &) = delete;

    /**
     * @brief Moves an attached Cnerium layer.
     */
    AttachedApp(AttachedApp &&other) noexcept;

    /**
     * @brief Move-assigns an attached Cnerium layer.
     */
    AttachedApp &operator=(AttachedApp &&other) noexcept;

    /**
     * @brief Registers a durable POST route into the attached Vix app.
     *
     * The route remains a Vix route.
     * Cnerium only wraps the handler with durability, idempotency and replay
     * protection.
     *
     * @param path HTTP path.
     * @param operation Stable operation name used by idempotency.
     * @param handler User-defined durable route handler.
     * @return Current attached Cnerium layer.
     */
    AttachedApp &durable_post(
        std::string path,
        std::string operation,
        http::DurableHandler handler);

    /**
     * @brief Enables realtime WebSocket support through Vix WebSocket.
     *
     * @param endpoint Public WebSocket endpoint.
     * @param host WebSocket bind host.
     * @param port WebSocket bind port.
     * @return Current attached Cnerium layer.
     */
    AttachedApp &realtime(
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
     * @brief Starts Cnerium runtime resources.
     *
     * This opens the Softadastra SDK-backed store and starts realtime support
     * when enabled.
     *
     * It does not run the HTTP server.
     * The HTTP server remains started by vix::App::run().
     *
     * @return true if started successfully.
     */
    [[nodiscard]] bool start();

    /**
     * @brief Stops Cnerium runtime resources.
     */
    void stop() noexcept;

    /**
     * @brief Returns true if Cnerium runtime resources are running.
     *
     * @return true if running.
     */
    [[nodiscard]] bool is_running() const noexcept;

    /**
     * @brief Returns the Cnerium runtime.
     *
     * @return Runtime.
     */
    [[nodiscard]] AppRuntime &runtime() noexcept;

    /**
     * @brief Returns the Cnerium runtime.
     *
     * @return Runtime.
     */
    [[nodiscard]] const AppRuntime &runtime() const noexcept;

    /**
     * @brief Returns the attached Vix app.
     *
     * @return Vix app.
     */
    [[nodiscard]] vix::App &vix_app() noexcept;

    /**
     * @brief Returns the attached Vix app.
     *
     * @return Vix app.
     */
    [[nodiscard]] const vix::App &vix_app() const noexcept;

    /**
     * @brief Returns the Cnerium configuration.
     *
     * @return Configuration.
     */
    [[nodiscard]] const AppConfig &config() const noexcept;

    /**
     * @brief Returns the mutable Cnerium configuration.
     *
     * @return Configuration.
     */
    [[nodiscard]] AppConfig &config() noexcept;

  private:
    /**
     * @brief Returns true if this layer is attached to a Vix app.
     *
     * @return true if attached.
     */
    [[nodiscard]] bool has_app() const noexcept;

  private:
    vix::App *app_{nullptr};
    AppRuntime runtime_{};
    std::vector<std::unique_ptr<http::DurableRoute>> durable_routes_{};
  };

  /**
   * @brief Attaches Cnerium to an existing Vix app.
   *
   * This is the recommended public entry point.
   *
   * Example:
   *
   * ```cpp
   * vix::App app;
   * auto cnerium = cnerium::attach(app);
   * ```
   *
   * @param app Existing Vix application.
   * @return Attached Cnerium layer.
   */
  [[nodiscard]] AttachedApp attach(vix::App &app);

  /**
   * @brief Attaches Cnerium to an existing Vix app with explicit configuration.
   *
   * @param app Existing Vix application.
   * @param config Cnerium configuration.
   * @return Attached Cnerium layer.
   */
  [[nodiscard]] AttachedApp attach(
      vix::App &app,
      AppConfig config);

} // namespace cnerium::app

namespace cnerium
{
  /**
   * @brief Public Cnerium layer attached to vix::App.
   */
  using AttachedApp = app::AttachedApp;

  /**
   * @brief Attaches Cnerium to an existing Vix app.
   *
   * Cnerium remains inside the Vix ecosystem.
   * Vix owns the backend.
   * Cnerium adds reliability-first backend features.
   *
   * @param app Existing Vix application.
   * @return Attached Cnerium layer.
   */
  [[nodiscard]] app::AttachedApp attach(vix::App &app);

  /**
   * @brief Attaches Cnerium to an existing Vix app with explicit configuration.
   *
   * @param app Existing Vix application.
   * @param config Cnerium configuration.
   * @return Attached Cnerium layer.
   */
  [[nodiscard]] app::AttachedApp attach(
      vix::App &app,
      app::AppConfig config);

} // namespace cnerium

#endif // CNERIUM_APP_APP_HPP
