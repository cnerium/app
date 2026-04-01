/**
 * @file 06_rest_api_complete.cpp
 * @brief Complete REST API example for cnerium/app
 */

#include <cnerium/app/app.hpp>
#include <cnerium/json/json.hpp>

#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

using cnerium::app::App;
using cnerium::app::AppContext;
using cnerium::json::array;
using cnerium::json::object;
using cnerium::json::value;

namespace
{
  struct User
  {
    int id{};
    std::string name;
    std::string email;
    bool active{true};
  };

  std::vector<User> users = {
      {1, "Alice", "alice@example.com", true},
      {2, "Bob", "bob@example.com", true},
      {3, "Charlie", "charlie@example.com", false},
  };

  int next_user_id = 4;

  value to_json(const User &user)
  {
    return object{
        {"id", user.id},
        {"name", user.name},
        {"email", user.email},
        {"active", user.active},
    };
  }

  value to_json_users(const std::vector<User> &items)
  {
    array result;
    result.reserve(items.size());

    for (const auto &user : items)
    {
      result.push_back(to_json(user));
    }

    return value(std::move(result));
  }

  std::optional<std::size_t> find_user_index_by_id(int id)
  {
    for (std::size_t i = 0; i < users.size(); ++i)
    {
      if (users[i].id == id)
      {
        return i;
      }
    }

    return std::nullopt;
  }

  std::optional<int> parse_id(std::string_view raw)
  {
    try
    {
      return std::stoi(std::string(raw));
    }
    catch (...)
    {
      return std::nullopt;
    }
  }

  void json_error(AppContext &ctx,
                  cnerium::http::Status status,
                  std::string message)
  {
    ctx.status(status).json({
        {"ok", false},
        {"error", std::move(message)},
    });
  }

  std::optional<int> param_as_int(AppContext &ctx, std::string_view key)
  {
    return parse_id(ctx.param(key));
  }

} // namespace

int main()
{
  App app;

  /**
   * Root
   */
  app.get("/", [](AppContext &ctx)
          { ctx.json({
                {"ok", true},
                {"name", "Cnerium Users API"},
                {"version", "0.1.0"},
                {"endpoints", array{
                                  "GET /",
                                  "GET /health",
                                  "GET /api/users",
                                  "GET /api/users/:id",
                                  "POST /api/users",
                                  "PUT /api/users/:id",
                                  "DELETE /api/users/:id",
                              }},
            }); });

  /**
   * Health check
   */
  app.get("/health", [](AppContext &ctx)
          { ctx.json({
                {"ok", true},
                {"status", "healthy"},
                {"service", "users-api"},
                {"users_count", static_cast<int>(users.size())},
            }); });

  /**
   * GET /api/users
   */
  app.get("/api/users", [](AppContext &ctx)
          { ctx.json({
                {"ok", true},
                {"count", static_cast<int>(users.size())},
                {"data", to_json_users(users)},
            }); });

  /**
   * GET /api/users/:id
   */
  app.get("/api/users/:id", [](AppContext &ctx)
          {
            const auto id = param_as_int(ctx, "id");

            if (!id)
            {
              json_error(ctx, cnerium::http::Status::bad_request, "invalid user id");
              return;
            }

            const auto index = find_user_index_by_id(*id);
            if (!index)
            {
              json_error(ctx, cnerium::http::Status::not_found, "user not found");
              return;
            }

            ctx.json({
                {"ok", true},
                {"data", to_json(users[*index])},
            }); });

  /**
   * POST /api/users
   *
   * Expected JSON:
   * {
   *   "name": "John",
   *   "email": "john@example.com",
   *   "active": true
   * }
   */
  app.post("/api/users", [](AppContext &ctx)
           {
             try
             {
               const value body = ctx.json();

               if (!body.is_object())
               {
                 json_error(ctx, cnerium::http::Status::bad_request, "request body must be a JSON object");
                 return;
               }

               if (!body.contains("name") || !body.contains("email"))
               {
                 json_error(ctx, cnerium::http::Status::bad_request, "fields 'name' and 'email' are required");
                 return;
               }

               User user;
               user.id = next_user_id++;
               user.name = body["name"].as_string();
               user.email = body["email"].as_string();
               user.active = body.contains("active") ? body["active"].as_bool() : true;

               users.push_back(user);

               ctx.status(cnerium::http::Status::created).json({
                   {"ok", true},
                   {"message", "user created"},
                   {"data", to_json(user)},
               });
             }
             catch (const std::exception &ex)
             {
               json_error(ctx, cnerium::http::Status::bad_request, ex.what());
             } });

  /**
   * PUT /api/users/:id
   *
   * Expected JSON:
   * {
   *   "name": "Updated Name",
   *   "email": "updated@example.com",
   *   "active": false
   * }
   */
  app.put("/api/users/:id", [](AppContext &ctx)
          {
            const auto id = param_as_int(ctx, "id");

            if (!id)
            {
              json_error(ctx, cnerium::http::Status::bad_request, "invalid user id");
              return;
            }

            const auto index = find_user_index_by_id(*id);
            if (!index)
            {
              json_error(ctx, cnerium::http::Status::not_found, "user not found");
              return;
            }

            try
            {
              const value body = ctx.json();

              if (!body.is_object())
              {
                json_error(ctx, cnerium::http::Status::bad_request, "request body must be a JSON object");
                return;
              }

              auto &user = users[*index];

              if (body.contains("name"))
              {
                user.name = body["name"].as_string();
              }

              if (body.contains("email"))
              {
                user.email = body["email"].as_string();
              }

              if (body.contains("active"))
              {
                user.active = body["active"].as_bool();
              }

              ctx.json({
                  {"ok", true},
                  {"message", "user updated"},
                  {"data", to_json(user)},
              });
            }
            catch (const std::exception &ex)
            {
              json_error(ctx, cnerium::http::Status::bad_request, ex.what());
            } });

  /**
   * DELETE /api/users/:id
   */
  app.delete_("/api/users/:id", [](AppContext &ctx)
              {
                const auto id = param_as_int(ctx, "id");

                if (!id)
                {
                  json_error(ctx, cnerium::http::Status::bad_request, "invalid user id");
                  return;
                }

                const auto index = find_user_index_by_id(*id);
                if (!index)
                {
                  json_error(ctx, cnerium::http::Status::not_found, "user not found");
                  return;
                }

                const auto removed = users[*index];
                users.erase(users.begin() + static_cast<std::ptrdiff_t>(*index));

                ctx.json({
                    {"ok", true},
                    {"message", "user deleted"},
                    {"data", to_json(removed)},
                }); });

  /**
   * Optional custom not found handler
   */
  app.set_not_found_handler([](cnerium::server::Context &ctx)
                            { ctx.status(cnerium::http::Status::not_found)
                                  .json({
                                      {"ok", false},
                                      {"error", "route not found"},
                                  }); });

  /**
   * Optional custom error handler
   */
  app.set_error_handler([](cnerium::server::Context &ctx, const std::exception &ex)
                        { ctx.status(cnerium::http::Status::internal_server_error)
                              .json({
                                  {"ok", false},
                                  {"error", "internal server error"},
                                  {"message", ex.what()},
                              }); });

  app.listen("127.0.0.1", 8080, []()
             { std::cout << "Users API is ready.\n"; });
}
