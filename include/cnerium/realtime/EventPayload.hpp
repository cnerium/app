/**
 *
 *  @file EventPayload.hpp
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

#ifndef CNERIUM_REALTIME_EVENT_PAYLOAD_HPP
#define CNERIUM_REALTIME_EVENT_PAYLOAD_HPP

#include <cnerium/support/Json.hpp>

namespace cnerium::realtime
{
  /**
   * @brief Payload carried by a realtime Cnerium event.
   *
   * EventPayload uses the Cnerium JSON type, which is backed by Vix JSON.
   *
   * This keeps realtime events compatible with the Vix WebSocket typed message
   * model while avoiding a separate JSON representation inside Cnerium.
   */
  using EventPayload = support::Json;

} // namespace cnerium::realtime

namespace cnerium
{
  /**
   * @brief Public convenience alias for realtime event payloads.
   */
  using EventPayload = realtime::EventPayload;

} // namespace cnerium

#endif // CNERIUM_REALTIME_EVENT_PAYLOAD_HPP
