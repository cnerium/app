/**
 * @file User.hpp
 * @brief Example domain model for modular users API
 */

#pragma once

#include <string>

namespace example::users
{
  struct User
  {
    int id{};
    std::string name;
    std::string email;
    bool active{true};
  };

} // namespace example::users
