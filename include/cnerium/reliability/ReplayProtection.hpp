/**
 *
 *  @file ReplayProtection.hpp
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

#ifndef CNERIUM_RELIABILITY_REPLAY_PROTECTION_HPP
#define CNERIUM_RELIABILITY_REPLAY_PROTECTION_HPP

#include <string_view>

#include <cnerium/reliability/DurableResult.hpp>
#include <cnerium/reliability/IdempotencyKey.hpp>
#include <cnerium/reliability/RequestHash.hpp>
#include <cnerium/store/Store.hpp>
#include <cnerium/store/StoredResponse.hpp>

namespace cnerium::reliability
{
  /**
   * @brief Protects durable routes against unsafe request replay.
   *
   * ReplayProtection checks whether an incoming durable request was already
   * processed before.
   *
   * Rules:
   * - missing or invalid idempotency key returns DurableResult::Invalid
   * - new idempotency key returns DurableResult::Execute
   * - same idempotency key with same request hash returns DurableResult::Replay
   * - same idempotency key with different request hash returns DurableResult::Conflict
   */
  class ReplayProtection
  {
  public:
    /**
     * @brief Creates replay protection using a Cnerium store.
     *
     * @param store Store used to read and write idempotency metadata.
     */
    explicit ReplayProtection(store::Store &store) noexcept;

    ReplayProtection(const ReplayProtection &) = delete;
    ReplayProtection &operator=(const ReplayProtection &) = delete;

    ReplayProtection(ReplayProtection &&) noexcept = delete;
    ReplayProtection &operator=(ReplayProtection &&) noexcept = delete;

    /**
     * @brief Checks an incoming durable request.
     *
     * @param operation Durable operation name.
     * @param key Idempotency key.
     * @param hash Request body hash.
     * @return Durable result describing the selected action.
     */
    [[nodiscard]] DurableResult check(
        std::string_view operation,
        const IdempotencyKey &key,
        const RequestHash &hash);

    /**
     * @brief Commits a successful durable response.
     *
     * The request hash and response are saved in the store so later retries can
     * return the exact same response without executing the handler again.
     *
     * @param operation Durable operation name.
     * @param key Idempotency key.
     * @param hash Request body hash.
     * @param response Response to store.
     * @return true if all required records were stored.
     */
    [[nodiscard]] bool commit(
        std::string_view operation,
        const IdempotencyKey &key,
        const RequestHash &hash,
        const store::StoredResponse &response);

  private:
    store::Store *store_{nullptr};
  };

} // namespace cnerium::reliability

#endif // CNERIUM_RELIABILITY_REPLAY_PROTECTION_HPP
