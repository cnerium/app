#include <cnerium/app/app.hpp>

using namespace cnerium::app;

int main()
{
  App app;

  app.use([](auto &ctx, auto next)
          {
    ctx.response().set_header("X-App", "Cnerium");
    next(); });

  app.get("/", [](AppContext &ctx)
          { ctx.text("middleware works"); });

  app.listen("127.0.0.1", 8080);
}
