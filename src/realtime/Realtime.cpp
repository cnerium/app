/**
 *
 *  @file Realtime.cpp
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

#include <cnerium/realtime/Realtime.hpp>

#include <utility>

namespace cnerium::realtime
{
  namespace
  {
    [[nodiscard]] vix::json::kvs to_vix_payload(const EventPayload &payload)
    {
      vix::json::kvs output{};

      if (!payload.is_object())
      {
        output.set("value", vix::json::token(payload.dump()));
        return output;
      }

      for (auto iterator = payload.begin(); iterator != payload.end(); ++iterator)
      {
        const auto &key = iterator.key();
        const auto &value = iterator.value();

        if (value.is_boolean())
        {
          output.set(key, vix::json::token(value.get<bool>()));
        }
        else if (value.is_number_integer())
        {
          output.set(key, vix::json::token(value.get<std::int64_t>()));
        }
        else if (value.is_number_float())
        {
          output.set(key, vix::json::token(value.get<double>()));
        }
        else if (value.is_string())
        {
          output.set(key, vix::json::token(value.get<std::string>()));
        }
        else if (value.is_null())
        {
          output.set(key, vix::json::token(nullptr));
        }
        else
        {
          output.set(key, vix::json::token(value.dump()));
        }
      }

      return output;
    }
  } // namespace

  Realtime::Realtime()
      : config_(RealtimeConfig::disabled())
  {
  }

  Realtime::Realtime(RealtimeConfig config)
      : config_(std::move(config))
  {
  }

  Realtime::~Realtime()
  {
    stop();
  }

  Realtime::Realtime(Realtime &&other) noexcept
      : config_(std::move(other.config_)),
        executor_(std::move(other.executor_)),
        server_(std::move(other.server_)),
        running_(other.running_)
  {
    other.running_ = false;
  }

  Realtime &Realtime::operator=(Realtime &&other) noexcept
  {
    if (this != &other)
    {
      stop();

      config_ = std::move(other.config_);
      executor_ = std::move(other.executor_);
      server_ = std::move(other.server_);
      running_ = other.running_;

      other.running_ = false;
    }

    return *this;
  }

  void Realtime::configure(RealtimeConfig config)
  {
    if (running_)
    {
      stop();
    }

    config_ = std::move(config);
  }

  void Realtime::enable(
      std::string endpoint,
      std::string host,
      std::uint16_t port)
  {
    configure(
        RealtimeConfig::enabled(
            std::move(endpoint),
            std::move(host),
            port));
  }

  void Realtime::disable() noexcept
  {
    stop();
    config_ = RealtimeConfig::disabled();
  }

  bool Realtime::start(
      vix::config::Config &core_config,
      std::shared_ptr<vix::executor::RuntimeExecutor> executor)
  {
    if (!config_.is_enabled())
    {
      return true;
    }

    if (!config_.is_valid() || !executor)
    {
      return false;
    }

    if (running_)
    {
      return true;
    }

    executor_ = std::move(executor);

    core_config.set("websocket.host", config_.host());
    core_config.set("websocket.port", static_cast<int>(config_.port()));

    try
    {
      server_ = std::make_unique<vix::websocket::Server>(
          core_config,
          executor_);

      server_->start();
      running_ = true;
      return true;
    }
    catch (...)
    {
      server_.reset();
      executor_.reset();
      running_ = false;
      return false;
    }
  }

  void Realtime::stop() noexcept
  {
    if (!running_ && !server_)
    {
      return;
    }

    try
    {
      if (server_)
      {
        server_->stop();
      }
    }
    catch (...)
    {
    }

    server_.reset();
    executor_.reset();
    running_ = false;
  }

  bool Realtime::emit(const Event &event)
  {
    if (!running_ || !server_ || !event.is_valid())
    {
      return false;
    }

    try
    {
      server_->broadcast_json(
          event.type(),
          to_vix_payload(event.payload()));

      return true;
    }
    catch (...)
    {
      return false;
    }
  }

  bool Realtime::emit(
      std::string type,
      EventPayload payload)
  {
    return emit(
        Event{
            std::move(type),
            std::move(payload)});
  }

  bool Realtime::emit_to(
      const std::string &room,
      const Event &event)
  {
    if (!running_ || !server_ || room.empty() || !event.is_valid())
    {
      return false;
    }

    try
    {
      server_->broadcast_room_json(
          room,
          event.type(),
          to_vix_payload(event.payload()));

      return true;
    }
    catch (...)
    {
      return false;
    }
  }

  bool Realtime::emit_to(
      const std::string &room,
      std::string type,
      EventPayload payload)
  {
    return emit_to(
        room,
        Event{
            std::move(type),
            std::move(payload)});
  }

  bool Realtime::is_enabled() const noexcept
  {
    return config_.is_enabled();
  }

  bool Realtime::is_running() const noexcept
  {
    return running_;
  }

  const RealtimeConfig &Realtime::config() const noexcept
  {
    return config_;
  }

  vix::websocket::Server *Realtime::server() noexcept
  {
    return server_.get();
  }

  const vix::websocket::Server *Realtime::server() const noexcept
  {
    return server_.get();
  }

} // namespace cnerium::realtime
