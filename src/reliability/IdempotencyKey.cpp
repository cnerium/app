/**
 *
 *  @file IdempotencyKey.cpp
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

#include <cnerium/reliability/IdempotencyKey.hpp>

#include <utility>

namespace cnerium::reliability
{
  IdempotencyKey::IdempotencyKey(std::string value)
      : value_(std::move(value))
  {
  }

  IdempotencyKey::IdempotencyKey(std::string_view value)
      : value_(value)
  {
  }

  IdempotencyKey::IdempotencyKey(const char *value)
      : value_(value == nullptr ? "" : value)
  {
  }

  IdempotencyKey IdempotencyKey::from(std::string value)
  {
    return IdempotencyKey{std::move(value)};
  }

  IdempotencyKey IdempotencyKey::from_view(std::string_view value)
  {
    return IdempotencyKey{value};
  }

  const std::string &IdempotencyKey::str() const noexcept
  {
    return value_;
  }

  const std::string &IdempotencyKey::value() const noexcept
  {
    return value_;
  }

  bool IdempotencyKey::empty() const noexcept
  {
    return value_.empty();
  }

  bool IdempotencyKey::is_valid() const noexcept
  {
    return !value_.empty();
  }

  bool IdempotencyKey::valid() const noexcept
  {
    return is_valid();
  }

  void IdempotencyKey::clear() noexcept
  {
    value_.clear();
  }

} // namespace cnerium::reliability
