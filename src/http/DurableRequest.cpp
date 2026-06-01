/**
 *
 *  @file DurableRequest.cpp
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

#include <cnerium/http/DurableRequest.hpp>

namespace cnerium::http
{
  DurableRequest::DurableRequest(const vix::http::Request &request) noexcept
      : request_(&request)
  {
  }

  std::string DurableRequest::method() const
  {
    return request_ == nullptr ? std::string{} : request_->method();
  }

  std::string DurableRequest::target() const
  {
    return request_ == nullptr ? std::string{} : request_->target();
  }

  std::string DurableRequest::path() const
  {
    return request_ == nullptr ? std::string{} : request_->path();
  }

  const std::string &DurableRequest::body() const noexcept
  {
    static const std::string empty{};

    if (request_ == nullptr)
    {
      return empty;
    }

    return request_->body();
  }

  std::string DurableRequest::header(std::string_view name) const
  {
    if (request_ == nullptr || name.empty())
    {
      return {};
    }

    return request_->header(std::string{name});
  }

  bool DurableRequest::has_header(std::string_view name) const
  {
    return !header(name).empty();
  }

  reliability::IdempotencyKey DurableRequest::idempotency_key() const
  {
    return reliability::IdempotencyKey{
        header("Idempotency-Key")};
  }

  std::string DurableRequest::idempotency_key_value() const
  {
    return idempotency_key().value();
  }

  reliability::RequestHash DurableRequest::request_hash() const
  {
    return reliability::RequestHash::from_body(body());
  }

  support::Json DurableRequest::json() const
  {
    return support::parse(body());
  }

  std::optional<support::Json> DurableRequest::try_json() const noexcept
  {
    return support::try_parse(body());
  }

  std::string DurableRequest::param(std::string_view name) const
  {
    if (request_ == nullptr || name.empty())
    {
      return {};
    }

    return request_->param(std::string{name});
  }

  std::string DurableRequest::query(std::string_view name) const
  {
    if (request_ == nullptr || name.empty())
    {
      return {};
    }

    return request_->query_value(std::string{name});
  }

  const vix::http::Request &DurableRequest::native() const noexcept
  {
    return *request_;
  }

} // namespace cnerium::http
