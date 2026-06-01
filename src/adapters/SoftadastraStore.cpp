/**
 *
 *  @file SoftadastraStore.cpp
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

#include <cnerium/adapters/SoftadastraStore.hpp>

#include <softadastra/sdk/Key.hpp>
#include <softadastra/sdk/Value.hpp>

#include <utility>

namespace cnerium::adapters
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

  SoftadastraStore::SoftadastraStore()
      : client_()
  {
  }

  SoftadastraStore::SoftadastraStore(softadastra::sdk::Client client)
      : client_(std::move(client))
  {
  }

  SoftadastraStore::~SoftadastraStore()
  {
    close();
  }

  SoftadastraStore::SoftadastraStore(SoftadastraStore &&other) noexcept
      : client_(std::move(other.client_))
  {
  }

  SoftadastraStore &SoftadastraStore::operator=(SoftadastraStore &&other) noexcept
  {
    if (this != &other)
    {
      close();
      client_ = std::move(other.client_);
    }

    return *this;
  }

  bool SoftadastraStore::open()
  {
    if (client_.is_open())
    {
      return true;
    }

    (void)client_.open();
    return client_.is_open();
  }

  void SoftadastraStore::close() noexcept
  {
    client_.close();
  }

  bool SoftadastraStore::is_open() const noexcept
  {
    return client_.is_open();
  }

  bool SoftadastraStore::opened() const noexcept
  {
    return is_open();
  }

  bool SoftadastraStore::put(
      const store::StoreKey &key,
      std::string_view value)
  {
    if (!key.is_valid())
    {
      return false;
    }

    return put(key.value(), value);
  }

  bool SoftadastraStore::put(
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

  std::optional<std::string> SoftadastraStore::get(
      const store::StoreKey &key) const
  {
    if (!key.is_valid())
    {
      return std::nullopt;
    }

    return get(key.value());
  }

  std::optional<std::string> SoftadastraStore::get(
      std::string_view key) const
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

  bool SoftadastraStore::contains(const store::StoreKey &key) const
  {
    if (!key.is_valid())
    {
      return false;
    }

    return contains(key.value());
  }

  bool SoftadastraStore::contains(std::string_view key) const
  {
    if (key.empty() || !client_.is_open())
    {
      return false;
    }

    return client_.contains(std::string{key});
  }

  bool SoftadastraStore::remove(const store::StoreKey &key)
  {
    if (!key.is_valid())
    {
      return false;
    }

    return remove(key.value());
  }

  bool SoftadastraStore::remove(std::string_view key)
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

  bool SoftadastraStore::put_response(
      const store::StoreKey &key,
      const store::StoredResponse &response)
  {
    if (!key.is_valid() || !response.is_valid())
    {
      return false;
    }

    return put(key, response.to_json_text());
  }

  std::optional<store::StoredResponse> SoftadastraStore::get_response(
      const store::StoreKey &key) const
  {
    auto value = get(key);

    if (!value)
    {
      return std::nullopt;
    }

    try
    {
      return store::StoredResponse::from_json_text(*value);
    }
    catch (...)
    {
      return std::nullopt;
    }
  }

  bool SoftadastraStore::tick(bool prune_completed)
  {
    if (!client_.is_open() && !open())
    {
      return false;
    }

    (void)client_.tick(prune_completed);
    return true;
  }

  std::size_t SoftadastraStore::size() const noexcept
  {
    return client_.size();
  }

  bool SoftadastraStore::empty() const noexcept
  {
    return client_.empty();
  }

  softadastra::sdk::Client &SoftadastraStore::client() noexcept
  {
    return client_;
  }

  const softadastra::sdk::Client &SoftadastraStore::client() const noexcept
  {
    return client_;
  }

} // namespace cnerium::adapters
