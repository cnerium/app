/**
 *
 *  @file VixWebSocket.cpp
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

#include <cnerium/adapters/VixWebSocket.hpp>

#include <cstdint>
#include <utility>

namespace cnerium::adapters
{
  VixWebSocket::~VixWebSocket()
  {
    stop();
  }

  VixWebSocket::VixWebSocket(VixWebSocket &&other) noexcept
      : executor_(std::move(other.executor_)),
        server_(std::move(other.server_)),
        running_(other.running_)
  {
    other.running_ = false;
  }

  VixWebSocket &VixWebSocket::operator=(VixWebSocket &&other) noexcept
  {
    if (this != &other)
    {
      stop();

      executor_ = std::move(other.executor_);
      server_ = std::move(other.server_);
      running_ = other.running_;

      other.running_ = false;
    }

    return *this;
  }

  bool VixWebSocket::start(
      const realtime::RealtimeConfig &config,
      vix::config::Config &core_config,
      std::shared_ptr<vix::executor::RuntimeExecutor> executor)
  {
    if (!config.is_enabled())
    {
      return true;
    }

    if (!config.is_valid() || !executor)
    {
      return false;
    }

    if (running_)
    {
      return true;
    }

    executor_ = std::move(executor);

    core_config.set("websocket.host", config.host());
    core_config.set("websocket.port", static_cast<int>(config.port()));

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

  void VixWebSocket::stop() noexcept
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

  bool VixWebSocket::emit(const realtime::Event &event)
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

  bool VixWebSocket::emit_to(
      const std::string &room,
      const realtime::Event &event)
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

  bool VixWebSocket::is_running() const noexcept
  {
    return running_;
  }

  vix::websocket::Server *VixWebSocket::server() noexcept
  {
    return server_.get();
  }

  const vix::websocket::Server *VixWebSocket::server() const noexcept
  {
    return server_.get();
  }

  vix::json::kvs VixWebSocket::to_vix_payload(
      const realtime::EventPayload &payload)
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

} // namespace cnerium::adapters
