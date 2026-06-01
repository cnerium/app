/**
 *
 *  @file Error.cpp
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
#include <cnerium/support/Error.hpp>
#include <utility>

namespace cnerium::support
{
  Error::Error(
      Code code,
      std::string message,
      std::string context)
      : code_(code),
        message_(std::move(message)),
        context_(std::move(context))
  {
  }

  Error Error::none()
  {
    return Error{};
  }

  Error Error::make(
      Code code,
      std::string message,
      std::string context)
  {
    return Error{
        code,
        std::move(message),
        std::move(context)};
  }

  Error Error::unknown(
      std::string message,
      std::string context)
  {
    return Error{
        Code::Unknown,
        std::move(message),
        std::move(context)};
  }

  Error Error::invalid_argument(
      std::string message,
      std::string context)
  {
    return Error{
        Code::InvalidArgument,
        std::move(message),
        std::move(context)};
  }

  Error Error::invalid_state(
      std::string message,
      std::string context)
  {
    return Error{
        Code::InvalidState,
        std::move(message),
        std::move(context)};
  }

  Error Error::not_found(
      std::string message,
      std::string context)
  {
    return Error{
        Code::NotFound,
        std::move(message),
        std::move(context)};
  }

  Error Error::already_exists(
      std::string message,
      std::string context)
  {
    return Error{
        Code::AlreadyExists,
        std::move(message),
        std::move(context)};
  }

  Error Error::io_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::IoError,
        std::move(message),
        std::move(context)};
  }

  Error Error::store_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::StoreError,
        std::move(message),
        std::move(context)};
  }

  Error Error::reliability_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::ReliabilityError,
        std::move(message),
        std::move(context)};
  }

  Error Error::idempotency_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::IdempotencyError,
        std::move(message),
        std::move(context)};
  }

  Error Error::replay_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::ReplayError,
        std::move(message),
        std::move(context)};
  }

  Error Error::http_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::HttpError,
        std::move(message),
        std::move(context)};
  }

  Error Error::realtime_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::RealtimeError,
        std::move(message),
        std::move(context)};
  }

  Error Error::vix_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::VixError,
        std::move(message),
        std::move(context)};
  }

  Error Error::softadastra_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::SoftadastraError,
        std::move(message),
        std::move(context)};
  }

  Error Error::internal_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::InternalError,
        std::move(message),
        std::move(context)};
  }

  Error::Code Error::code() const noexcept
  {
    return code_;
  }

  const std::string &Error::message() const noexcept
  {
    return message_;
  }

  const std::string &Error::context() const noexcept
  {
    return context_;
  }

  bool Error::ok() const noexcept
  {
    return code_ == Code::None;
  }

  bool Error::has_error() const noexcept
  {
    return code_ != Code::None;
  }

  bool Error::has_context() const noexcept
  {
    return !context_.empty();
  }

  void Error::clear() noexcept
  {
    code_ = Code::None;
    message_.clear();
    context_.clear();
  }

  std::string_view Error::to_string(Code code) noexcept
  {
    switch (code)
    {
    case Code::None:
      return "none";

    case Code::Unknown:
      return "unknown";

    case Code::InvalidArgument:
      return "invalid_argument";

    case Code::InvalidState:
      return "invalid_state";

    case Code::NotFound:
      return "not_found";

    case Code::AlreadyExists:
      return "already_exists";

    case Code::IoError:
      return "io_error";

    case Code::StoreError:
      return "store_error";

    case Code::ReliabilityError:
      return "reliability_error";

    case Code::IdempotencyError:
      return "idempotency_error";

    case Code::ReplayError:
      return "replay_error";

    case Code::HttpError:
      return "http_error";

    case Code::RealtimeError:
      return "realtime_error";

    case Code::VixError:
      return "vix_error";

    case Code::SoftadastraError:
      return "softadastra_error";

    case Code::InternalError:
      return "internal_error";
    }

    return "unknown";
  }

  std::string_view Error::code_string() const noexcept
  {
    return to_string(code_);
  }

} // namespace cnerium::support
