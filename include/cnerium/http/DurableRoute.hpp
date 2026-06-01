/**
 *
 *  @file DurableRoute.hpp
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

#ifndef CNERIUM_HTTP_DURABLE_ROUTE_HPP
#define CNERIUM_HTTP_DURABLE_ROUTE_HPP

#include <string>
#include <string_view>

#include <vix/http/Request.hpp>

#include <cnerium/http/DurableHandler.hpp>
#include <cnerium/http/DurableRequest.hpp>
#include <cnerium/http/DurableResponse.hpp>
#include <cnerium/reliability/Idempotency.hpp>
#include <cnerium/store/Store.hpp>

namespace cnerium::http
{
  /**
   * @brief Durable route executor used by Cnerium.
   *
   * DurableRoute applies Cnerium reliability rules around a user handler.
   *
   * It does not own an HTTP server, router, session, or socket. Vix owns those.
   * This class only receives a Vix request and returns a DurableResponse.
   *
   * Flow:
   * - read Idempotency-Key
   * - compute request hash
   * - check replay protection
   * - replay stored response if possible
   * - reject unsafe replay with 409 Conflict
   * - execute user handler for new requests
   * - store the final response for future retries
   */
  class DurableRoute
  {
  public:
    /**
     * @brief Creates an empty invalid durable route.
     */
    DurableRoute() = default;

    /**
     * @brief Creates a durable route.
     *
     * @param operation Stable operation name.
     * @param store Store used by the reliability layer.
     * @param handler User-defined durable handler.
     */
    DurableRoute(
        std::string operation,
        store::Store &store,
        DurableHandler handler);

    DurableRoute(const DurableRoute &) = delete;
    DurableRoute &operator=(const DurableRoute &) = delete;

    /**
     * @brief Moves a durable route.
     */
    DurableRoute(DurableRoute &&other) noexcept;

    /**
     * @brief Move-assigns a durable route.
     */
    DurableRoute &operator=(DurableRoute &&other) noexcept;

    /**
     * @brief Executes the durable route logic for a Vix HTTP request.
     *
     * @param request Vix HTTP request.
     * @return Durable response.
     */
    [[nodiscard]] DurableResponse execute(
        const vix::http::Request &request);

    /**
     * @brief Returns the stable operation name.
     *
     * @return Operation name.
     */
    [[nodiscard]] const std::string &operation() const noexcept;

    /**
     * @brief Returns true if the route has a usable operation, store, and handler.
     *
     * @return true if the route is ready.
     */
    [[nodiscard]] bool is_valid() const noexcept;

    /**
     * @brief Backward-compatible alias for is_valid().
     *
     * @return true if the route is valid.
     */
    [[nodiscard]] bool valid() const noexcept;

  private:
    std::string operation_{};
    store::Store *store_{nullptr};
    DurableHandler handler_{};
  };

} // namespace cnerium::http

#endif // CNERIUM_HTTP_DURABLE_ROUTE_HPP
