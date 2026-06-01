/**
 *
 *  @file RequestHash.hpp
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

#ifndef CNERIUM_RELIABILITY_REQUEST_HASH_HPP
#define CNERIUM_RELIABILITY_REQUEST_HASH_HPP

#include <cstdint>
#include <string>
#include <string_view>

namespace cnerium::reliability
{
  /**
   * @brief Stable hash of a durable HTTP request body.
   *
   * RequestHash is used by Cnerium idempotency to detect whether a repeated
   * request uses the same Idempotency-Key with the same body.
   *
   * The hash is deterministic and does not rely on std::hash because std::hash
   * is not guaranteed to be stable across implementations.
   */
  class RequestHash
  {
  public:
    /**
     * @brief Creates an empty invalid request hash.
     */
    RequestHash() = default;

    /**
     * @brief Creates a request hash from a string.
     *
     * @param value Hash value.
     */
    explicit RequestHash(std::string value);

    /**
     * @brief Creates a request hash from a string view.
     *
     * @param value Hash value.
     */
    explicit RequestHash(std::string_view value);

    /**
     * @brief Creates a request hash from a C string.
     *
     * A null pointer is converted to an empty hash.
     *
     * @param value Hash value.
     */
    explicit RequestHash(const char *value);

    /**
     * @brief Creates a request hash from an existing hash string.
     *
     * @param value Hash value.
     * @return Request hash.
     */
    [[nodiscard]] static RequestHash from(std::string value);

    /**
     * @brief Computes a stable request body hash.
     *
     * @param body Request body.
     * @return Request hash.
     */
    [[nodiscard]] static RequestHash from_body(std::string_view body);

    /**
     * @brief Computes the raw FNV-1a 64-bit hash value.
     *
     * @param value Input value.
     * @return 64-bit hash.
     */
    [[nodiscard]] static std::uint64_t fnv1a64(std::string_view value) noexcept;

    /**
     * @brief Converts a 64-bit hash value to lowercase hexadecimal text.
     *
     * @param value Hash value.
     * @return Hexadecimal string.
     */
    [[nodiscard]] static std::string to_hex(std::uint64_t value);

    /**
     * @brief Returns the hash string.
     *
     * @return Hash value.
     */
    [[nodiscard]] const std::string &str() const noexcept;

    /**
     * @brief Returns the hash string.
     *
     * Alias for str().
     *
     * @return Hash value.
     */
    [[nodiscard]] const std::string &value() const noexcept;

    /**
     * @brief Returns true if the hash is empty.
     *
     * @return true if the hash is empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Returns true if the hash can be used by Cnerium.
     *
     * @return true if the hash is not empty.
     */
    [[nodiscard]] bool is_valid() const noexcept;

    /**
     * @brief Backward-compatible alias for is_valid().
     *
     * @return true if the hash is valid.
     */
    [[nodiscard]] bool valid() const noexcept;

    /**
     * @brief Clears the hash.
     */
    void clear() noexcept;

    /**
     * @brief Compares two request hashes for equality.
     *
     * @param left Left hash.
     * @param right Right hash.
     * @return true if both hashes contain the same value.
     */
    [[nodiscard]] friend bool operator==(
        const RequestHash &left,
        const RequestHash &right) noexcept
    {
      return left.value_ == right.value_;
    }

    /**
     * @brief Compares two request hashes for inequality.
     *
     * @param left Left hash.
     * @param right Right hash.
     * @return true if hashes are different.
     */
    [[nodiscard]] friend bool operator!=(
        const RequestHash &left,
        const RequestHash &right) noexcept
    {
      return !(left == right);
    }

    /**
     * @brief Orders request hashes lexicographically.
     *
     * @param left Left hash.
     * @param right Right hash.
     * @return true if left is lexicographically smaller than right.
     */
    [[nodiscard]] friend bool operator<(
        const RequestHash &left,
        const RequestHash &right) noexcept
    {
      return left.value_ < right.value_;
    }

  private:
    std::string value_{};
  };

} // namespace cnerium::reliability

#endif // CNERIUM_RELIABILITY_REQUEST_HASH_HPP
