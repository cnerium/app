/**
 *
 *  @file Json.cpp
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

#include <cnerium/support/Json.hpp>

#include <utility>

namespace cnerium::support
{
  Json object()
  {
    return Json::object();
  }

  Json array()
  {
    return Json::array();
  }

  Json object(std::initializer_list<std::pair<std::string, Json>> items)
  {
    Json result = Json::object();

    for (const auto &item : items)
    {
      result[item.first] = item.second;
    }

    return result;
  }

  Json parse(std::string_view text)
  {
    return vix::json::loads(text);
  }

  std::optional<Json> try_parse(std::string_view text) noexcept
  {
    return vix::json::try_loads(text);
  }

  std::string dump(const Json &value)
  {
    return vix::json::dumps_compact(value);
  }

  std::string dump_pretty(
      const Json &value,
      int indent)
  {
    if (indent < 0)
    {
      indent = 0;
    }

    return vix::json::dumps_pretty(value, indent);
  }

  bool is_object(const Json &value) noexcept
  {
    return value.is_object();
  }

  bool is_array(const Json &value) noexcept
  {
    return value.is_array();
  }

  bool is_null(const Json &value) noexcept
  {
    return value.is_null();
  }

  bool contains(
      const Json &value,
      std::string_view key) noexcept
  {
    if (!value.is_object())
    {
      return false;
    }

    return value.contains(std::string(key));
  }

  std::string string_or(
      const Json &value,
      std::string_view key,
      std::string fallback)
  {
    return vix::json::get_or<std::string>(
        value,
        key,
        std::move(fallback));
  }

  int int_or(
      const Json &value,
      std::string_view key,
      int fallback) noexcept
  {
    return vix::json::get_or<int>(
        value,
        key,
        fallback);
  }

  bool bool_or(
      const Json &value,
      std::string_view key,
      bool fallback) noexcept
  {
    return vix::json::get_or<bool>(
        value,
        key,
        fallback);
  }

} // namespace cnerium::support
