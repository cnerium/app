/**
 *
 *  @file AppRuntime.cpp
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

#include <cnerium/app/AppRuntime.hpp>

#include <utility>

namespace cnerium::app
{
  AppRuntime::AppRuntime()
      : AppRuntime(AppConfig::development())
  {
  }

  AppRuntime::AppRuntime(AppConfig config)
      : config_(std::move(config)),
        vix_config_(config_.vix_config_path()),
        executor_(make_executor()),
        store_(make_client()),
        websocket_(),
        running_(false)
  {
  }

  AppRuntime::~AppRuntime()
  {
    stop();
  }

  AppRuntime::AppRuntime(AppRuntime &&other) noexcept
      : config_(std::move(other.config_)),
        vix_config_(std::move(other.vix_config_)),
        executor_(std::move(other.executor_)),
        store_(std::move(other.store_)),
        websocket_(std::move(other.websocket_)),
        running_(other.running_)
  {
    other.running_ = false;
  }

  AppRuntime &AppRuntime::operator=(AppRuntime &&other) noexcept
  {
    if (this != &other)
    {
      stop();

      config_ = std::move(other.config_);
      vix_config_ = std::move(other.vix_config_);
      executor_ = std::move(other.executor_);
      store_ = std::move(other.store_);
      websocket_ = std::move(other.websocket_);
      running_ = other.running_;

      other.running_ = false;
    }

    return *this;
  }

  bool AppRuntime::start()
  {
    if (running_)
    {
      return true;
    }

    if (!config_.is_valid())
    {
      return false;
    }

    if (!executor_)
    {
      executor_ = make_executor();
    }

    if (!store_.open())
    {
      return false;
    }

    if (!websocket_.start(
            config_.realtime(),
            vix_config_,
            executor_))
    {
      store_.close();
      return false;
    }

    running_ = true;
    return true;
  }

  void AppRuntime::stop() noexcept
  {
    if (!running_)
    {
      websocket_.stop();
      store_.close();
      return;
    }

    websocket_.stop();
    store_.close();

    running_ = false;
  }

  bool AppRuntime::is_running() const noexcept
  {
    return running_;
  }

  const AppConfig &AppRuntime::config() const noexcept
  {
    return config_;
  }

  AppConfig &AppRuntime::config() noexcept
  {
    return config_;
  }

  vix::config::Config &AppRuntime::vix_config() noexcept
  {
    return vix_config_;
  }

  const vix::config::Config &AppRuntime::vix_config() const noexcept
  {
    return vix_config_;
  }

  std::shared_ptr<vix::executor::RuntimeExecutor> AppRuntime::executor() noexcept
  {
    return executor_;
  }

  store::Store &AppRuntime::store() noexcept
  {
    return store_;
  }

  const store::Store &AppRuntime::store() const noexcept
  {
    return store_;
  }

  adapters::VixWebSocket &AppRuntime::websocket() noexcept
  {
    return websocket_;
  }

  const adapters::VixWebSocket &AppRuntime::websocket() const noexcept
  {
    return websocket_;
  }

  bool AppRuntime::realtime_enabled() const noexcept
  {
    return config_.realtime_enabled();
  }

  bool AppRuntime::emit(const realtime::Event &event)
  {
    return websocket_.emit(event);
  }

  bool AppRuntime::emit_to(
      const std::string &room,
      const realtime::Event &event)
  {
    return websocket_.emit_to(room, event);
  }

  softadastra::sdk::Client AppRuntime::make_client() const
  {
    auto options =
        softadastra::sdk::ClientOptions::durable(
            config_.node_id(),
            config_.wal_path());

    return softadastra::sdk::Client{std::move(options)};
  }

  std::shared_ptr<vix::executor::RuntimeExecutor> AppRuntime::make_executor()
  {
    return std::make_shared<vix::executor::RuntimeExecutor>();
  }

} // namespace cnerium::app
