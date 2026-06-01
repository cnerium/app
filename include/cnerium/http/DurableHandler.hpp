/**
 *
 *  @file DurableHandler.hpp
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

#ifndef CNERIUM_HTTP_DURABLE_HANDLER_HPP
#define CNERIUM_HTTP_DURABLE_HANDLER_HPP

#include <functional>

#include <cnerium/http/DurableRequest.hpp>
#include <cnerium/http/DurableResponse.hpp>

namespace cnerium::http
{
  /**
   * @brief User-defined durable route handler.
   *
   * DurableHandler is the function type executed by Cnerium after the
   * reliability layer decides that a request is safe to process.
   *
   * The handler receives a DurableRequest and returns a DurableResponse.
   */
  using DurableHandler = std::function<DurableResponse(DurableRequest &)>;

} // namespace cnerium::http

namespace cnerium
{
  /**
   * @brief Public convenience alias for durable route handlers.
   */
  using DurableHandler = http::DurableHandler;

} // namespace cnerium

#endif // CNERIUM_HTTP_DURABLE_HANDLER_HPP
