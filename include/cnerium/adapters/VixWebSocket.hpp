/**
 *
 *  @file VixWebSocket.hpp
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

#ifndef CNERIUM_ADAPTERS_VIX_WEBSOCKET_HPP
#define CNERIUM_ADAPTERS_VIX_WEBSOCKET_HPP

#include <memory>
#include <string>
#include <string_view>

#include <vix/config/Config.hpp>
#include <vix/executor/RuntimeExecutor.hpp>
#include <vix/json/Simple.hpp>
#include <vix/websocket/server.hpp>

#include <cnerium/realtime/Event.hpp>
#include <cnerium/realtime/EventPayload.hpp>
#include <cnerium/realtime/RealtimeConfig.hpp>

namespace cnerium::adapters
{
  /**
   * @brief Adapter between Cnerium realtime events and Vix WebSocket.
   *
   * VixWebSocket is intentionally only an adapter.
   *
   * It does not implement a WebSocket server, session, protocol parser,
   * frame parser, router, heartbeat, or connection lifecycle. Vix already owns
   * those responsibilities.
   *
   * This adapter only:
   * - creates a Vix WebSocket server
   * - starts and stops it
   * - converts Cnerium events to Vix typed JSON payloads
   * - broadcasts events globally or to a room
   */
  class VixWebSocket
  {
  public:
    /**
     * @brief Creates an empty WebSocket adapter.
     */
    VixWebSocket() = default;

    /**
     * @brief Destroys the adapter and stops the server.
     */
    ~VixWebSocket();

    VixWebSocket(const VixWebSocket &) = delete;
    VixWebSocket &operator=(const VixWebSocket &) = delete;

    /**
     * @brief Moves a WebSocket adapter.
     */
    VixWebSocket(VixWebSocket &&other) noexcept;

    /**
     * @brief Move-assigns a WebSocket adapter.
     */
    VixWebSocket &operator=(VixWebSocket &&other) noexcept;

    /**
     * @brief Starts the Vix WebSocket server.
     *
     * @param config Cnerium realtime configuration.
     * @param core_config Vix core configuration.
     * @param executor Shared Vix runtime executor.
     * @return true if the server started successfully or realtime is disabled.
     */
    [[nodiscard]] bool start(
        const realtime::RealtimeConfig &config,
        vix::config::Config &core_config,
        std::shared_ptr<vix::executor::RuntimeExecutor> executor);

    /**
     * @brief Stops the Vix WebSocket server.
     */
    void stop() noexcept;

    /**
     * @brief Emits an event to all connected WebSocket clients.
     *
     * @param event Cnerium realtime event.
     * @return true if the event was broadcast.
     */
    [[nodiscard]] bool emit(const realtime::Event &event);

    /**
     * @brief Emits an event to a WebSocket room.
     *
     * @param room Room name.
     * @param event Cnerium realtime event.
     * @return true if the event was broadcast to the room.
     */
    [[nodiscard]] bool emit_to(
        const std::string &room,
        const realtime::Event &event);

    /**
     * @brief Returns true if the Vix WebSocket server is running.
     *
     * @return true if running.
     */
    [[nodiscard]] bool is_running() const noexcept;

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

    /**
     * @brief Converts a Cnerium event payload to a Vix WebSocket payload.
     *
     * @param payload Cnerium event payload.
     * @return Vix lightweight JSON object payload.
     */
    [[nodiscard]] static vix::json::kvs to_vix_payload(
        const realtime::EventPayload &payload);

  private:
    std::shared_ptr<vix::executor::RuntimeExecutor> executor_{};
    std::unique_ptr<vix::websocket::Server> server_{};
    bool running_{false};
  };

} // namespace cnerium::adapters

#endif // CNERIUM_ADAPTERS_VIX_WEBSOCKET_HPP
