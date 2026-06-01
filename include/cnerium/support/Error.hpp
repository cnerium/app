/**
 *
 *  @file Error.hpp
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
#ifndef CNERIUM_SUPPORT_ERROR_HPP
#define CNERIUM_SUPPORT_ERROR_HPP

#include <string>
#include <string_view>

namespace cnerium::support
{
  /**
   * @brief Public Cnerium error object.
   *
   * Error represents a stable developer-facing error returned by Cnerium
   * components.
   *
   * Cnerium does not expose raw Vix or Softadastra SDK errors directly from its
   * public API. Lower-level errors are converted into this type before reaching
   * application code.
   */
  class Error
  {
  public:
    /**
     * @brief Stable Cnerium error code.
     *
     * These codes form the public error surface of Cnerium.
     */
    enum class Code
    {
      None,
      Unknown,
      InvalidArgument,
      InvalidState,
      NotFound,
      AlreadyExists,
      IoError,
      StoreError,
      ReliabilityError,
      IdempotencyError,
      ReplayError,
      HttpError,
      RealtimeError,
      VixError,
      SoftadastraError,
      InternalError
    };

    /**
     * @brief Creates an empty success-like error.
     */
    Error() = default;

    /**
     * @brief Creates a Cnerium error.
     *
     * @param code Stable error code.
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     */
    Error(
        Code code,
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a success-like error.
     *
     * @return Error with Code::None.
     */
    [[nodiscard]] static Error none();

    /**
     * @brief Creates an error from explicit fields.
     *
     * @param code Stable error code.
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error make(
        Code code,
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an unknown error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error unknown(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an invalid argument error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error invalid_argument(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an invalid state error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error invalid_state(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a not found error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error not_found(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an already exists error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error already_exists(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an I/O error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error io_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a store error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error store_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a reliability error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error reliability_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an idempotency error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error idempotency_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a replay protection error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error replay_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an HTTP integration error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error http_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a realtime integration error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error realtime_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a Vix integration error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error vix_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a Softadastra SDK integration error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error softadastra_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an internal Cnerium error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Cnerium error.
     */
    [[nodiscard]] static Error internal_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Returns the stable error code.
     *
     * @return Error code.
     */
    [[nodiscard]] Code code() const noexcept;

    /**
     * @brief Returns the developer-facing error message.
     *
     * @return Error message.
     */
    [[nodiscard]] const std::string &message() const noexcept;

    /**
     * @brief Returns the optional diagnostic context.
     *
     * @return Error context.
     */
    [[nodiscard]] const std::string &context() const noexcept;

    /**
     * @brief Returns true when this object represents success.
     *
     * @return true if the error code is Code::None.
     */
    [[nodiscard]] bool ok() const noexcept;

    /**
     * @brief Returns true when this object represents an error.
     *
     * @return true if the error code is not Code::None.
     */
    [[nodiscard]] bool has_error() const noexcept;

    /**
     * @brief Returns true when diagnostic context is available.
     *
     * @return true if context is not empty.
     */
    [[nodiscard]] bool has_context() const noexcept;

    /**
     * @brief Clears the error and turns it into a success-like object.
     */
    void clear() noexcept;

    /**
     * @brief Converts an error code to a stable string.
     *
     * @param code Error code.
     * @return String representation.
     */
    [[nodiscard]] static std::string_view to_string(Code code) noexcept;

    /**
     * @brief Returns the current error code as a stable string.
     *
     * @return String representation of the current code.
     */
    [[nodiscard]] std::string_view code_string() const noexcept;

  private:
    Code code_{Code::None};
    std::string message_{};
    std::string context_{};
  };

} // namespace cnerium::support

#endif // CNERIUM_SUPPORT_ERROR_HPP
