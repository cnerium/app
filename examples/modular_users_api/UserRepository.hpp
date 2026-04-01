/**
 * @file UserRepository.hpp
 * @brief Example in-memory repository for modular users API
 */

#pragma once

#include "User.hpp"
#include <optional>
#include <vector>

namespace example::users
{
  class UserRepository
  {
  public:
    UserRepository()
        : users_{
              {1, "Alice", "alice@example.com", true},
              {2, "Bob", "bob@example.com", true},
              {3, "Charlie", "charlie@example.com", false}},
          next_id_(4)
    {
    }

    [[nodiscard]] const std::vector<User> &find_all() const noexcept
    {
      return users_;
    }

    [[nodiscard]] std::optional<User> find_by_id(int id) const
    {
      const auto index = find_index(id);
      if (!index)
      {
        return std::nullopt;
      }

      return users_[*index];
    }

    [[nodiscard]] User create(std::string name,
                              std::string email,
                              bool active = true)
    {
      User user;
      user.id = next_id_++;
      user.name = std::move(name);
      user.email = std::move(email);
      user.active = active;

      users_.push_back(user);
      return user;
    }

    [[nodiscard]] std::optional<User> update(int id,
                                             std::optional<std::string> name,
                                             std::optional<std::string> email,
                                             std::optional<bool> active)
    {
      const auto index = find_index(id);
      if (!index)
      {
        return std::nullopt;
      }

      auto &user = users_[*index];

      if (name)
      {
        user.name = std::move(*name);
      }

      if (email)
      {
        user.email = std::move(*email);
      }

      if (active.has_value())
      {
        user.active = *active;
      }

      return user;
    }

    [[nodiscard]] std::optional<User> remove(int id)
    {
      const auto index = find_index(id);
      if (!index)
      {
        return std::nullopt;
      }

      User removed = users_[*index];
      users_.erase(users_.begin() + static_cast<std::ptrdiff_t>(*index));
      return removed;
    }

  private:
    std::vector<User> users_;
    int next_id_{1};

    [[nodiscard]] std::optional<std::size_t> find_index(int id) const
    {
      for (std::size_t i = 0; i < users_.size(); ++i)
      {
        if (users_[i].id == id)
        {
          return i;
        }
      }

      return std::nullopt;
    }
  };

} // namespace example::users
