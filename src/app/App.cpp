/**
 *
 *  @file App.cpp
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

#include <cnerium/app/App.hpp>
#include <cnerium/adapters/VixHttp.hpp>

#include <utility>

namespace cnerium::app
{
  App::App()
      : App(AppConfig::development())
  {
  }

  App::App(AppConfig config)
      : app_(config.vix_config_path()),
        runtime_(std::move(config))
  {
  }

  App::~App()
  {
    stop();
  }

  App::App(App &&other) noexcept
      : app_(std::move(other.app_)),
        runtime_(std::move(other.runtime_))
  {
  }

  App &App::operator=(App &&other) noexcept
  {
    if (this != &other)
    {
      stop();

      app_ = std::move(other.app_);
      runtime_ = std::move(other.runtime_);
    }

    return *this;
  }

  App &App::durable_post(
      std::string path,
      std::string operation,
      http::DurableHandler handler)
  {
    auto route =
        std::make_shared<http::DurableRoute>(
            std::move(operation),
            runtime_.store(),
            std::move(handler));

    app_.post(
        std::move(path),
        [route](const vix::http::Request &request,
                vix::http::ResponseWrapper &response)
        {
          adapters::VixHttp::execute_route(
              *route,
              request,
              response);
        });

    return *this;
  }

  App &App::realtime(
      std::string endpoint,
      std::string host,
      std::uint16_t port)
  {
    runtime_.config().enable_realtime(
        std::move(endpoint),
        std::move(host),
        port);

    return *this;
  }

  bool App::emit(const realtime::Event &event)
  {
    return runtime_.emit(event);
  }

  bool App::emit(
      std::string type,
      realtime::EventPayload payload)
  {
    return emit(
        realtime::Event{
            std::move(type),
            std::move(payload)});
  }

  bool App::emit_to(
      const std::string &room,
      const realtime::Event &event)
  {
    return runtime_.emit_to(room, event);
  }

  bool App::emit_to(
      const std::string &room,
      std::string type,
      realtime::EventPayload payload)
  {
    return emit_to(
        room,
        realtime::Event{
            std::move(type),
            std::move(payload)});
  }

  int App::run()
  {
    if (!start())
    {
      return 1;
    }

    return app_.run();
  }

  bool App::start()
  {
    return runtime_.start();
  }

  void App::stop() noexcept
  {
    try
    {
      app_.close();
    }
    catch (...)
    {
    }

    runtime_.stop();
  }

  bool App::is_running() const noexcept
  {
    return runtime_.is_running();
  }

  AppRuntime &App::runtime() noexcept
  {
    return runtime_;
  }

  const AppRuntime &App::runtime() const noexcept
  {
    return runtime_;
  }

  vix::App &App::vix_app() noexcept
  {
    return app_;
  }

  const vix::App &App::vix_app() const noexcept
  {
    return app_;
  }

  const AppConfig &App::config() const noexcept
  {
    return runtime_.config();
  }

  AppConfig &App::config() noexcept
  {
    return runtime_.config();
  }

} // namespace cnerium::app
