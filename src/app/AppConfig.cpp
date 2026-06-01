/**
 *
 *  @file AppConfig.cpp
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

#include <cnerium/app/AppConfig.hpp>

#include <utility>

namespace cnerium::app
{
  namespace
  {
    [[nodiscard]] std::string join_path(
        const std::string &left,
        const std::string &right)
    {
      if (left.empty())
      {
        return right;
      }

      if (right.empty())
      {
        return left;
      }

      if (left.back() == '/' || left.back() == '\\')
      {
        return left + right;
      }

      return left + "/" + right;
    }
  } // namespace

  AppConfig::AppConfig(
      std::string name,
      std::string data_dir,
      std::string node_id)
      : name_(std::move(name)),
        data_dir_(std::move(data_dir)),
        node_id_(std::move(node_id))
  {
  }

  AppConfig AppConfig::development()
  {
    AppConfig config{};
    config.set_name("cnerium-dev");
    config.set_data_dir("data/cnerium");
    config.set_node_id("cnerium-dev-node");
    config.set_vix_config_path("vix.json");
    config.enable_realtime("/ws", "0.0.0.0", 9090);
    return config;
  }

  AppConfig AppConfig::production()
  {
    AppConfig config{};
    config.set_name("cnerium");
    config.set_data_dir("data/cnerium");
    config.set_node_id("cnerium-node");
    config.set_vix_config_path("vix.json");
    config.disable_realtime();
    return config;
  }

  const std::string &AppConfig::name() const noexcept
  {
    return name_;
  }

  const std::string &AppConfig::data_dir() const noexcept
  {
    return data_dir_;
  }

  const std::string &AppConfig::node_id() const noexcept
  {
    return node_id_;
  }

  const std::string &AppConfig::vix_config_path() const noexcept
  {
    return vix_config_path_;
  }

  std::string AppConfig::store_path() const
  {
    return join_path(data_dir_, "store");
  }

  std::string AppConfig::wal_path() const
  {
    return join_path(data_dir_, "cnerium.wal");
  }

  const realtime::RealtimeConfig &AppConfig::realtime() const noexcept
  {
    return realtime_;
  }

  realtime::RealtimeConfig &AppConfig::realtime() noexcept
  {
    return realtime_;
  }

  bool AppConfig::realtime_enabled() const noexcept
  {
    return realtime_.is_enabled();
  }

  void AppConfig::set_name(std::string value)
  {
    name_ = std::move(value);
  }

  void AppConfig::set_data_dir(std::string value)
  {
    data_dir_ = std::move(value);
  }

  void AppConfig::set_node_id(std::string value)
  {
    node_id_ = std::move(value);
  }

  void AppConfig::set_vix_config_path(std::string value)
  {
    vix_config_path_ = std::move(value);
  }

  void AppConfig::set_realtime(realtime::RealtimeConfig value)
  {
    realtime_ = std::move(value);
  }

  void AppConfig::enable_realtime(
      std::string endpoint,
      std::string host,
      std::uint16_t port)
  {
    realtime_ =
        realtime::RealtimeConfig::enabled(
            std::move(endpoint),
            std::move(host),
            port);
  }

  void AppConfig::disable_realtime() noexcept
  {
    realtime_ = realtime::RealtimeConfig::disabled();
  }

  bool AppConfig::has_name() const noexcept
  {
    return !name_.empty();
  }

  bool AppConfig::has_data_dir() const noexcept
  {
    return !data_dir_.empty();
  }

  bool AppConfig::has_node_id() const noexcept
  {
    return !node_id_.empty();
  }

  bool AppConfig::has_vix_config_path() const noexcept
  {
    return !vix_config_path_.empty();
  }

  bool AppConfig::is_valid() const noexcept
  {
    return has_name() &&
           has_data_dir() &&
           has_node_id() &&
           has_vix_config_path() &&
           realtime_.is_valid();
  }

  bool AppConfig::valid() const noexcept
  {
    return is_valid();
  }

  void AppConfig::clear() noexcept
  {
    name_.clear();
    data_dir_.clear();
    node_id_.clear();
    vix_config_path_.clear();
    realtime_.clear();
  }

} // namespace cnerium::app
