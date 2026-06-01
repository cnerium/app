/**
 *
 *  @file AppConfig.hpp
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

#ifndef CNERIUM_APP_APP_CONFIG_HPP
#define CNERIUM_APP_APP_CONFIG_HPP

#include <cstdint>
#include <string>

#include <cnerium/realtime/RealtimeConfig.hpp>

namespace cnerium::app
{
  /**
   * @brief Configuration for a Cnerium application.
   *
   * AppConfig contains only Cnerium-level configuration.
   *
   * Vix remains responsible for HTTP server configuration, routing, middleware,
   * runtime execution, and production workflows.
   *
   * Softadastra SDK remains responsible for durable storage, sync, transport,
   * discovery, and local-first guarantees.
   */
  class AppConfig
  {
  public:
    /**
     * @brief Creates a default Cnerium application configuration.
     */
    AppConfig() = default;

    /**
     * @brief Creates a Cnerium application configuration.
     *
     * @param name Application name.
     * @param data_dir Directory used by Cnerium for framework data.
     * @param node_id Local Softadastra node identifier.
     */
    AppConfig(
        std::string name,
        std::string data_dir,
        std::string node_id);

    /**
     * @brief Creates a default development configuration.
     *
     * @return Application configuration.
     */
    [[nodiscard]] static AppConfig development();

    /**
     * @brief Creates a default production configuration.
     *
     * @return Application configuration.
     */
    [[nodiscard]] static AppConfig production();

    /**
     * @brief Returns the application name.
     *
     * @return Application name.
     */
    [[nodiscard]] const std::string &name() const noexcept;

    /**
     * @brief Returns the application data directory.
     *
     * @return Data directory.
     */
    [[nodiscard]] const std::string &data_dir() const noexcept;

    /**
     * @brief Returns the local Softadastra node identifier.
     *
     * @return Node identifier.
     */
    [[nodiscard]] const std::string &node_id() const noexcept;

    /**
     * @brief Returns the Vix configuration file path.
     *
     * @return Vix config path.
     */
    [[nodiscard]] const std::string &vix_config_path() const noexcept;

    /**
     * @brief Returns the Cnerium store file path.
     *
     * @return Store path.
     */
    [[nodiscard]] std::string store_path() const;

    /**
     * @brief Returns the Cnerium WAL file path.
     *
     * @return WAL path.
     */
    [[nodiscard]] std::string wal_path() const;

    /**
     * @brief Returns the realtime configuration.
     *
     * @return Realtime configuration.
     */
    [[nodiscard]] const realtime::RealtimeConfig &realtime() const noexcept;

    /**
     * @brief Returns the realtime configuration.
     *
     * @return Mutable realtime configuration.
     */
    [[nodiscard]] realtime::RealtimeConfig &realtime() noexcept;

    /**
     * @brief Returns true if realtime support is enabled.
     *
     * @return true if realtime is enabled.
     */
    [[nodiscard]] bool realtime_enabled() const noexcept;

    /**
     * @brief Sets the application name.
     *
     * @param value Application name.
     */
    void set_name(std::string value);

    /**
     * @brief Sets the application data directory.
     *
     * @param value Data directory.
     */
    void set_data_dir(std::string value);

    /**
     * @brief Sets the local Softadastra node identifier.
     *
     * @param value Node identifier.
     */
    void set_node_id(std::string value);

    /**
     * @brief Sets the Vix configuration file path.
     *
     * @param value Vix config path.
     */
    void set_vix_config_path(std::string value);

    /**
     * @brief Sets the realtime configuration.
     *
     * @param value Realtime configuration.
     */
    void set_realtime(realtime::RealtimeConfig value);

    /**
     * @brief Enables realtime support.
     *
     * @param endpoint Public WebSocket endpoint.
     * @param host WebSocket bind host.
     * @param port WebSocket bind port.
     */
    void enable_realtime(
        std::string endpoint = "/ws",
        std::string host = "0.0.0.0",
        std::uint16_t port = 9090);

    /**
     * @brief Disables realtime support.
     */
    void disable_realtime() noexcept;

    /**
     * @brief Returns true if the application name is configured.
     *
     * @return true if name is not empty.
     */
    [[nodiscard]] bool has_name() const noexcept;

    /**
     * @brief Returns true if the data directory is configured.
     *
     * @return true if data directory is not empty.
     */
    [[nodiscard]] bool has_data_dir() const noexcept;

    /**
     * @brief Returns true if the node identifier is configured.
     *
     * @return true if node id is not empty.
     */
    [[nodiscard]] bool has_node_id() const noexcept;

    /**
     * @brief Returns true if the Vix config path is configured.
     *
     * @return true if Vix config path is not empty.
     */
    [[nodiscard]] bool has_vix_config_path() const noexcept;

    /**
     * @brief Returns true if the configuration can be used.
     *
     * @return true if the configuration is valid.
     */
    [[nodiscard]] bool is_valid() const noexcept;

    /**
     * @brief Backward-compatible alias for is_valid().
     *
     * @return true if the configuration is valid.
     */
    [[nodiscard]] bool valid() const noexcept;

    /**
     * @brief Clears the configuration.
     */
    void clear() noexcept;

  private:
    std::string name_{"cnerium-app"};
    std::string data_dir_{"data/cnerium"};
    std::string node_id_{"cnerium-node"};
    std::string vix_config_path_{"vix.json"};
    realtime::RealtimeConfig realtime_{realtime::RealtimeConfig::disabled()};
  };

} // namespace cnerium::app

#endif // CNERIUM_APP_APP_CONFIG_HPP
