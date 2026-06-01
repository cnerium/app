/**
 *
 *  @file Store.cpp
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

#include <cnerium/store/Store.hpp>

#include <softadastra/sdk/Key.hpp>
#include <softadastra/sdk/Value.hpp>

#include <utility>

namespace cnerium::store
{
  namespace
  {
    [[nodiscard]] softadastra::sdk::Key make_sdk_key(std::string_view value)
    {
      return softadastra::sdk::Key{value};
    }

    [[nodiscard]] softadastra::sdk::Value make_sdk_value(std::string_view value)
    {
      return softadastra::sdk::Value::from_string(value);
    }
  } // namespace

  Store::Store()
      : client_()
  {
  }

  Store::Store(softadastra::sdk::Client client)
      : client_(std::move(client))
  {
  }

  Store::~Store()
  {
    close();
  }

  Store::Store(Store &&other) noexcept
      : client_(std::move(other.client_))
  {
  }

  Store &Store::operator=(Store &&other) noexcept
  {
    if (this != &other)
    {
      close();
      client_ = std::move(other.client_);
    }

    return *this;
  }

  bool Store::open()
  {
    if (client_.is_open())
    {
      return true;
    }

    (void)client_.open();
    return client_.is_open();
  }

  void Store::close() noexcept
  {
    client_.close();
  }

  bool Store::is_open() const noexcept
  {
    return client_.is_open();
  }

  bool Store::opened() const noexcept
  {
    return is_open();
  }

  bool Store::put(
      const StoreKey &key,
      std::string_view value)
  {
    if (!key.is_valid())
    {
      return false;
    }

    return put(key.value(), value);
  }

  bool Store::put(
      std::string_view key,
      std::string_view value)
  {
    if (key.empty())
    {
      return false;
    }

    if (!client_.is_open() && !open())
    {
      return false;
    }

    (void)client_.put(
        make_sdk_key(key),
        make_sdk_value(value));

    return true;
  }

  std::optional<std::string> Store::get(const StoreKey &key) const
  {
    if (!key.is_valid())
    {
      return std::nullopt;
    }

    return get(key.value());
  }

  std::optional<std::string> Store::get(std::string_view key) const
  {
    if (key.empty() || !client_.is_open())
    {
      return std::nullopt;
    }

    auto result = client_.get(std::string{key});

    if (!result)
    {
      return std::nullopt;
    }

    return result.value().to_string();
  }

  bool Store::contains(const StoreKey &key) const
  {
    if (!key.is_valid())
    {
      return false;
    }

    return contains(key.value());
  }

  bool Store::contains(std::string_view key) const
  {
    if (key.empty() || !client_.is_open())
    {
      return false;
    }

    return client_.contains(std::string{key});
  }

  bool Store::remove(const StoreKey &key)
  {
    if (!key.is_valid())
    {
      return false;
    }

    return remove(key.value());
  }

  bool Store::remove(std::string_view key)
  {
    if (key.empty())
    {
      return false;
    }

    if (!client_.is_open() && !open())
    {
      return false;
    }

    (void)client_.remove(std::string{key});
    return true;
  }

  bool Store::put_response(
      const StoreKey &key,
      const StoredResponse &response)
  {
    if (!key.is_valid() || !response.is_valid())
    {
      return false;
    }

    return put(key, response.to_json_text());
  }

  std::optional<StoredResponse> Store::get_response(
      const StoreKey &key) const
  {
    auto value = get(key);

    if (!value)
    {
      return std::nullopt;
    }

    try
    {
      return StoredResponse::from_json_text(*value);
    }
    catch (...)
    {
      return std::nullopt;
    }
  }

  std::size_t Store::size() const noexcept
  {
    return client_.size();
  }

  bool Store::empty() const noexcept
  {
    return client_.empty();
  }

  bool Store::tick(bool prune_completed)
  {
    if (!client_.is_open() && !open())
    {
      return false;
    }

    (void)client_.tick(prune_completed);
    return true;
  }

  softadastra::sdk::Client &Store::client() noexcept
  {
    return client_;
  }

  const softadastra::sdk::Client &Store::client() const noexcept
  {
    return client_;
  }

} // namespace cnerium::store
