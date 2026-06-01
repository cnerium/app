/**
 *
 *  @file Store.hpp
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

#ifndef CNERIUM_STORE_STORE_HPP
#define CNERIUM_STORE_STORE_HPP

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

#include <softadastra/sdk/Client.hpp>

#include <cnerium/store/StoreKey.hpp>
#include <cnerium/store/StoredResponse.hpp>

namespace cnerium::store
{
  /**
   * @brief Cnerium storage facade backed by the Softadastra SDK client.
   *
   * Store is the only storage entry point used by Cnerium internals.
   *
   * It intentionally wraps the public Softadastra SDK API instead of depending
   * on internal Softadastra modules such as store engines, sync engines,
   * transport engines, or WAL internals.
   *
   * Responsibilities:
   * - open and close the Softadastra SDK client
   * - store framework key/value records
   * - store and restore idempotent HTTP responses
   * - expose small helpers needed by durable routes
   */
  class Store
  {
  public:
    /**
     * @brief Creates a store with a default Softadastra SDK client.
     */
    Store();

    /**
     * @brief Creates a store from an existing Softadastra SDK client.
     *
     * @param client Softadastra SDK client.
     */
    explicit Store(softadastra::sdk::Client client);

    /**
     * @brief Destroys the store and closes the SDK client.
     */
    ~Store();

    Store(const Store &) = delete;
    Store &operator=(const Store &) = delete;

    /**
     * @brief Moves a store.
     */
    Store(Store &&other) noexcept;

    /**
     * @brief Move-assigns a store.
     */
    Store &operator=(Store &&other) noexcept;

    /**
     * @brief Opens the underlying Softadastra SDK client.
     *
     * @return true if the client is open after the call.
     */
    [[nodiscard]] bool open();

    /**
     * @brief Closes the underlying Softadastra SDK client.
     */
    void close() noexcept;

    /**
     * @brief Returns true if the underlying SDK client is open.
     *
     * @return true if the store is open.
     */
    [[nodiscard]] bool is_open() const noexcept;

    /**
     * @brief Backward-compatible alias for is_open().
     *
     * @return true if the store is open.
     */
    [[nodiscard]] bool opened() const noexcept;

    /**
     * @brief Stores a string value.
     *
     * @param key Cnerium store key.
     * @param value Text value.
     * @return true if the operation was submitted to the SDK.
     */
    [[nodiscard]] bool put(
        const StoreKey &key,
        std::string_view value);

    /**
     * @brief Stores a string value.
     *
     * @param key Raw key.
     * @param value Text value.
     * @return true if the operation was submitted to the SDK.
     */
    [[nodiscard]] bool put(
        std::string_view key,
        std::string_view value);

    /**
     * @brief Reads a string value.
     *
     * @param key Cnerium store key.
     * @return Stored value, or std::nullopt if unavailable.
     */
    [[nodiscard]] std::optional<std::string> get(
        const StoreKey &key) const;

    /**
     * @brief Reads a string value.
     *
     * @param key Raw key.
     * @return Stored value, or std::nullopt if unavailable.
     */
    [[nodiscard]] std::optional<std::string> get(
        std::string_view key) const;

    /**
     * @brief Returns true if a key exists.
     *
     * @param key Cnerium store key.
     * @return true if the key exists.
     */
    [[nodiscard]] bool contains(const StoreKey &key) const;

    /**
     * @brief Returns true if a key exists.
     *
     * @param key Raw key.
     * @return true if the key exists.
     */
    [[nodiscard]] bool contains(std::string_view key) const;

    /**
     * @brief Removes a key.
     *
     * @param key Cnerium store key.
     * @return true if the remove operation was submitted to the SDK.
     */
    [[nodiscard]] bool remove(const StoreKey &key);

    /**
     * @brief Removes a key.
     *
     * @param key Raw key.
     * @return true if the remove operation was submitted to the SDK.
     */
    [[nodiscard]] bool remove(std::string_view key);

    /**
     * @brief Stores a durable HTTP response.
     *
     * @param key Cnerium store key.
     * @param response Response to store.
     * @return true if the response was stored.
     */
    [[nodiscard]] bool put_response(
        const StoreKey &key,
        const StoredResponse &response);

    /**
     * @brief Reads a durable HTTP response.
     *
     * @param key Cnerium store key.
     * @return Stored response, or std::nullopt if unavailable.
     */
    [[nodiscard]] std::optional<StoredResponse> get_response(
        const StoreKey &key) const;

    /**
     * @brief Returns the number of records known by the SDK client.
     *
     * @return Store size.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Returns true if the SDK client has no known records.
     *
     * @return true if empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Advances the Softadastra SDK sync pipeline once.
     *
     * This is a small helper for applications that want manual sync ticks.
     *
     * @param prune_completed Whether completed sync entries should be pruned.
     * @return true if the tick call was submitted to the SDK.
     */
    [[nodiscard]] bool tick(bool prune_completed = true);

    /**
     * @brief Returns access to the underlying Softadastra SDK client.
     *
     * @return SDK client.
     */
    [[nodiscard]] softadastra::sdk::Client &client() noexcept;

    /**
     * @brief Returns const access to the underlying Softadastra SDK client.
     *
     * @return SDK client.
     */
    [[nodiscard]] const softadastra::sdk::Client &client() const noexcept;

  private:
    softadastra::sdk::Client client_;
  };

} // namespace cnerium::store

#endif // CNERIUM_STORE_STORE_HPP
