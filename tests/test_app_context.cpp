#include <cassert>
#include <string>

#include <cnerium/app/app.hpp>
#include <cnerium/server/Context.hpp>
#include <cnerium/http/Method.hpp>
#include <cnerium/http/Status.hpp>

int main()
{
  cnerium::server::Context server_ctx;
  server_ctx.request().set_method(cnerium::http::Method::Get);
  server_ctx.request().set_path("/users/42");
  server_ctx.params().set("id", "42");

  cnerium::app::AppContext ctx(server_ctx);

  assert(ctx.request().path() == "/users/42");
  assert(ctx.params().get("id") == "42");

  ctx.status(cnerium::http::Status::ok);
  ctx.header("X-Test", "yes");
  ctx.text("hello");

  assert(ctx.response().status() == cnerium::http::Status::ok);
  assert(ctx.response().header("X-Test") == "yes");
  assert(ctx.response().body() == "hello");

  auto &raw = ctx.raw();
  assert(&raw == &server_ctx);

  return 0;
}
