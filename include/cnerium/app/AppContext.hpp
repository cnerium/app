/**
 * @file AppContext.hpp
 * @brief cnerium::app - High-level request/response context
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @copyright (c) 2026 Gaspard Kirira
 * @license MIT
 *
 * @details
 * Defines AppContext, a user-friendly wrapper around
 * cnerium::server::Context.
 *
 * AppContext:
 *   - exposes request / response / params
 *   - provides convenient helpers (text, json, status, header)
 *   - hides low-level server details
 *
 * Design goals:
 *   - Header-only
 *   - Thin wrapper (zero abstraction cost)
 *   - Developer-friendly API
 *
 * Notes:
 *   - Does not own the underlying server context
 *   - Lifetime is managed by server execution
 *   - This class intentionally forwards most operations to
 *     cnerium::server::Context
 */

#pragma once

#include <initializer_list>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include <cnerium/http/Status.hpp>
#include <cnerium/json/json.hpp>
#include <cnerium/server/Context.hpp>

namespace cnerium::app
{
  /**
   * @brief High-level request/response context wrapper.
   *
   * AppContext provides a more framework-style API while keeping the
   * underlying server context fully accessible when needed.
   */
  class AppContext
  {
  public:
    using server_context_type = cnerium::server::Context;
    using request_type = server_context_type::request_type;
    using response_type = server_context_type::response_type;
    using params_type = server_context_type::params_type;

    /**
     * @brief Construct from server context.
     *
     * @param ctx Underlying server context
     */
    explicit AppContext(server_context_type &ctx) noexcept
        : ctx_(&ctx)
    {
    }

    AppContext(const AppContext &) = default;
    AppContext &operator=(const AppContext &) = default;

    /**
     * @brief Returns true if the wrapper references a valid context.
     *
     * @return true if valid
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return ctx_ != nullptr;
    }

    /**
     * @brief Explicit boolean conversion.
     *
     * @return true if valid
     */
    explicit operator bool() const noexcept
    {
      return valid();
    }

    /**
     * @brief Access the underlying request.
     *
     * @return request_type& Request
     */
    [[nodiscard]] request_type &request() noexcept
    {
      ensure_valid();
      return ctx_->request();
    }

    /**
     * @brief Access the underlying request.
     *
     * @return const request_type& Request
     */
    [[nodiscard]] const request_type &request() const noexcept
    {
      ensure_valid();
      return ctx_->request();
    }

    /**
     * @brief Access the underlying response.
     *
     * @return response_type& Response
     */
    [[nodiscard]] response_type &response() noexcept
    {
      ensure_valid();
      return ctx_->response();
    }

    /**
     * @brief Access the underlying response.
     *
     * @return const response_type& Response
     */
    [[nodiscard]] const response_type &response() const noexcept
    {
      ensure_valid();
      return ctx_->response();
    }

    /**
     * @brief Access the route parameters.
     *
     * @return params_type& Params
     */
    [[nodiscard]] params_type &params() noexcept
    {
      ensure_valid();
      return ctx_->params();
    }

    /**
     * @brief Access the route parameters.
     *
     * @return const params_type& Params
     */
    [[nodiscard]] const params_type &params() const noexcept
    {
      ensure_valid();
      return ctx_->params();
    }

    /**
     * @brief Returns the HTTP method.
     */
    [[nodiscard]] cnerium::http::Method method() const noexcept
    {
      ensure_valid();
      return ctx_->method();
    }

    /**
     * @brief Returns the request path.
     */
    [[nodiscard]] std::string_view path() const noexcept
    {
      ensure_valid();
      return ctx_->path();
    }

    /**
     * @brief Returns the raw query string.
     */
    [[nodiscard]] std::string_view query() const noexcept
    {
      ensure_valid();
      return ctx_->query();
    }

    /**
     * @brief Returns a request header value.
     *
     * @param key Header name
     * @return std::string_view Header value
     */
    [[nodiscard]] std::string_view header(std::string_view key) const noexcept
    {
      ensure_valid();
      return ctx_->header(key);
    }

    /**
     * @brief Returns a route parameter.
     *
     * @param key Parameter name
     * @return std::string_view Parameter value
     */
    [[nodiscard]] std::string_view param(std::string_view key) const noexcept
    {
      ensure_valid();
      return ctx_->param(key);
    }

    /**
     * @brief Returns true if a route parameter exists.
     *
     * @param key Parameter name
     * @return true if found
     */
    [[nodiscard]] bool has_param(std::string_view key) const noexcept
    {
      ensure_valid();
      return ctx_->has_param(key);
    }

