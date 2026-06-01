/**
 *
 *  @file Idempotency.cpp
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

#include <cnerium/reliability/Idempotency.hpp>

namespace cnerium::reliability
{
  Idempotency::Idempotency(store::Store &store) noexcept
      : replay_(store)
  {
  }

  DurableResult Idempotency::check(
      std::string_view operation,
      const IdempotencyKey &key,
      std::string_view body)
  {
    return check_hash(
        operation,
        key,
        hash_body(body));
  }

  DurableResult Idempotency::check_hash(
      std::string_view operation,
      const IdempotencyKey &key,
      const RequestHash &hash)
  {
    return replay_.check(
        operation,
        key,
        hash);
  }

  bool Idempotency::commit(
      std::string_view operation,
      const IdempotencyKey &key,
      std::string_view body,
      const store::StoredResponse &response)
  {
    return commit_hash(
        operation,
        key,
        hash_body(body),
        response);
  }

  bool Idempotency::commit_hash(
      std::string_view operation,
      const IdempotencyKey &key,
      const RequestHash &hash,
      const store::StoredResponse &response)
  {
    return replay_.commit(
        operation,
        key,
        hash,
        response);
  }

  RequestHash Idempotency::hash_body(std::string_view body)
  {
    return RequestHash::from_body(body);
  }

} // namespace cnerium::reliability
