/**
 *
 *  @file VixHttp.hpp
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

#ifndef CNERIUM_ADAPTERS_VIX_HTTP_HPP
#define CNERIUM_ADAPTERS_VIX_HTTP_HPP

#include <string_view>

#include <vix/http/Request.hpp>
#include <vix/http/ResponseWrapper.hpp>

#include <cnerium/http/DurableResponse.hpp>
#include <cnerium/http/DurableRoute.hpp>

namespace cnerium::adapters
{
  /**
   * @brief Adapter between Cnerium durable HTTP responses and Vix HTTP.
   *
   * VixHttp is intentionally small.
   *
   * It does not create a server, router, session, middleware engine, or HTTP
   * parser. Vix already owns those responsibilities.
   *
   * This adapter only writes Cnerium DurableResponse objects into
   * vix::http::ResponseWrapper.
   */
  class VixHttp
  {
  public:
    /**
     * @brief Writes a Cnerium durable response to a Vix response wrapper.
     *
     * @param response Cnerium durable response.
     * @param output Vix response wrapper.
     */
    static void write_response(
        const http::DurableResponse &response,
        vix::http::ResponseWrapper &output);

    /**
     * @brief Executes a durable route and writes its response to Vix.
     *
     * @param route Cnerium durable route.
     * @param request Vix HTTP request.
     * @param output Vix response wrapper.
     */
    static void execute_route(
        http::DurableRoute &route,
        const vix::http::Request &request,
        vix::http::ResponseWrapper &output);

    /**
     * @brief Returns true if a content type is JSON-like.
     *
     * @param content_type Content-Type value.
     * @return true if the content type represents JSON.
     */
    [[nodiscard]] static bool is_json_content_type(
        std::string_view content_type) noexcept;

    /**
     * @brief Returns true if a content type is text-like.
     *
     * @param content_type Content-Type value.
     * @return true if the content type represents text.
     */
    [[nodiscard]] static bool is_text_content_type(
        std::string_view content_type) noexcept;
  };

} // namespace cnerium::adapters

#endif // CNERIUM_ADAPTERS_VIX_HTTP_HPP
