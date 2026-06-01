/**
 *
 *  @file StoreKey.cpp
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

#include <cnerium/store/StoreKey.hpp>
#include <utility>
#include <string_view>
#include <string>

namespace cnerium::store
{
  namespace
  {
    constexpr std::string_view CNERIUM_PREFIX = "cnerium:";

    [[nodiscard]] std::string build_key(
        std::string_view category,
        std::string_view operation,
        std::string_view key)
    {
      std::string output;
      output.reserve(
          CNERIUM_PREFIX.size() +
          category.size() +
          1U +
          operation.size() +
          1U +
          key.size());

      output.append(CNERIUM_PREFIX);
      output.append(category);
      output.push_back(':');
      output.append(operation);
      output.push_back(':');
      output.append(key);

      return output;
    }
  } // namespace

  StoreKey::StoreKey(std::string value)
      : value_(std::move(value))
  {
  }

  StoreKey::StoreKey(std::string_view value)
      : value_(value)
  {
  }

  StoreKey::StoreKey(const char *value)
      : value_(value == nullptr ? "" : value)
  {
  }

  StoreKey StoreKey::from(std::string value)
  {
    return StoreKey{std::move(value)};
  }

  StoreKey StoreKey::idempotency(
      std::string_view operation,
      std::string_view key)
  {
    return make_operation_key(
        "idempotency",
        operation,
        key);
  }

  StoreKey StoreKey::request_hash(
      std::string_view operation,
      std::string_view key)
  {
    return make_operation_key(
        "request_hash",
        operation,
        key);
  }

  StoreKey StoreKey::response(
      std::string_view operation,
      std::string_view key)
  {
    return make_operation_key(
        "response",
        operation,
        key);
  }

  StoreKey StoreKey::event(std::string_view event_id)
  {
    std::string output;
    output.reserve(
        CNERIUM_PREFIX.size() +
        std::string_view{"event"}.size() +
        1U +
        event_id.size());

    output.append(CNERIUM_PREFIX);
    output.append("event");
    output.push_back(':');
    output.append(event_id);

    return StoreKey{std::move(output)};
  }

  const std::string &StoreKey::str() const noexcept
  {
    return value_;
  }

  const std::string &StoreKey::value() const noexcept
  {
    return value_;
  }

  bool StoreKey::empty() const noexcept
  {
    return value_.empty();
  }

  bool StoreKey::is_valid() const noexcept
  {
    return !value_.empty();
  }

  bool StoreKey::valid() const noexcept
  {
    return is_valid();
  }

  void StoreKey::clear() noexcept
  {
    value_.clear();
  }

  StoreKey StoreKey::make_operation_key(
      std::string_view category,
      std::string_view operation,
      std::string_view key)
  {
    if (category.empty() || operation.empty() || key.empty())
    {
      return StoreKey{};
    }

    return StoreKey{
        build_key(
            category,
            operation,
            key)};
  }

} // namespace cnerium::store
