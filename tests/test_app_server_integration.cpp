#include <cassert>

#include <cnerium/app/app.hpp>
#include <cnerium/http/Method.hpp>
#include <cnerium/http/Request.hpp>
#include <cnerium/http/Status.hpp>

int main()
{
  using namespace cnerium::app;

  App app;

  app.get("/ping", [](AppContext &ctx)
          { ctx.status(cnerium::http::Status::ok)
                .header("X-App", "Cnerium")
                .text("pong"); });

  cnerium::http::Request req;
  req.set_method(cnerium::http::Method::Get);
  req.set_path("/ping");

  auto res = app.server().handle(std::move(req));

  assert(res.status() == cnerium::http::Status::ok);
  assert(res.header("X-App") == "Cnerium");
  assert(res.body() == "pong");

  return 0;
}
