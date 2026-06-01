/**
 *
 *  @file AppRuntime.hpp
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

#ifndef CNERIUM_APP_APP_RUNTIME_HPP
#define CNERIUM_APP_APP_RUNTIME_HPP

#include <memory>

#include <softadastra/sdk/Client.hpp>
#include <softadastra/sdk/ClientOptions.hpp>

#include <vix/config/Config.hpp>
#include <vix/executor/RuntimeExecutor.hpp>

#include <cnerium/adapters/SoftadastraStore.hpp>
#include <cnerium/adapters/VixWebSocket.hpp>
#include <cnerium/app/AppConfig.hpp>
#include <cnerium/store/Store.hpp>

namespace cnerium::app
{
  /**
   * @brief Runtime resources owned by a Cnerium application.
   *
   * AppRuntime centralizes the runtime objects used by Cnerium:
   * - Vix core configuration
   * - Vix runtime executor
   * - Softadastra SDK-backed store
   * - Vix WebSocket adapter
   *
   * It does not create a new HTTP runtime or a new WebSocket runtime.
   * It only prepares and owns the resources needed by cnerium::app::App.
   */
  class AppRuntime
  {
  public:
    /**
     * @brief Creates a runtime with default configuration.
     */
    AppRuntime();

    /**
     * @brief Creates a runtime with explicit Cnerium configuration.
     *
     * @param config Cnerium application configuration.
     */
    explicit AppRuntime(AppConfig config);

    /**
     * @brief Destroys the runtime and stops owned services.
     */
    ~AppRuntime();

    AppRuntime(const AppRuntime &) = delete;
    AppRuntime &operator=(const AppRuntime &) = delete;

    /**
     * @brief Moves a runtime.
     */
    AppRuntime(AppRuntime &&other) noexcept;

    /**
     * @brief Move-assigns a runtime.
     */
    AppRuntime &operator=(AppRuntime &&other) noexcept;

    /**
     * @brief Starts runtime resources.
     *
     * This opens the Softadastra-backed store and starts realtime support if it
     * is enabled in the application configuration.
     *
     * @return true if the runtime started successfully.
     */
    [[nodiscard]] bool start();

    /**
     * @brief Stops runtime resources.
     */
    void stop() noexcept;

    /**
     * @brief Returns true if the runtime has been started.
     *
     * @return true if started.
     */
    [[nodiscard]] bool is_running() const noexcept;

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

    /**
     * @brief Returns the Vix core configuration.
     *
     * @return Vix configuration.
     */
    [[nodiscard]] vix::config::Config &vix_config() noexcept;

    /**
     * @brief Returns the Vix core configuration.
     *
     * @return Vix configuration.
     */
    [[nodiscard]] const vix::config::Config &vix_config() const noexcept;

    /**
     * @brief Returns the shared Vix runtime executor.
     *
     * @return Shared runtime executor.
     */
    [[nodiscard]] std::shared_ptr<vix::executor::RuntimeExecutor> executor() noexcept;

    /**
     * @brief Returns the Cnerium store facade.
     *
     * @return Store.
     */
    [[nodiscard]] store::Store &store() noexcept;

    /**
     * @brief Returns the Cnerium store facade.
     *
     * @return Store.
     */
    [[nodiscard]] const store::Store &store() const noexcept;

    /**
     * @brief Returns the Vix WebSocket adapter.
     *
     * @return WebSocket adapter.
     */
    [[nodiscard]] adapters::VixWebSocket &websocket() noexcept;

    /**
     * @brief Returns the Vix WebSocket adapter.
     *
     * @return WebSocket adapter.
     */
    [[nodiscard]] const adapters::VixWebSocket &websocket() const noexcept;

    /**
     * @brief Returns true if realtime support is enabled.
     *
     * @return true if realtime is enabled.
     */
    [[nodiscard]] bool realtime_enabled() const noexcept;

    /**
     * @brief Emits a realtime event through the Vix WebSocket adapter.
     *
     * @param event Event to emit.
     * @return true if the event was emitted.
     */
    [[nodiscard]] bool emit(const realtime::Event &event);

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

  private:
    /**
     * @brief Builds the default Softadastra SDK client.
     *
     * @return SDK client.
     */
    [[nodiscard]] softadastra::sdk::Client make_client() const;

    /**
     * @brief Creates the Vix runtime executor.
     *
     * @return Shared runtime executor.
     */
    [[nodiscard]] static std::shared_ptr<vix::executor::RuntimeExecutor>
    make_executor();

  private:
    AppConfig config_{};
    vix::config::Config vix_config_;
    std::shared_ptr<vix::executor::RuntimeExecutor> executor_{};
    store::Store store_;
    adapters::VixWebSocket websocket_{};
    bool running_{false};
  };

} // namespace cnerium::app

#endif // CNERIUM_APP_APP_RUNTIME_HPP
