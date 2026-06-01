/**
 *
 *  @file DurableRequest.hpp
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

#ifndef CNERIUM_HTTP_DURABLE_REQUEST_HPP
#define CNERIUM_HTTP_DURABLE_REQUEST_HPP

#include <string>
#include <string_view>

#include <vix/http/Request.hpp>

#include <cnerium/reliability/IdempotencyKey.hpp>
#include <cnerium/reliability/RequestHash.hpp>
#include <cnerium/support/Json.hpp>

namespace cnerium::http
{
  /**
   * @brief Durable request wrapper around a Vix HTTP request.
   *
   * DurableRequest exposes only the request data needed by Cnerium durable
   * handlers while keeping the real HTTP runtime owned by Vix.
   *
   * It does not parse sockets, own sessions, or create a new HTTP layer.
   * It simply wraps vix::http::Request for reliability-first route handlers.
   */
  class DurableRequest
  {
  public:
    /**
     * @brief Creates a durable request from a Vix HTTP request.
     *
     * @param request Vix HTTP request.
     */
    explicit DurableRequest(const vix::http::Request &request) noexcept;

    DurableRequest(const DurableRequest &) = delete;
    DurableRequest &operator=(const DurableRequest &) = delete;

    DurableRequest(DurableRequest &&) noexcept = default;
    DurableRequest &operator=(DurableRequest &&) noexcept = default;

    /**
     * @brief Returns the HTTP method.
     *
     * @return Request method.
     */
    [[nodiscard]] std::string method() const;

    /**
     * @brief Returns the request target.
     *
     * The target may include the query string.
     *
     * @return Request target.
     */
    [[nodiscard]] std::string target() const;

    /**
     * @brief Returns the request path.
     *
     * @return Request path.
     */
    [[nodiscard]] std::string path() const;

    /**
     * @brief Returns the request body.
     *
     * @return Request body.
     */
    [[nodiscard]] const std::string &body() const noexcept;

    /**
     * @brief Returns an HTTP header value.
     *
     * @param name Header name.
     * @return Header value, or an empty string if missing.
     */
    [[nodiscard]] std::string header(std::string_view name) const;

    /**
     * @brief Returns true if a header exists.
     *
     * @param name Header name.
     * @return true if the header exists.
     */
    [[nodiscard]] bool has_header(std::string_view name) const;

    /**
     * @brief Returns the Idempotency-Key header as a value object.
     *
     * @return Idempotency key.
     */
    [[nodiscard]] reliability::IdempotencyKey idempotency_key() const;

    /**
     * @brief Returns the raw Idempotency-Key header value.
     *
     * @return Idempotency-Key header value.
     */
    [[nodiscard]] std::string idempotency_key_value() const;

    /**
     * @brief Computes the stable request body hash.
     *
     * @return Request hash.
     */
    [[nodiscard]] reliability::RequestHash request_hash() const;

    /**
     * @brief Parses the request body as JSON.
     *
     * @return Parsed JSON body.
     *
     * @throws nlohmann::json::parse_error if the body is not valid JSON.
     */
    [[nodiscard]] support::Json json() const;

    /**
     * @brief Parses the request body as JSON without throwing.
     *
     * @return Parsed JSON body or std::nullopt on failure.
     */
    [[nodiscard]] std::optional<support::Json> try_json() const noexcept;

    /**
     * @brief Returns a route parameter value.
     *
     * @param name Parameter name.
     * @return Parameter value, or an empty string if missing.
     */
    [[nodiscard]] std::string param(std::string_view name) const;

    /**
     * @brief Returns a query parameter value.
     *
     * @param name Query parameter name.
     * @return Query value, or an empty string if missing.
     */
    [[nodiscard]] std::string query(std::string_view name) const;

    /**
     * @brief Returns the wrapped Vix request.
     *
     * @return Vix HTTP request.
     */
    [[nodiscard]] const vix::http::Request &native() const noexcept;

  private:
    const vix::http::Request *request_{nullptr};
  };

} // namespace cnerium::http

namespace cnerium
{
  using DurableRequest = http::DurableRequest;
} // namespace cnerium

#endif // CNERIUM_HTTP_DURABLE_REQUEST_HPP
