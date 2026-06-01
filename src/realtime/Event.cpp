/**
 *
 *  @file Event.cpp
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

#include <cnerium/realtime/Event.hpp>

#include <utility>

namespace cnerium::realtime
{
  Event::Event(
      std::string type,
      EventPayload payload)
      : type_(std::move(type)),
        payload_(std::move(payload))
  {
  }

  Event Event::make(
      std::string type,
      EventPayload payload)
  {
    return Event{
        std::move(type),
        std::move(payload)};
  }

  Event Event::from(
      std::string type,
      EventPayload payload)
  {
    return make(
        std::move(type),
        std::move(payload));
  }

  const std::string &Event::type() const noexcept
  {
    return type_;
  }

  const EventPayload &Event::payload() const noexcept
  {
    return payload_;
  }

  EventPayload &Event::payload() noexcept
  {
    return payload_;
  }

  void Event::set_type(std::string value)
  {
    type_ = std::move(value);
  }

  void Event::set_payload(EventPayload value)
  {
    payload_ = std::move(value);
  }

  bool Event::is_valid() const noexcept
  {
    return !type_.empty();
  }

  bool Event::valid() const noexcept
  {
    return is_valid();
  }

  bool Event::empty() const noexcept
  {
    return type_.empty();
  }

  support::Json Event::to_json() const
  {
    return support::object({
        {"type", support::Json(type_)},
        {"payload", payload_},
    });
  }

  std::string Event::to_json_text() const
  {
    return support::dump(to_json());
  }

  void Event::clear() noexcept
  {
    type_.clear();
    payload_ = support::object();
  }

} // namespace cnerium::realtime
