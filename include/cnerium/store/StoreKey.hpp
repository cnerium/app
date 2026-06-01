/**
 *
 *  @file StoreKey.hpp
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

#ifndef CNERIUM_STORE_STORE_KEY_HPP
#define CNERIUM_STORE_STORE_KEY_HPP

#include <string>
#include <string_view>

namespace cnerium::store
{
  /**
   * @brief Stable key used by Cnerium to store framework data.
   *
   * StoreKey is a small value object used to generate namespaced keys for the
   * Softadastra SDK store.
   *
   * Cnerium keeps its keys under the "cnerium:" namespace to avoid collisions
   * with application data.
   */
  class StoreKey
  {
  public:
    /**
     * @brief Creates an empty invalid store key.
     */
    StoreKey() = default;

    /**
     * @brief Creates a store key from a string.
     *
     * @param value Store key value.
     */
    explicit StoreKey(std::string value);

    /**
     * @brief Creates a store key from a string view.
     *
     * @param value Store key value.
     */
    explicit StoreKey(std::string_view value);

    /**
     * @brief Creates a store key from a C string.
     *
     * A null pointer is converted to an empty key.
     *
     * @param value Store key value.
     */
    explicit StoreKey(const char *value);

    /**
     * @brief Creates a store key from a string.
     *
     * @param value Store key value.
     * @return Store key.
     */
    [[nodiscard]] static StoreKey from(std::string value);

    /**
     * @brief Creates an idempotency key namespace entry.
     *
     * Format:
     * cnerium:idempotency:<operation>:<key>
     *
     * @param operation Operation name.
     * @param key Idempotency key.
     * @return Store key.
     */
    [[nodiscard]] static StoreKey idempotency(
        std::string_view operation,
        std::string_view key);

    /**
     * @brief Creates a request hash namespace entry.
     *
     * Format:
     * cnerium:request_hash:<operation>:<key>
     *
     * @param operation Operation name.
     * @param key Idempotency key.
     * @return Store key.
     */
    [[nodiscard]] static StoreKey request_hash(
        std::string_view operation,
        std::string_view key);

    /**
     * @brief Creates a stored response namespace entry.
     *
     * Format:
     * cnerium:response:<operation>:<key>
     *
     * @param operation Operation name.
     * @param key Idempotency key.
     * @return Store key.
     */
    [[nodiscard]] static StoreKey response(
        std::string_view operation,
        std::string_view key);

    /**
     * @brief Creates a realtime event namespace entry.
     *
     * Format:
     * cnerium:event:<event_id>
     *
     * @param event_id Event identifier.
     * @return Store key.
     */
    [[nodiscard]] static StoreKey event(std::string_view event_id);

    /**
     * @brief Returns the raw key string.
     *
     * @return Store key value.
     */
    [[nodiscard]] const std::string &str() const noexcept;

    /**
     * @brief Returns the raw key string.
     *
     * Alias for str().
     *
     * @return Store key value.
     */
    [[nodiscard]] const std::string &value() const noexcept;

    /**
     * @brief Returns true if the key is empty.
     *
     * @return true if the key is empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Returns true if the key can be used.
     *
     * @return true if the key is not empty.
     */
    [[nodiscard]] bool is_valid() const noexcept;

    /**
     * @brief Backward-compatible alias for is_valid().
     *
     * @return true if the key is valid.
     */
    [[nodiscard]] bool valid() const noexcept;

    /**
     * @brief Clears the key.
     */
    void clear() noexcept;

    /**
     * @brief Compares two store keys for equality.
     *
     * @param left Left key.
     * @param right Right key.
     * @return true if both keys contain the same value.
     */
    [[nodiscard]] friend bool operator==(
        const StoreKey &left,
        const StoreKey &right) noexcept
    {
      return left.value_ == right.value_;
    }

    /**
     * @brief Compares two store keys for inequality.
     *
     * @param left Left key.
     * @param right Right key.
     * @return true if keys are different.
     */
    [[nodiscard]] friend bool operator!=(
        const StoreKey &left,
        const StoreKey &right) noexcept
    {
      return !(left == right);
    }

    /**
     * @brief Orders store keys lexicographically.
     *
     * @param left Left key.
     * @param right Right key.
     * @return true if left is lexicographically smaller than right.
     */
    [[nodiscard]] friend bool operator<(
        const StoreKey &left,
        const StoreKey &right) noexcept
    {
      return left.value_ < right.value_;
    }

  private:
    /**
     * @brief Builds a namespaced Cnerium key.
     *
     * @param category Internal Cnerium category.
     * @param operation Operation name.
     * @param key User or generated key.
     * @return Store key.
     */
    [[nodiscard]] static StoreKey make_operation_key(
        std::string_view category,
        std::string_view operation,
        std::string_view key);

  private:
    std::string value_{};
  };

} // namespace cnerium::store

#endif // CNERIUM_STORE_STORE_KEY_HPP
