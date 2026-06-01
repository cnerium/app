/**
 *
 *  @file RealtimeConfig.hpp
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

#ifndef CNERIUM_REALTIME_REALTIME_CONFIG_HPP
#define CNERIUM_REALTIME_REALTIME_CONFIG_HPP

#include <cstdint>
#include <string>

namespace cnerium::realtime
{
  /**
   * @brief Configuration for Cnerium realtime support.
   *
   * RealtimeConfig describes how Cnerium should attach to the Vix WebSocket
   * runtime.
   *
   * Cnerium does not implement its own WebSocket server. These options are used
   * to configure and start the existing Vix WebSocket layer from the Cnerium
   * application facade.
   */
  class RealtimeConfig
  {
  public:
    /**
     * @brief Creates a default realtime configuration.
     */
    RealtimeConfig() = default;

    /**
     * @brief Creates a realtime configuration.
     *
     * @param enabled Whether realtime support is enabled.
     * @param endpoint Public WebSocket endpoint.
     * @param host WebSocket bind host.
     * @param port WebSocket bind port.
     */
    RealtimeConfig(
        bool enabled,
        std::string endpoint,
        std::string host,
        std::uint16_t port);

    /**
     * @brief Creates an enabled realtime configuration.
     *
     * @param endpoint Public WebSocket endpoint.
     * @param host WebSocket bind host.
     * @param port WebSocket bind port.
     * @return Realtime configuration.
     */
    [[nodiscard]] static RealtimeConfig enabled(
        std::string endpoint = "/ws",
        std::string host = "0.0.0.0",
        std::uint16_t port = 9090);

    /**
     * @brief Creates a disabled realtime configuration.
     *
     * @return Realtime configuration.
     */
    [[nodiscard]] static RealtimeConfig disabled();

    /**
     * @brief Returns true if realtime support is enabled.
     *
     * @return true if enabled.
     */
    [[nodiscard]] bool is_enabled() const noexcept;

    /**
     * @brief Backward-compatible alias for is_enabled().
     *
     * @return true if enabled.
     */
    [[nodiscard]] bool enabled() const noexcept;

    /**
     * @brief Returns the public WebSocket endpoint.
     *
     * @return WebSocket endpoint.
     */
    [[nodiscard]] const std::string &endpoint() const noexcept;

    /**
     * @brief Returns the WebSocket bind host.
     *
     * @return WebSocket host.
     */
    [[nodiscard]] const std::string &host() const noexcept;

    /**
     * @brief Returns the WebSocket bind port.
     *
     * @return WebSocket port.
     */
    [[nodiscard]] std::uint16_t port() const noexcept;

    /**
     * @brief Sets whether realtime support is enabled.
     *
     * @param value Enabled state.
     */
    void set_enabled(bool value) noexcept;

    /**
     * @brief Sets the public WebSocket endpoint.
     *
     * @param value WebSocket endpoint.
     */
    void set_endpoint(std::string value);

    /**
     * @brief Sets the WebSocket bind host.
     *
     * @param value WebSocket bind host.
     */
    void set_host(std::string value);

    /**
     * @brief Sets the WebSocket bind port.
     *
     * @param value WebSocket bind port.
     */
    void set_port(std::uint16_t value) noexcept;

    /**
     * @brief Returns true if the endpoint is not empty.
     *
     * @return true if endpoint is configured.
     */
    [[nodiscard]] bool has_endpoint() const noexcept;

    /**
     * @brief Returns true if the host is not empty.
     *
     * @return true if host is configured.
     */
    [[nodiscard]] bool has_host() const noexcept;

    /**
     * @brief Returns true if the port is not zero.
     *
     * @return true if port is configured.
     */
    [[nodiscard]] bool has_port() const noexcept;

    /**
     * @brief Returns true if the configuration can be used.
     *
     * A valid enabled configuration must have:
     * - a non-empty endpoint
     * - a non-empty host
     * - a non-zero port
     *
     * Disabled configurations are always considered valid.
     *
     * @return true if valid.
     */
    [[nodiscard]] bool is_valid() const noexcept;

    /**
     * @brief Backward-compatible alias for is_valid().
     *
     * @return true if valid.
     */
    [[nodiscard]] bool valid() const noexcept;

    /**
     * @brief Clears the configuration and disables realtime support.
     */
    void clear() noexcept;

  private:
    bool enabled_{false};
    std::string endpoint_{"/ws"};
    std::string host_{"0.0.0.0"};
    std::uint16_t port_{9090};
  };

} // namespace cnerium::realtime

#endif // CNERIUM_REALTIME_REALTIME_CONFIG_HPP
