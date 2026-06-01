/**
 *
 *  @file DurableResponse.cpp
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

#include <cnerium/http/DurableResponse.hpp>

#include <utility>

namespace cnerium::http
{
  namespace
  {
    [[nodiscard]] bool valid_status_code(int value) noexcept
    {
      return value >= 100 && value <= 599;
    }
  } // namespace

  DurableResponse::DurableResponse(
      int status_code,
      std::string body,
      std::string content_type)
      : status_code_(status_code),
        body_(std::move(body)),
        content_type_(std::move(content_type))
  {
  }

  DurableResponse DurableResponse::json(
      int status_code,
      const support::Json &body)
  {
    return DurableResponse{
        status_code,
        support::dump(body),
        "application/json; charset=utf-8"};
  }

  DurableResponse DurableResponse::ok(const support::Json &body)
  {
    return json(200, body);
  }

  DurableResponse DurableResponse::created(const support::Json &body)
  {
    return json(201, body);
  }

  DurableResponse DurableResponse::text(
      int status_code,
      std::string body)
  {
    return DurableResponse{
        status_code,
        std::move(body),
        "text/plain; charset=utf-8"};
  }

  DurableResponse DurableResponse::error(
      int status_code,
      std::string message)
  {
    return json(
        status_code,
        support::object({
            {"error", support::Json(std::move(message))},
        }));
  }

  DurableResponse DurableResponse::bad_request(std::string message)
  {
    return error(400, std::move(message));
  }

  DurableResponse DurableResponse::conflict(std::string message)
  {
    return error(409, std::move(message));
  }

  DurableResponse DurableResponse::internal_error(std::string message)
  {
    return error(500, std::move(message));
  }

  DurableResponse DurableResponse::from_stored(
      const store::StoredResponse &response)
  {
    return DurableResponse{
        response.status_code(),
        response.body(),
        response.content_type()};
  }

  int DurableResponse::status_code() const noexcept
  {
    return status_code_;
  }

  const std::string &DurableResponse::body() const noexcept
  {
    return body_;
  }

  const std::string &DurableResponse::content_type() const noexcept
  {
    return content_type_;
  }

  void DurableResponse::set_status_code(int value) noexcept
  {
    status_code_ = value;
  }

  void DurableResponse::set_body(std::string value)
  {
    body_ = std::move(value);
  }

  void DurableResponse::set_content_type(std::string value)
  {
    content_type_ = std::move(value);
  }

  bool DurableResponse::has_body() const noexcept
  {
    return !body_.empty();
  }

  bool DurableResponse::has_content_type() const noexcept
  {
    return !content_type_.empty();
  }

  bool DurableResponse::is_valid() const noexcept
  {
    return valid_status_code(status_code_);
  }

  bool DurableResponse::valid() const noexcept
  {
    return is_valid();
  }

  store::StoredResponse DurableResponse::to_stored_response() const
  {
    return store::StoredResponse{
        status_code_,
        body_,
        content_type_};
  }

  void DurableResponse::clear() noexcept
  {
    status_code_ = 200;
    body_ = "{}";
    content_type_ = "application/json; charset=utf-8";
  }

  DurableResponse json(
      int status_code,
      const support::Json &body)
  {
    return DurableResponse::json(status_code, body);
  }

  DurableResponse json(const support::Json &body)
  {
    return DurableResponse::ok(body);
  }

  DurableResponse created(const support::Json &body)
  {
    return DurableResponse::created(body);
  }

  DurableResponse text(
      int status_code,
      std::string body)
  {
    return DurableResponse::text(
        status_code,
        std::move(body));
  }

} // namespace cnerium::http

namespace cnerium
{
  http::DurableResponse json(
      int status_code,
      const support::Json &body)
  {
    return http::json(status_code, body);
  }

  http::DurableResponse json(const support::Json &body)
  {
    return http::json(body);
  }

  http::DurableResponse created(const support::Json &body)
  {
    return http::created(body);
  }

  http::DurableResponse text(
      int status_code,
      std::string body)
  {
    return http::text(
        status_code,
        std::move(body));
  }

} // namespace cnerium
