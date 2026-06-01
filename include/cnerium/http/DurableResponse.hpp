/**
 *
 *  @file DurableResponse.hpp
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

#ifndef CNERIUM_HTTP_DURABLE_RESPONSE_HPP
#define CNERIUM_HTTP_DURABLE_RESPONSE_HPP

#include <string>
#include <string_view>

#include <cnerium/store/StoredResponse.hpp>
#include <cnerium/support/Json.hpp>

namespace cnerium::http
{
  /**
   * @brief Response returned by a durable Cnerium route handler.
   *
   * DurableResponse is the framework-level response type used by durable
   * handlers before the response is written back through Vix HTTP.
   *
   * It is intentionally small:
   * - HTTP status code
   * - response body
   * - content type
   *
   * The same response can be converted into a StoredResponse and persisted by
   * the idempotency layer for safe replay.
   */
  class DurableResponse
  {
  public:
    /**
     * @brief Creates a default JSON 200 response.
     */
    DurableResponse() = default;

    /**
     * @brief Creates a durable response.
     *
     * @param status_code HTTP status code.
     * @param body Response body.
     * @param content_type Response content type.
     */
    DurableResponse(
        int status_code,
        std::string body,
        std::string content_type = "application/json; charset=utf-8");

    /**
     * @brief Creates a JSON durable response.
     *
     * @param status_code HTTP status code.
     * @param body JSON response body.
     * @return Durable response.
     */
    [[nodiscard]] static DurableResponse json(
        int status_code,
        const support::Json &body);

    /**
     * @brief Creates a JSON 200 durable response.
     *
     * @param body JSON response body.
     * @return Durable response.
     */
    [[nodiscard]] static DurableResponse ok(const support::Json &body);

    /**
     * @brief Creates a JSON 201 durable response.
     *
     * @param body JSON response body.
     * @return Durable response.
     */
    [[nodiscard]] static DurableResponse created(const support::Json &body);

    /**
     * @brief Creates a text durable response.
     *
     * @param status_code HTTP status code.
     * @param body Text response body.
     * @return Durable response.
     */
    [[nodiscard]] static DurableResponse text(
        int status_code,
        std::string body);

    /**
     * @brief Creates a JSON error response.
     *
     * @param status_code HTTP status code.
     * @param message Error message.
     * @return Durable response.
     */
    [[nodiscard]] static DurableResponse error(
        int status_code,
        std::string message);

    /**
     * @brief Creates a bad request response.
     *
     * @param message Error message.
     * @return Durable response.
     */
    [[nodiscard]] static DurableResponse bad_request(std::string message);

    /**
     * @brief Creates a conflict response.
     *
     * @param message Error message.
     * @return Durable response.
     */
    [[nodiscard]] static DurableResponse conflict(std::string message);

    /**
     * @brief Creates an internal server error response.
     *
     * @param message Error message.
     * @return Durable response.
     */
    [[nodiscard]] static DurableResponse internal_error(std::string message);

    /**
     * @brief Restores a durable response from a stored response.
     *
     * @param response Stored response.
     * @return Durable response.
     */
    [[nodiscard]] static DurableResponse from_stored(
        const store::StoredResponse &response);

    /**
     * @brief Returns the HTTP status code.
     *
     * @return HTTP status code.
     */
    [[nodiscard]] int status_code() const noexcept;

    /**
     * @brief Returns the response body.
     *
     * @return Response body.
     */
    [[nodiscard]] const std::string &body() const noexcept;

    /**
     * @brief Returns the response content type.
     *
     * @return Content type.
     */
    [[nodiscard]] const std::string &content_type() const noexcept;

    /**
     * @brief Sets the HTTP status code.
     *
     * @param value HTTP status code.
     */
    void set_status_code(int value) noexcept;

    /**
     * @brief Sets the response body.
     *
     * @param value Response body.
     */
    void set_body(std::string value);

    /**
     * @brief Sets the response content type.
     *
     * @param value Content type.
     */
    void set_content_type(std::string value);

    /**
     * @brief Returns true if the response has a body.
     *
     * @return true if body is not empty.
     */
    [[nodiscard]] bool has_body() const noexcept;

    /**
     * @brief Returns true if the response has a content type.
     *
     * @return true if content type is not empty.
     */
    [[nodiscard]] bool has_content_type() const noexcept;

    /**
     * @brief Returns true if the response status code is valid.
     *
     * @return true if status code is between 100 and 599.
     */
    [[nodiscard]] bool is_valid() const noexcept;

    /**
     * @brief Backward-compatible alias for is_valid().
     *
     * @return true if the response is valid.
     */
    [[nodiscard]] bool valid() const noexcept;

    /**
     * @brief Converts this response to a storable response.
     *
     * @return Stored response.
     */
    [[nodiscard]] store::StoredResponse to_stored_response() const;

    /**
     * @brief Clears the response.
     */
    void clear() noexcept;

  private:
    int status_code_{200};
    std::string body_{"{}"};
    std::string content_type_{"application/json; charset=utf-8"};
  };

  /**
   * @brief Creates a JSON durable response.
   *
   * Convenience function for application code.
   *
   * @param status_code HTTP status code.
   * @param body JSON response body.
   * @return Durable response.
   */
  [[nodiscard]] DurableResponse json(
      int status_code,
      const support::Json &body);

  /**
   * @brief Creates a JSON 200 durable response.
   *
   * Convenience function for application code.
   *
   * @param body JSON response body.
   * @return Durable response.
   */
  [[nodiscard]] DurableResponse json(const support::Json &body);

  /**
   * @brief Creates a JSON 201 durable response.
   *
   * Convenience function for application code.
   *
   * @param body JSON response body.
   * @return Durable response.
   */
  [[nodiscard]] DurableResponse created(const support::Json &body);

  /**
   * @brief Creates a text durable response.
   *
   * Convenience function for application code.
   *
   * @param status_code HTTP status code.
   * @param body Text response body.
   * @return Durable response.
   */
  [[nodiscard]] DurableResponse text(
      int status_code,
      std::string body);

} // namespace cnerium::http

namespace cnerium
{
  using DurableResponse = http::DurableResponse;

  /**
   * @brief Creates a JSON durable response.
   *
   * @param status_code HTTP status code.
   * @param body JSON response body.
   * @return Durable response.
   */
  [[nodiscard]] http::DurableResponse json(
      int status_code,
      const support::Json &body);

  /**
   * @brief Creates a JSON 200 durable response.
   *
   * @param body JSON response body.
   * @return Durable response.
   */
  [[nodiscard]] http::DurableResponse json(const support::Json &body);

  /**
   * @brief Creates a JSON 201 durable response.
   *
   * @param body JSON response body.
   * @return Durable response.
   */
  [[nodiscard]] http::DurableResponse created(const support::Json &body);

  /**
   * @brief Creates a text durable response.
   *
   * @param status_code HTTP status code.
   * @param body Text response body.
   * @return Durable response.
   */
  [[nodiscard]] http::DurableResponse text(
      int status_code,
      std::string body);

} // namespace cnerium

#endif // CNERIUM_HTTP_DURABLE_RESPONSE_HPP
