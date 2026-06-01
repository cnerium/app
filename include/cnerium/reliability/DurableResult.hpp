/**
 *
 *  @file DurableResult.hpp
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

#ifndef CNERIUM_RELIABILITY_DURABLE_RESULT_HPP
#define CNERIUM_RELIABILITY_DURABLE_RESULT_HPP

#include <cnerium/store/StoredResponse.hpp>

namespace cnerium::reliability
{
  /**
   * @brief Result of a durable request replay protection check.
   *
   * DurableResult tells the durable route execution layer what to do with an
   * incoming request:
   * - execute the user handler
   * - replay a previously stored response
   * - reject the request because the same idempotency key was reused with a
   *   different request body
   * - reject the request because the idempotency key is missing or invalid
   */
  class DurableResult
  {
  public:
    /**
     * @brief Action selected by the reliability layer.
     */
    enum class Action
    {
      Execute,
      Replay,
      Conflict,
      Invalid
    };

    /**
     * @brief Creates an invalid durable result.
     */
    DurableResult() = default;

    /**
     * @brief Creates a durable result from an action.
     *
     * @param action Selected action.
     */
    explicit DurableResult(Action action);

    /**
     * @brief Creates a replay result from a stored response.
     *
     * @param response Stored response to replay.
     * @return Durable result.
     */
    [[nodiscard]] static DurableResult replay(
        store::StoredResponse response);

    /**
     * @brief Creates an execute result.
     *
     * @return Durable result.
     */
    [[nodiscard]] static DurableResult execute();

    /**
     * @brief Creates a conflict result.
     *
     * @return Durable result.
     */
    [[nodiscard]] static DurableResult conflict();

    /**
     * @brief Creates an invalid result.
     *
     * @return Durable result.
     */
    [[nodiscard]] static DurableResult invalid();

    /**
     * @brief Returns the selected action.
     *
     * @return Durable action.
     */
    [[nodiscard]] Action action() const noexcept;

    /**
     * @brief Returns the stored response.
     *
     * This is only meaningful when action() is Action::Replay.
     *
     * @return Stored response.
     */
    [[nodiscard]] const store::StoredResponse &response() const noexcept;

    /**
     * @brief Returns true if the request should execute the user handler.
     *
     * @return true if action is Execute.
     */
    [[nodiscard]] bool should_execute() const noexcept;

    /**
     * @brief Returns true if the saved response should be replayed.
     *
     * @return true if action is Replay.
     */
    [[nodiscard]] bool should_replay() const noexcept;

    /**
     * @brief Returns true if the request must be rejected as a conflict.
     *
     * @return true if action is Conflict.
     */
    [[nodiscard]] bool is_conflict() const noexcept;

    /**
     * @brief Returns true if the request is invalid.
     *
     * @return true if action is Invalid.
     */
    [[nodiscard]] bool is_invalid() const noexcept;

  private:
    Action action_{Action::Invalid};
    store::StoredResponse response_{};
  };

  inline DurableResult::DurableResult(Action action)
      : action_(action)
  {
  }

  inline DurableResult DurableResult::replay(
      store::StoredResponse response)
  {
    DurableResult result{Action::Replay};
    result.response_ = std::move(response);
    return result;
  }

  inline DurableResult DurableResult::execute()
  {
    return DurableResult{Action::Execute};
  }

  inline DurableResult DurableResult::conflict()
  {
    return DurableResult{Action::Conflict};
  }

  inline DurableResult DurableResult::invalid()
  {
    return DurableResult{Action::Invalid};
  }

  inline DurableResult::Action DurableResult::action() const noexcept
  {
    return action_;
  }

  inline const store::StoredResponse &DurableResult::response() const noexcept
  {
    return response_;
  }

  inline bool DurableResult::should_execute() const noexcept
  {
    return action_ == Action::Execute;
  }

  inline bool DurableResult::should_replay() const noexcept
  {
    return action_ == Action::Replay;
  }

  inline bool DurableResult::is_conflict() const noexcept
  {
    return action_ == Action::Conflict;
  }

  inline bool DurableResult::is_invalid() const noexcept
  {
    return action_ == Action::Invalid;
  }

} // namespace cnerium::reliability

#endif // CNERIUM_RELIABILITY_DURABLE_RESULT_HPP
