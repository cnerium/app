/**
 *
 *  @file RealtimeConfig.cpp
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

#include <cnerium/realtime/RealtimeConfig.hpp>

#include <utility>

namespace cnerium::realtime
{
  RealtimeConfig::RealtimeConfig(
      bool enabled,
      std::string endpoint,
      std::string host,
      std::uint16_t port)
      : enabled_(enabled),
        endpoint_(std::move(endpoint)),
        host_(std::move(host)),
        port_(port)
  {
  }

  RealtimeConfig RealtimeConfig::enabled(
      std::string endpoint,
      std::string host,
      std::uint16_t port)
  {
    return RealtimeConfig{
        true,
        std::move(endpoint),
        std::move(host),
        port};
  }

  RealtimeConfig RealtimeConfig::disabled()
  {
    return RealtimeConfig{};
  }

  bool RealtimeConfig::is_enabled() const noexcept
  {
    return enabled_;
  }

  bool RealtimeConfig::enabled() const noexcept
  {
    return is_enabled();
  }

  const std::string &RealtimeConfig::endpoint() const noexcept
  {
    return endpoint_;
  }

  const std::string &RealtimeConfig::host() const noexcept
  {
    return host_;
  }

  std::uint16_t RealtimeConfig::port() const noexcept
  {
    return port_;
  }

  void RealtimeConfig::set_enabled(bool value) noexcept
  {
    enabled_ = value;
  }

  void RealtimeConfig::set_endpoint(std::string value)
  {
    endpoint_ = std::move(value);
  }

  void RealtimeConfig::set_host(std::string value)
  {
    host_ = std::move(value);
  }

  void RealtimeConfig::set_port(std::uint16_t value) noexcept
  {
    port_ = value;
  }

  bool RealtimeConfig::has_endpoint() const noexcept
  {
    return !endpoint_.empty();
  }

  bool RealtimeConfig::has_host() const noexcept
  {
    return !host_.empty();
  }

  bool RealtimeConfig::has_port() const noexcept
  {
    return port_ != 0;
  }

  bool RealtimeConfig::is_valid() const noexcept
  {
    if (!enabled_)
    {
      return true;
    }

    return has_endpoint() &&
           has_host() &&
           has_port();
  }

  bool RealtimeConfig::valid() const noexcept
  {
    return is_valid();
  }

  void RealtimeConfig::clear() noexcept
  {
    enabled_ = false;
    endpoint_.clear();
    host_.clear();
    port_ = 0;
  }

} // namespace cnerium::realtime
