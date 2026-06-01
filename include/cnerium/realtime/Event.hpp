/**
 *
 *  @file Event.hpp
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

#ifndef CNERIUM_REALTIME_EVENT_HPP
#define CNERIUM_REALTIME_EVENT_HPP

#include <string>
#include <string_view>

#include <cnerium/realtime/EventPayload.hpp>
#include <cnerium/support/Json.hpp>

namespace cnerium::realtime
{
  /**
   * @brief Realtime event emitted by a Cnerium application.
   *
   * Event represents a typed application event that can be sent through the
   * Vix WebSocket runtime.
   *
   * Cnerium does not implement its own WebSocket protocol. It only creates a
   * stable event model that can be converted to the typed JSON messages used by
   * Vix WebSocket.
   */
  class Event
  {
  public:
    /**
     * @brief Creates an empty invalid event.
     */
    Event() = default;

    /**
     * @brief Creates an event.
     *
     * @param type Event type.
     * @param payload Event payload.
     */
    Event(
        std::string type,
        EventPayload payload = support::object());

    /**
     * @brief Creates an event.
     *
     * @param type Event type.
     * @param payload Event payload.
     * @return Event.
     */
    [[nodiscard]] static Event make(
        std::string type,
        EventPayload payload = support::object());

    /**
     * @brief Creates an event from a type and payload.
     *
     * Alias for make().
     *
     * @param type Event type.
     * @param payload Event payload.
     * @return Event.
     */
    [[nodiscard]] static Event from(
        std::string type,
        EventPayload payload = support::object());

    /**
     * @brief Returns the event type.
     *
     * @return Event type.
     */
    [[nodiscard]] const std::string &type() const noexcept;

    /**
     * @brief Returns the event payload.
     *
     * @return Event payload.
     */
    [[nodiscard]] const EventPayload &payload() const noexcept;

    /**
     * @brief Returns the event payload.
     *
     * @return Mutable event payload.
     */
    [[nodiscard]] EventPayload &payload() noexcept;

    /**
     * @brief Sets the event type.
     *
     * @param value Event type.
     */
    void set_type(std::string value);

    /**
     * @brief Sets the event payload.
     *
     * @param value Event payload.
     */
    void set_payload(EventPayload value);

    /**
     * @brief Returns true if the event has a non-empty type.
     *
     * @return true if the event type is not empty.
     */
    [[nodiscard]] bool is_valid() const noexcept;

    /**
     * @brief Backward-compatible alias for is_valid().
     *
     * @return true if the event is valid.
     */
    [[nodiscard]] bool valid() const noexcept;

    /**
     * @brief Returns true if the event type is empty.
     *
     * @return true if the event type is empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Converts the event to JSON.
     *
     * Output format:
     * {
     *   "type": "...",
     *   "payload": { ... }
     * }
     *
     * @return JSON object.
     */
    [[nodiscard]] support::Json to_json() const;

    /**
     * @brief Serializes the event to compact JSON text.
     *
     * @return JSON text.
     */
    [[nodiscard]] std::string to_json_text() const;

    /**
     * @brief Clears the event.
     */
    void clear() noexcept;

  private:
    std::string type_{};
    EventPayload payload_{support::object()};
  };

} // namespace cnerium::realtime

namespace cnerium
{
  /**
   * @brief Public convenience alias for realtime events.
   */
  using Event = realtime::Event;

} // namespace cnerium

#endif // CNERIUM_REALTIME_EVENT_HPP
