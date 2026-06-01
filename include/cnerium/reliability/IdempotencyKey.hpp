/**
 *
 *  @file IdempotencyKey.hpp
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

#ifndef CNERIUM_RELIABILITY_IDEMPOTENCY_KEY_HPP
#define CNERIUM_RELIABILITY_IDEMPOTENCY_KEY_HPP

#include <string>
#include <string_view>

namespace cnerium::reliability
{
  /**
   * @brief Represents an idempotency key used by durable Cnerium routes.
   *
   * IdempotencyKey is extracted from the HTTP `Idempotency-Key` header.
   *
   * It allows Cnerium to detect repeated requests and return the previously
   * stored response instead of executing the same operation again.
   */
  class IdempotencyKey
  {
  public:
    /**
     * @brief Creates an empty invalid idempotency key.
     */
    IdempotencyKey() = default;

    /**
     * @brief Creates an idempotency key from a string.
     *
     * @param value Key value.
     */
    explicit IdempotencyKey(std::string value);

    /**
     * @brief Creates an idempotency key from a string view.
     *
     * @param value Key value.
     */
    explicit IdempotencyKey(std::string_view value);

    /**
     * @brief Creates an idempotency key from a C string.
     *
     * A null pointer is converted to an empty key.
     *
     * @param value Key value.
     */
    explicit IdempotencyKey(const char *value);

    /**
     * @brief Creates an idempotency key from a string.
     *
     * @param value Key value.
     * @return Idempotency key.
     */
    [[nodiscard]] static IdempotencyKey from(std::string value);

    /**
     * @brief Creates an idempotency key from a string view.
     *
     * @param value Key value.
     * @return Idempotency key.
     */
    [[nodiscard]] static IdempotencyKey from_view(std::string_view value);

    /**
     * @brief Returns the key string.
     *
     * @return Key value.
     */
    [[nodiscard]] const std::string &str() const noexcept;

    /**
     * @brief Returns the key string.
     *
     * Alias for str().
     *
     * @return Key value.
     */
    [[nodiscard]] const std::string &value() const noexcept;

    /**
     * @brief Returns true if the key is empty.
     *
     * @return true if the key is empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Returns true if the key can be used by Cnerium.
     *
     * A valid idempotency key must not be empty.
     *
     * @return true if the key is valid.
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
     * @brief Compares two idempotency keys for equality.
     *
     * @param left Left key.
     * @param right Right key.
     * @return true if both keys contain the same value.
     */
    [[nodiscard]] friend bool operator==(
        const IdempotencyKey &left,
        const IdempotencyKey &right) noexcept
    {
      return left.value_ == right.value_;
    }

    /**
     * @brief Compares two idempotency keys for inequality.
     *
     * @param left Left key.
     * @param right Right key.
     * @return true if keys are different.
     */
    [[nodiscard]] friend bool operator!=(
        const IdempotencyKey &left,
        const IdempotencyKey &right) noexcept
    {
      return !(left == right);
    }

    /**
     * @brief Orders idempotency keys lexicographically.
     *
     * @param left Left key.
     * @param right Right key.
     * @return true if left is lexicographically smaller than right.
     */
    [[nodiscard]] friend bool operator<(
        const IdempotencyKey &left,
        const IdempotencyKey &right) noexcept
    {
      return left.value_ < right.value_;
    }

  private:
    std::string value_{};
  };

} // namespace cnerium::reliability

#endif // CNERIUM_RELIABILITY_IDEMPOTENCY_KEY_HPP
