/**
 *
 *  @file StoredResponse.cpp
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

#include <cnerium/store/StoredResponse.hpp>
#include <utility>

namespace cnerium::store
{
  namespace
  {
    [[nodiscard]] bool valid_status_code(int value) noexcept
    {
      return value >= 100 && value <= 599;
    }
  } // namespace

  StoredResponse::StoredResponse(
      int status_code,
      std::string body,
      std::string content_type)
      : status_code_(status_code),
        body_(std::move(body)),
        content_type_(std::move(content_type))
  {
  }

  StoredResponse StoredResponse::json(
      int status_code,
      const support::Json &body)
  {
    return StoredResponse{
        status_code,
        support::dump(body),
        "application/json; charset=utf-8"};
  }

  StoredResponse StoredResponse::text(
      int status_code,
      std::string body)
  {
    return StoredResponse{
        status_code,
        std::move(body),
        "text/plain; charset=utf-8"};
  }

  StoredResponse StoredResponse::from_json_text(std::string_view text)
  {
    return from_json(support::parse(text));
  }

  StoredResponse StoredResponse::from_json(const support::Json &value)
  {
    StoredResponse response{};

    response.set_status_code(
        support::int_or(value, "status_code", 200));

    response.set_body(
        support::string_or(value, "body", ""));

    response.set_content_type(
        support::string_or(
            value,
            "content_type",
            "application/json; charset=utf-8"));

    return response;
  }

  int StoredResponse::status_code() const noexcept
  {
    return status_code_;
  }

  const std::string &StoredResponse::body() const noexcept
  {
    return body_;
  }

  const std::string &StoredResponse::content_type() const noexcept
  {
    return content_type_;
  }

  void StoredResponse::set_status_code(int value) noexcept
  {
    status_code_ = value;
  }

  void StoredResponse::set_body(std::string value)
  {
    body_ = std::move(value);
  }

  void StoredResponse::set_content_type(std::string value)
  {
    content_type_ = std::move(value);
  }

  bool StoredResponse::has_body() const noexcept
  {
    return !body_.empty();
  }

  bool StoredResponse::has_content_type() const noexcept
  {
    return !content_type_.empty();
  }

  bool StoredResponse::is_valid() const noexcept
  {
    return valid_status_code(status_code_);
  }

  bool StoredResponse::valid() const noexcept
  {
    return is_valid();
  }

  support::Json StoredResponse::to_json() const
  {
    return support::object({
        {"status_code", support::Json(status_code_)},
        {"body", support::Json(body_)},
        {"content_type", support::Json(content_type_)},
    });
  }

  std::string StoredResponse::to_json_text() const
  {
    return support::dump(to_json());
  }

  void StoredResponse::clear() noexcept
  {
    status_code_ = 200;
    body_.clear();
    content_type_ = "application/json; charset=utf-8";
  }

} // namespace cnerium::store
