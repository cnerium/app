/**
 * @file UserController.hpp
 * @brief Example controller layer for modular users API
 */

#pragma once

#include "UserService.hpp"

#include <cnerium/app/app.hpp>
#include <cnerium/http/Status.hpp>

#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace example::users
{
  class UserController
  {
  public:
    explicit UserController(UserService &service) noexcept
        : service_(&service)
    {
    }

    void register_routes(cnerium::app::App &app)
    {
      app.get("/", [this](cnerium::app::AppContext &ctx)
              { handle_root(ctx); });

      app.get("/health", [this](cnerium::app::AppContext &ctx)
              { handle_health(ctx); });

      app.get("/api/users", [this](cnerium::app::AppContext &ctx)
              { handle_list(ctx); });

      app.get("/api/users/:id", [this](cnerium::app::AppContext &ctx)
              { handle_show(ctx); });

      app.post("/api/users", [this](cnerium::app::AppContext &ctx)
               { handle_create(ctx); });

      app.put("/api/users/:id", [this](cnerium::app::AppContext &ctx)
              { handle_update(ctx); });

      app.delete_("/api/users/:id", [this](cnerium::app::AppContext &ctx)
                  { handle_delete(ctx); });
    }

  private:
    UserService *service_{nullptr};

    static std::optional<int> parse_id(cnerium::app::AppContext &ctx)
    {
      try
      {
        return std::stoi(std::string(ctx.param("id")));
      }
      catch (...)
      {
        return std::nullopt;
      }
    }

    static void json_error(cnerium::app::AppContext &ctx,
                           cnerium::http::Status status,
                           std::string message)
    {
      ctx.status(status).json({
          {"ok", false},
          {"error", std::move(message)},
      });
    }

    void handle_root(cnerium::app::AppContext &ctx)
    {
      ctx.json({
          {"ok", true},
          {"name", "Cnerium Modular Users API"},
          {"version", "0.1.0"},
          {"architecture", "controller-service-repository"},
          {"endpoints", cnerium::json::array{
                            "GET /",
                            "GET /health",
                            "GET /api/users",
                            "GET /api/users/:id",
                            "POST /api/users",
                            "PUT /api/users/:id",
                            "DELETE /api/users/:id",
                        }},
      });
    }

    void handle_health(cnerium::app::AppContext &ctx)
    {
      ctx.json(service_->health());
    }

    void handle_list(cnerium::app::AppContext &ctx)
    {
      ctx.json(service_->list_users());
    }

    void handle_show(cnerium::app::AppContext &ctx)
    {
      const auto id = parse_id(ctx);
      if (!id)
      {
        json_error(ctx, cnerium::http::Status::bad_request, "invalid user id");
        return;
      }

      try
      {
        ctx.json(service_->get_user(*id));
      }
      catch (const std::exception &ex)
      {
        json_error(ctx, cnerium::http::Status::not_found, ex.what());
      }
    }

    void handle_create(cnerium::app::AppContext &ctx)
    {
      try
      {
        const auto body = ctx.json();
        ctx.status(cnerium::http::Status::created)
            .json(service_->create_user(body));
      }
      catch (const std::exception &ex)
      {
        json_error(ctx, cnerium::http::Status::bad_request, ex.what());
      }
    }

    void handle_update(cnerium::app::AppContext &ctx)
    {
      const auto id = parse_id(ctx);
      if (!id)
      {
        json_error(ctx, cnerium::http::Status::bad_request, "invalid user id");
        return;
      }

      try
      {
        const auto body = ctx.json();
        ctx.json(service_->update_user(*id, body));
      }
      catch (const std::runtime_error &ex)
      {
        if (std::string_view(ex.what()) == "user not found")
        {
          json_error(ctx, cnerium::http::Status::not_found, ex.what());
          return;
        }

        json_error(ctx, cnerium::http::Status::bad_request, ex.what());
      }
      catch (const std::exception &ex)
      {
        json_error(ctx, cnerium::http::Status::bad_request, ex.what());
      }
    }

    void handle_delete(cnerium::app::AppContext &ctx)
    {
      const auto id = parse_id(ctx);
      if (!id)
      {
        json_error(ctx, cnerium::http::Status::bad_request, "invalid user id");
        return;
      }

      try
      {
        ctx.json(service_->delete_user(*id));
      }
      catch (const std::exception &ex)
      {
        json_error(ctx, cnerium::http::Status::not_found, ex.what());
      }
    }
  };

} // namespace example::users
