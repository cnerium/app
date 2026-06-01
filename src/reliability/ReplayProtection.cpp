/**
 *
 *  @file ReplayProtection.cpp
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

#include <cnerium/reliability/ReplayProtection.hpp>
#include <cnerium/store/StoreKey.hpp>

namespace cnerium::reliability
{
  ReplayProtection::ReplayProtection(store::Store &store) noexcept
      : store_(&store)
  {
  }

  DurableResult ReplayProtection::check(
      std::string_view operation,
      const IdempotencyKey &key,
      const RequestHash &hash)
  {
    if (store_ == nullptr ||
        operation.empty() ||
        !key.is_valid() ||
        !hash.is_valid())
    {
      return DurableResult::invalid();
    }

    const auto hash_key =
        store::StoreKey::request_hash(operation, key.value());

    const auto response_key =
        store::StoreKey::response(operation, key.value());

    const auto stored_hash = store_->get(hash_key);

    if (!stored_hash)
    {
      return DurableResult::execute();
    }

    if (*stored_hash != hash.value())
    {
      return DurableResult::conflict();
    }

    auto stored_response = store_->get_response(response_key);

    if (!stored_response)
    {
      return DurableResult::execute();
    }

    return DurableResult::replay(*stored_response);
  }

  bool ReplayProtection::commit(
      std::string_view operation,
      const IdempotencyKey &key,
      const RequestHash &hash,
      const store::StoredResponse &response)
  {
    if (store_ == nullptr ||
        operation.empty() ||
        !key.is_valid() ||
        !hash.is_valid() ||
        !response.is_valid())
    {
      return false;
    }

    const auto hash_key =
        store::StoreKey::request_hash(operation, key.value());

    const auto response_key =
        store::StoreKey::response(operation, key.value());

    const bool hash_saved = store_->put(hash_key, hash.value());
    const bool response_saved = store_->put_response(response_key, response);

    return hash_saved && response_saved;
  }

} // namespace cnerium::reliability
