/**
 *
 *  @file VixHttp.cpp
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

#include <cnerium/adapters/VixHttp.hpp>
#include <cnerium/support/String.hpp>

namespace cnerium::adapters
{
  void VixHttp::write_response(
      const http::DurableResponse &response,
      vix::http::ResponseWrapper &output)
  {
    if (!response.is_valid())
    {
      output.status(500)
          .type("application/json; charset=utf-8")
          .send(R"({"error":"Invalid durable response"})");
      return;
    }

    output.status(response.status_code());

    if (response.has_content_type())
    {
      output.type(response.content_type());
    }

    output.header("X-Content-Type-Options", "nosniff");

    if (!response.has_body())
    {
      output.send();
      return;
    }

    output.send(response.body());
  }

  void VixHttp::execute_route(
      http::DurableRoute &route,
      const vix::http::Request &request,
      vix::http::ResponseWrapper &output)
  {
    write_response(
        route.execute(request),
        output);
  }

  bool VixHttp::is_json_content_type(
      std::string_view content_type) noexcept
  {
    return support::contains_ignore_case(
               content_type,
               "application/json") ||
           support::contains_ignore_case(
               content_type,
               "+json");
  }

  bool VixHttp::is_text_content_type(
      std::string_view content_type) noexcept
  {
    return support::starts_with_ignore_case(
               content_type,
               "text/") ||
           is_json_content_type(content_type);
  }

} // namespace cnerium::adapters