    /**
     * @brief Returns true if any route parameters are present.
     *
     * @return true if params are present
     */
    [[nodiscard]] bool has_params() const noexcept
    {
      ensure_valid();
      return ctx_->has_params();
    }

    /**
     * @brief Returns the raw request body.
     *
     * @return std::string_view Request body
     */
    [[nodiscard]] std::string_view body() const noexcept
    {
      ensure_valid();
      return ctx_->body();
    }

    /**
     * @brief Parse request body as JSON.
     *
     * @return cnerium::json::value Parsed JSON value
     */
    [[nodiscard]] cnerium::json::value json() const
    {
      ensure_valid();
      return ctx_->json();
    }

    /**
     * @brief Set response status.
     *
     * @param s HTTP status
     * @return AppContext& Self
     */
    AppContext &status(cnerium::http::Status s)
    {
      ensure_valid();
      ctx_->status(s);
      return *this;
    }

    /**
     * @brief Set response header.
     *
     * @param key Header name
     * @param value Header value
     * @return AppContext& Self
     */
    AppContext &set_header(std::string key, std::string value)
    {
      ensure_valid();
      ctx_->set_header(std::move(key), std::move(value));
      return *this;
    }

    /**
     * @brief Set response header.
     *
     * @param key Header name
     * @param value Header value
     * @return AppContext& Self
     */
    AppContext &header(std::string key, std::string value)
    {
      ensure_valid();
      ctx_->set_header(std::move(key), std::move(value));
      return *this;
    }

    /**
     * @brief Set Content-Type header.
     *
     * @param value MIME type
     * @return AppContext& Self
     */
    AppContext &content_type(std::string value)
    {
      ensure_valid();
      ctx_->content_type(std::move(value));
      return *this;
    }

    /**
     * @brief Send plain text response.
     *
     * @param value Response body
     * @return AppContext& Self
     */
    AppContext &text(std::string value)
    {
      ensure_valid();
      ctx_->text(std::move(value));
      return *this;
    }

    /**
     * @brief Send HTML response.
     *
     * @param value Response body
     * @return AppContext& Self
     */
    AppContext &html(std::string value)
    {
      ensure_valid();
      ctx_->html(std::move(value));
      return *this;
    }

    /**
     * @brief Send JSON response from a JSON value.
     *
     * @param value JSON value
     * @return AppContext& Self
     */
    AppContext &json(const cnerium::json::value &value)
    {
      ensure_valid();
      ctx_->json(value);
      return *this;
    }

    /**
     * @brief Send JSON response from a temporary JSON value.
     *
     * @param value JSON value
     * @return AppContext& Self
     */
    AppContext &json(cnerium::json::value &&value)
    {
      ensure_valid();
      ctx_->json(std::move(value));
      return *this;
    }

    /**
     * @brief Send JSON response from an initializer list.
     *
     * Allows usage like:
     * @code
     * ctx.json({
     *   {"ok", true},
     *   {"framework", "Cnerium"}
     * });
     * @endcode
     *
     * @param value JSON object initializer
     * @return AppContext& Self
     */
    AppContext &json(
        std::initializer_list<std::pair<std::string_view, cnerium::json::value>> value)
    {
      ensure_valid();
      ctx_->json(cnerium::json::value(value));
      return *this;
    }

    /**
     * @brief Send a success JSON response.
     *
     * @param message Success message
     * @return AppContext& Self
     */
    AppContext &ok(std::string message = "ok")
    {
      ensure_valid();
      ctx_->ok(std::move(message));
      return *this;
    }

    /**
     * @brief Send an error JSON response.
     *
     * @param s HTTP status
     * @param message Error message
     * @return AppContext& Self
     */
    AppContext &error(cnerium::http::Status s, std::string message)
    {
      ensure_valid();
      ctx_->error(s, std::move(message));
      return *this;
    }

    /**
     * @brief Access underlying server context.
     *
     * @return server_context_type& Underlying context
     */
    [[nodiscard]] server_context_type &raw() noexcept
    {
      ensure_valid();
      return *ctx_;
    }

    /**
     * @brief Access underlying server context.
     *
     * @return const server_context_type& Underlying context
     */
    [[nodiscard]] const server_context_type &raw() const noexcept
    {
      ensure_valid();
      return *ctx_;
    }

  private:
    /**
     * @brief Throw if the wrapper does not reference a valid context.
     */
    void ensure_valid() const
    {
      if (ctx_ == nullptr)
      {
        throw std::logic_error("invalid app context");
      }
    }

    server_context_type *ctx_{nullptr};
  };

} // namespace cnerium::app
