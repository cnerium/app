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

#include <memory>
#include <utility>

namespace cnerium::app
{
  AttachedApp::AttachedApp(vix::App &app)
      : AttachedApp(app, AppConfig::development())
  {
  }

  AttachedApp::AttachedApp(
      vix::App &app,
      AppConfig config)
      : app_(&app),
        runtime_(std::move(config)),
        durable_routes_()
  {
  }

  AttachedApp::~AttachedApp()
  {
    stop();
  }

  AttachedApp::AttachedApp(AttachedApp &&other) noexcept
      : app_(other.app_),
        runtime_(std::move(other.runtime_)),
        durable_routes_(std::move(other.durable_routes_))
  {
    other.app_ = nullptr;
  }

  AttachedApp &AttachedApp::operator=(AttachedApp &&other) noexcept
  {
    if (this != &other)
    {
      stop();

      app_ = other.app_;
      runtime_ = std::move(other.runtime_);
      durable_routes_ = std::move(other.durable_routes_);

      other.app_ = nullptr;
    }

    return *this;
  }

  AttachedApp &AttachedApp::durable_post(
      std::string path,
      std::string operation,
      http::DurableHandler handler)
  {
    if (!has_app())
    {
      return *this;
    }

    auto route =
        std::make_unique<http::DurableRoute>(
            std::move(operation),
            runtime_.store(),
            std::move(handler));

    auto *route_ptr = route.get();

    durable_routes_.push_back(std::move(route));

    app_->post(
        std::move(path),
        [route_ptr](const vix::http::Request &request,
                    vix::http::ResponseWrapper &response)
        {
          adapters::VixHttp::execute_route(
              *route_ptr,
              request,
              response);
        });

    return *this;
  }

  AttachedApp &AttachedApp::realtime(
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

  bool AttachedApp::emit(const realtime::Event &event)
  {
    return runtime_.emit(event);
  }

  bool AttachedApp::emit(
      std::string type,
      realtime::EventPayload payload)
  {
    return emit(
        realtime::Event{
            std::move(type),
            std::move(payload)});
  }

  bool AttachedApp::emit_to(
      const std::string &room,
      const realtime::Event &event)
  {
    return runtime_.emit_to(room, event);
  }

  bool AttachedApp::emit_to(
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

  bool AttachedApp::start()
  {
    return runtime_.start();
  }

  void AttachedApp::stop() noexcept
  {
    runtime_.stop();
  }

  bool AttachedApp::is_running() const noexcept
  {
    return runtime_.is_running();
  }

  AppRuntime &AttachedApp::runtime() noexcept
  {
    return runtime_;
  }

  const AppRuntime &AttachedApp::runtime() const noexcept
  {
    return runtime_;
  }

  vix::App &AttachedApp::vix_app() noexcept
  {
    return *app_;
  }

  const vix::App &AttachedApp::vix_app() const noexcept
  {
    return *app_;
  }

  const AppConfig &AttachedApp::config() const noexcept
  {
    return runtime_.config();
  }

  AppConfig &AttachedApp::config() noexcept
  {
    return runtime_.config();
  }

  bool AttachedApp::has_app() const noexcept
  {
    return app_ != nullptr;
  }

  AttachedApp attach(vix::App &app)
  {
    return AttachedApp{app};
  }

  AttachedApp attach(
      vix::App &app,
      AppConfig config)
  {
    return AttachedApp{
        app,
        std::move(config)};
  }

} // namespace cnerium::app

namespace cnerium
{
  app::AttachedApp attach(vix::App &app)
  {
    return app::attach(app);
  }

  app::AttachedApp attach(
      vix::App &app,
      app::AppConfig config)
  {
    return app::attach(
        app,
        std::move(config));
  }

} // namespace cnerium
