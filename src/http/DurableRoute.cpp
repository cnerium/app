/**
 *
 *  @file DurableRoute.cpp
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

#include <cnerium/http/DurableRoute.hpp>

#include <exception>
#include <utility>

namespace cnerium::http
{
  DurableRoute::DurableRoute(
      std::string operation,
      store::Store &store,
      DurableHandler handler)
      : operation_(std::move(operation)),
        store_(&store),
        handler_(std::move(handler))
  {
  }

  DurableRoute::DurableRoute(DurableRoute &&other) noexcept
      : operation_(std::move(other.operation_)),
        store_(other.store_),
        handler_(std::move(other.handler_))
  {
    other.store_ = nullptr;
  }

  DurableRoute &DurableRoute::operator=(DurableRoute &&other) noexcept
  {
    if (this != &other)
    {
      operation_ = std::move(other.operation_);
      store_ = other.store_;
      handler_ = std::move(other.handler_);

      other.store_ = nullptr;
    }

    return *this;
  }

  DurableResponse DurableRoute::execute(
      const vix::http::Request &request)
  {
    if (!is_valid())
    {
      return DurableResponse::internal_error(
          "Durable route is not configured correctly");
    }

    DurableRequest durable_request{request};

    const auto key = durable_request.idempotency_key();

    if (!key.is_valid())
    {
      return DurableResponse::bad_request(
          "Missing required Idempotency-Key header");
    }

    const auto hash = durable_request.request_hash();

    reliability::Idempotency idempotency{*store_};

    const auto result =
        idempotency.check_hash(
            operation_,
            key,
            hash);

    if (result.is_invalid())
    {
      return DurableResponse::bad_request(
          "Invalid durable request");
    }

    if (result.is_conflict())
    {
      return DurableResponse::conflict(
          "Idempotency-Key was reused with a different request body");
    }

    if (result.should_replay())
    {
      return DurableResponse::from_stored(result.response());
    }

    DurableResponse response{};

    try
    {
      response = handler_(durable_request);
    }
    catch (const std::exception &exception)
    {
      return DurableResponse::internal_error(exception.what());
    }
    catch (...)
    {
      return DurableResponse::internal_error(
          "Unhandled exception in durable route handler");
    }

    if (!response.is_valid())
    {
      return DurableResponse::internal_error(
          "Durable route returned an invalid response");
    }

    const bool committed =
        idempotency.commit_hash(
            operation_,
            key,
            hash,
            response.to_stored_response());

    if (!committed)
    {
      return DurableResponse::internal_error(
          "Failed to persist durable response");
    }

    return response;
  }

  const std::string &DurableRoute::operation() const noexcept
  {
    return operation_;
  }

  bool DurableRoute::is_valid() const noexcept
  {
    return !operation_.empty() &&
           store_ != nullptr &&
           static_cast<bool>(handler_);
  }

  bool DurableRoute::valid() const noexcept
  {
    return is_valid();
  }

} // namespace cnerium::http
