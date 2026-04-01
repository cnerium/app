/**
 * @file UserService.hpp
 * @brief Example service layer for modular users API
 */

#pragma once

#include "UserRepository.hpp"

#include <cnerium/json/json.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

namespace example::users
{
  class UserService
  {
  public:
    explicit UserService(UserRepository &repository) noexcept
        : repository_(&repository)
    {
    }

    [[nodiscard]] cnerium::json::value list_users() const
    {
      return cnerium::json::object{
          {"ok", true},
          {"count", static_cast<int>(repository_->find_all().size())},
          {"data", users_to_json(repository_->find_all())}};
    }

    [[nodiscard]] cnerium::json::value get_user(int id) const
    {
      const auto user = repository_->find_by_id(id);
      if (!user)
      {
        throw std::runtime_error("user not found");
      }

      return cnerium::json::object{
          {"ok", true},
          {"data", to_json(*user)}};
    }

    [[nodiscard]] cnerium::json::value create_user(const cnerium::json::value &body)
    {
      ensure_object(body);
      ensure_required(body, "name");
      ensure_required(body, "email");

      const auto name = body["name"].as_string();
      const auto email = body["email"].as_string();

      bool active = true;
      if (body.contains("active"))
      {
        active = body["active"].as_bool();
      }

      const auto user = repository_->create(name, email, active);

      return cnerium::json::object{
          {"ok", true},
          {"message", "user created"},
          {"data", to_json(user)}};
    }

    [[nodiscard]] cnerium::json::value update_user(int id, const cnerium::json::value &body)
    {
      ensure_object(body);

      std::optional<std::string> name;
      std::optional<std::string> email;
      std::optional<bool> active;

      if (body.contains("name"))
      {
        name = body["name"].as_string();
      }

      if (body.contains("email"))
      {
        email = body["email"].as_string();
      }

      if (body.contains("active"))
      {
        active = body["active"].as_bool();
      }

      const auto updated = repository_->update(id, std::move(name), std::move(email), active);
      if (!updated)
      {
        throw std::runtime_error("user not found");
      }

      return cnerium::json::object{
          {"ok", true},
          {"message", "user updated"},
          {"data", to_json(*updated)}};
    }

    [[nodiscard]] cnerium::json::value delete_user(int id)
    {
      const auto removed = repository_->remove(id);
      if (!removed)
      {
        throw std::runtime_error("user not found");
      }

      return cnerium::json::object{
          {"ok", true},
          {"message", "user deleted"},
          {"data", to_json(*removed)}};
    }

    [[nodiscard]] cnerium::json::value health() const
    {
      return cnerium::json::object{
          {"ok", true},
          {"status", "healthy"},
          {"users_count", static_cast<int>(repository_->find_all().size())}};
    }

  private:
    UserRepository *repository_{nullptr};

    [[nodiscard]] static cnerium::json::value to_json(const User &user)
    {
      return cnerium::json::object{
          {"id", user.id},
          {"name", user.name},
          {"email", user.email},
          {"active", user.active}};
    }

    [[nodiscard]] static cnerium::json::value users_to_json(const std::vector<User> &users)
    {
      cnerium::json::array items;
      items.reserve(users.size());

      for (const auto &user : users)
      {
        items.push_back(to_json(user));
      }

      return cnerium::json::value(std::move(items));
    }

    static void ensure_object(const cnerium::json::value &body)
    {
      if (!body.is_object())
      {
        throw std::runtime_error("request body must be a JSON object");
      }
    }

    static void ensure_required(const cnerium::json::value &body, std::string_view field)
    {
      if (!body.contains(field))
      {
        throw std::runtime_error("missing required field: " + std::string(field));
      }
    }
  };

} // namespace example::users
