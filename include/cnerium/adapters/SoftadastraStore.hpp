/**
 *
 *  @file SoftadastraStore.hpp
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

#ifndef CNERIUM_ADAPTERS_SOFTADASTRA_STORE_HPP
#define CNERIUM_ADAPTERS_SOFTADASTRA_STORE_HPP

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

#include <softadastra/sdk/Client.hpp>

#include <cnerium/store/StoreKey.hpp>
#include <cnerium/store/StoredResponse.hpp>

namespace cnerium::adapters
{
  /**
   * @brief Adapter around the public Softadastra SDK client.
   *
   * SoftadastraStore is the low-level Cnerium adapter that talks directly to
   * softadastra::sdk::Client.
   *
   * It exists to keep Cnerium independent from Softadastra internal modules.
   * Cnerium must use the public SDK surface only.
   *
   * This adapter is responsible for:
   * - opening and closing the SDK client
   * - writing framework key/value data
   * - reading framework key/value data
   * - storing replayable durable responses
   * - advancing the SDK sync pipeline manually when needed
   */
  class SoftadastraStore
  {
  public:
    /**
     * @brief Creates an adapter with a default SDK client.
     */
    SoftadastraStore();

    /**
     * @brief Creates an adapter from an existing SDK client.
     *
     * @param client Softadastra SDK client.
     */
    explicit SoftadastraStore(softadastra::sdk::Client client);

    /**
     * @brief Destroys the adapter and closes the SDK client.
     */
    ~SoftadastraStore();

    SoftadastraStore(const SoftadastraStore &) = delete;
    SoftadastraStore &operator=(const SoftadastraStore &) = delete;

    /**
     * @brief Moves an adapter.
     */
    SoftadastraStore(SoftadastraStore &&other) noexcept;

    /**
     * @brief Move-assigns an adapter.
     */
    SoftadastraStore &operator=(SoftadastraStore &&other) noexcept;

    /**
     * @brief Opens the underlying SDK client.
     *
     * @return true if the SDK client is open after the call.
     */
    [[nodiscard]] bool open();

    /**
     * @brief Closes the underlying SDK client.
     */
    void close() noexcept;

    /**
     * @brief Returns true if the SDK client is open.
     *
     * @return true if open.
     */
    [[nodiscard]] bool is_open() const noexcept;

    /**
     * @brief Backward-compatible alias for is_open().
     *
     * @return true if open.
     */
    [[nodiscard]] bool opened() const noexcept;

    /**
     * @brief Stores a text value.
     *
     * @param key Cnerium store key.
     * @param value Text value.
     * @return true if the write was submitted.
     */
    [[nodiscard]] bool put(
        const store::StoreKey &key,
        std::string_view value);

    /**
     * @brief Stores a text value.
     *
     * @param key Raw key.
     * @param value Text value.
     * @return true if the write was submitted.
     */
    [[nodiscard]] bool put(
        std::string_view key,
        std::string_view value);

    /**
     * @brief Reads a text value.
     *
     * @param key Cnerium store key.
     * @return Stored value, or std::nullopt when missing or unavailable.
     */
    [[nodiscard]] std::optional<std::string> get(
        const store::StoreKey &key) const;

    /**
     * @brief Reads a text value.
     *
     * @param key Raw key.
     * @return Stored value, or std::nullopt when missing or unavailable.
     */
    [[nodiscard]] std::optional<std::string> get(
        std::string_view key) const;

    /**
     * @brief Returns true if a key exists.
     *
     * @param key Cnerium store key.
     * @return true if the key exists.
     */
    [[nodiscard]] bool contains(const store::StoreKey &key) const;

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
     * @return true if the remove operation was submitted.
     */
    [[nodiscard]] bool remove(const store::StoreKey &key);

    /**
     * @brief Removes a key.
     *
     * @param key Raw key.
     * @return true if the remove operation was submitted.
     */
    [[nodiscard]] bool remove(std::string_view key);

    /**
     * @brief Stores a replayable durable response.
     *
     * @param key Cnerium store key.
     * @param response Stored response.
     * @return true if the response was stored.
     */
    [[nodiscard]] bool put_response(
        const store::StoreKey &key,
        const store::StoredResponse &response);

    /**
     * @brief Reads a replayable durable response.
     *
     * @param key Cnerium store key.
     * @return Stored response, or std::nullopt when missing or invalid.
     */
    [[nodiscard]] std::optional<store::StoredResponse> get_response(
        const store::StoreKey &key) const;

    /**
     * @brief Advances the Softadastra SDK sync pipeline once.
     *
     * @param prune_completed Whether completed entries should be pruned.
     * @return true if the tick was submitted.
     */
    [[nodiscard]] bool tick(bool prune_completed = true);

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
     * @brief Returns the underlying public Softadastra SDK client.
     *
     * @return SDK client.
     */
    [[nodiscard]] softadastra::sdk::Client &client() noexcept;

    /**
     * @brief Returns the underlying public Softadastra SDK client.
     *
     * @return SDK client.
     */
    [[nodiscard]] const softadastra::sdk::Client &client() const noexcept;

  private:
    softadastra::sdk::Client client_;
  };

} // namespace cnerium::adapters

#endif // CNERIUM_ADAPTERS_SOFTADASTRA_STORE_HPP
