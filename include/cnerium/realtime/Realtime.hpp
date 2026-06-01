/**
 *
 *  @file Realtime.hpp
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

#ifndef CNERIUM_REALTIME_REALTIME_HPP
#define CNERIUM_REALTIME_REALTIME_HPP

#include <memory>
#include <string>

#include <vix/config/Config.hpp>
#include <vix/executor/RuntimeExecutor.hpp>
#include <vix/websocket/server.hpp>

#include <cnerium/realtime/Event.hpp>
#include <cnerium/realtime/EventPayload.hpp>
#include <cnerium/realtime/RealtimeConfig.hpp>

namespace cnerium::realtime
{
  /**
   * @brief Cnerium realtime facade backed by Vix WebSocket.
   *
   * Realtime exposes a small application-level API for emitting typed events
   * from Cnerium applications.
   *
   * Cnerium does not implement its own WebSocket server, WebSocket session,
   * protocol parser, frame parser, or room system. All low-level realtime
   * behavior is delegated to the existing Vix WebSocket runtime.
   */
  class Realtime
  {
  public:
    /**
     * @brief Creates a disabled realtime facade.
     */
    Realtime();

    /**
     * @brief Creates a realtime facade with configuration.
     *
     * @param config Realtime configuration.
     */
    explicit Realtime(RealtimeConfig config);

    /**
     * @brief Destroys the realtime facade and stops the WebSocket server.
     */
    ~Realtime();

    Realtime(const Realtime &) = delete;
    Realtime &operator=(const Realtime &) = delete;

    /**
     * @brief Moves a realtime facade.
     */
    Realtime(Realtime &&other) noexcept;

    /**
     * @brief Move-assigns a realtime facade.
     */
    Realtime &operator=(Realtime &&other) noexcept;

    /**
     * @brief Configures realtime support.
     *
     * @param config Realtime configuration.
     */
    void configure(RealtimeConfig config);

    /**
     * @brief Enables realtime support.
     *
     * @param endpoint Public WebSocket endpoint.
     * @param host WebSocket bind host.
     * @param port WebSocket bind port.
     */
    void enable(
        std::string endpoint = "/ws",
        std::string host = "0.0.0.0",
        std::uint16_t port = 9090);

    /**
     * @brief Disables realtime support.
     */
    void disable() noexcept;

    /**
     * @brief Starts the Vix WebSocket server.
     *
     * @param core_config Vix core configuration.
     * @param executor Shared Vix runtime executor.
     * @return true if realtime is disabled or started successfully.
     */
    [[nodiscard]] bool start(
        vix::config::Config &core_config,
        std::shared_ptr<vix::executor::RuntimeExecutor> executor);

    /**
     * @brief Stops the Vix WebSocket server.
     */
    void stop() noexcept;

    /**
     * @brief Emits a realtime event.
     *
     * @param event Event to emit.
     * @return true if the event was accepted for broadcast.
     */
    [[nodiscard]] bool emit(const Event &event);

    /**
     * @brief Emits a realtime event.
     *
     * @param type Event type.
     * @param payload Event payload.
     * @return true if the event was accepted for broadcast.
     */
    [[nodiscard]] bool emit(
        std::string type,
        EventPayload payload = support::object());

    /**
     * @brief Emits a realtime event to a room.
     *
     * @param room Room name.
     * @param event Event to emit.
     * @return true if the event was accepted for broadcast.
     */
    [[nodiscard]] bool emit_to(
        const std::string &room,
        const Event &event);

    /**
     * @brief Emits a realtime event to a room.
     *
     * @param room Room name.
     * @param type Event type.
     * @param payload Event payload.
     * @return true if the event was accepted for broadcast.
     */
    [[nodiscard]] bool emit_to(
        const std::string &room,
        std::string type,
        EventPayload payload = support::object());

    /**
     * @brief Returns true if realtime support is enabled.
     *
     * @return true if enabled.
     */
    [[nodiscard]] bool is_enabled() const noexcept;

    /**
     * @brief Returns true if the Vix WebSocket server has been started.
     *
     * @return true if started.
     */
    [[nodiscard]] bool is_running() const noexcept;

    /**
     * @brief Returns the realtime configuration.
     *
     * @return Realtime configuration.
     */
    [[nodiscard]] const RealtimeConfig &config() const noexcept;

    /**
     * @brief Returns the underlying Vix WebSocket server.
     *
     * @return Vix WebSocket server pointer, or nullptr if not started.
     */
    [[nodiscard]] vix::websocket::Server *server() noexcept;

    /**
     * @brief Returns the underlying Vix WebSocket server.
     *
     * @return Vix WebSocket server pointer, or nullptr if not started.
     */
    [[nodiscard]] const vix::websocket::Server *server() const noexcept;

  private:
    RealtimeConfig config_{};
    std::shared_ptr<vix::executor::RuntimeExecutor> executor_{};
    std::unique_ptr<vix::websocket::Server> server_{};
    bool running_{false};
  };

} // namespace cnerium::realtime

namespace cnerium
{
  /**
   * @brief Public convenience alias for realtime support.
   */
  using Realtime = realtime::Realtime;

} // namespace cnerium

#endif // CNERIUM_REALTIME_REALTIME_HPP
