/**
 *
 *  @file Json.hpp
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

#ifndef CNERIUM_SUPPORT_JSON_HPP
#define CNERIUM_SUPPORT_JSON_HPP

#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include <vix/json/json.hpp>

namespace cnerium::support
{
  /**
   * @brief Public JSON type used by Cnerium.
   *
   * Cnerium uses the Vix JSON type because Cnerium runs on top of Vix HTTP,
   * Vix WebSocket, and Vix response helpers.
   */
  using Json = vix::json::Json;

  /**
   * @brief Ordered JSON type used when deterministic object order is useful.
   */
  using OrderedJson = vix::json::OrderedJson;

  /**
   * @brief Lightweight Vix JSON token type.
   *
   * This is useful for WebSocket typed payloads and small structured events.
   */
  using JsonToken = vix::json::token;

  /**
   * @brief Lightweight Vix JSON object payload type.
   */
  using JsonObjectPayload = vix::json::kvs;

  /**
   * @brief Lightweight Vix JSON array payload type.
   */
  using JsonArrayPayload = vix::json::array_t;

  /**
   * @brief Creates an empty JSON object.
   *
   * @return Empty JSON object.
   */
  [[nodiscard]] Json object();

  /**
   * @brief Creates an empty JSON array.
   *
   * @return Empty JSON array.
   */
  [[nodiscard]] Json array();

  /**
   * @brief Creates a JSON object from key/value pairs.
   *
   * @param items Key/value pairs.
   * @return JSON object.
   */
  [[nodiscard]] Json object(
      std::initializer_list<std::pair<std::string, Json>> items);

  /**
   * @brief Parses JSON text.
   *
   * @param text JSON text.
   * @return Parsed JSON value.
   *
   * @throws nlohmann::json::parse_error if the text is invalid.
   */
  [[nodiscard]] Json parse(std::string_view text);

  /**
   * @brief Parses JSON text without throwing.
   *
   * @param text JSON text.
   * @return Parsed JSON value or std::nullopt on failure.
   */
  [[nodiscard]] std::optional<Json> try_parse(std::string_view text) noexcept;

  /**
   * @brief Serializes JSON to compact text.
   *
   * @param value JSON value.
   * @return Compact JSON string.
   */
  [[nodiscard]] std::string dump(const Json &value);

  /**
   * @brief Serializes JSON to pretty text.
   *
   * @param value JSON value.
   * @param indent Number of spaces per indentation level.
   * @return Pretty JSON string.
   */
  [[nodiscard]] std::string dump_pretty(
      const Json &value,
      int indent = 2);

  /**
   * @brief Returns true if the JSON value is an object.
   *
   * @param value JSON value.
   * @return true if value is an object.
   */
  [[nodiscard]] bool is_object(const Json &value) noexcept;

  /**
   * @brief Returns true if the JSON value is an array.
   *
   * @param value JSON value.
   * @return true if value is an array.
   */
  [[nodiscard]] bool is_array(const Json &value) noexcept;

  /**
   * @brief Returns true if the JSON value is null.
   *
   * @param value JSON value.
   * @return true if value is null.
   */
  [[nodiscard]] bool is_null(const Json &value) noexcept;

  /**
   * @brief Returns true if an object contains a key.
   *
   * @param value JSON object.
   * @param key Object key.
   * @return true if value is an object and contains key.
   */
  [[nodiscard]] bool contains(
      const Json &value,
      std::string_view key) noexcept;

  /**
   * @brief Returns a string field or a fallback.
   *
   * @param value JSON object.
   * @param key Object key.
   * @param fallback Fallback value.
   * @return String value or fallback.
   */
  [[nodiscard]] std::string string_or(
      const Json &value,
      std::string_view key,
      std::string fallback = {});

  /**
   * @brief Returns an integer field or a fallback.
   *
   * @param value JSON object.
   * @param key Object key.
   * @param fallback Fallback value.
   * @return Integer value or fallback.
   */
  [[nodiscard]] int int_or(
      const Json &value,
      std::string_view key,
      int fallback = 0) noexcept;

  /**
   * @brief Returns a boolean field or a fallback.
   *
   * @param value JSON object.
   * @param key Object key.
   * @param fallback Fallback value.
   * @return Boolean value or fallback.
   */
  [[nodiscard]] bool bool_or(
      const Json &value,
      std::string_view key,
      bool fallback = false) noexcept;

} // namespace cnerium::support

namespace cnerium
{
  /**
   * @brief Public Cnerium JSON type.
   */
  using Json = support::Json;

  /**
   * @brief Public Cnerium ordered JSON type.
   */
  using OrderedJson = support::OrderedJson;

} // namespace cnerium

#endif // CNERIUM_SUPPORT_JSON_HPP
