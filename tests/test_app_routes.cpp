#include <cassert>
#include <string>

#include <cnerium/app/app.hpp>
#include <cnerium/http/Method.hpp>
#include <cnerium/http/Request.hpp>
#include <cnerium/http/Status.hpp>

int main()
{
  using namespace cnerium::app;

  App app;

  app.get("/", [](AppContext &ctx)
          { ctx.text("home"); });

  app.get("/users/:id", [](AppContext &ctx)
          { ctx.text(std::string(ctx.params().get("id"))); });

  {
    cnerium::http::Request req;
    req.set_method(cnerium::http::Method::Get);
    req.set_path("/");

    auto res = app.server().handle(std::move(req));
    assert(res.status() == cnerium::http::Status::ok);
    assert(res.body() == "home");
  }

  {
    cnerium::http::Request req;
    req.set_method(cnerium::http::Method::Get);
    req.set_path("/users/42");

    auto res = app.server().handle(std::move(req));
    assert(res.status() == cnerium::http::Status::ok);
    assert(res.body() == "42");
  }

  return 0;
}
