#include <cnerium/app/app.hpp>

#include <string>

using namespace cnerium::app;

int main()
{
  App app;

  app.get("/", [](AppContext &ctx)
          { ctx.text("home"); });

  app.get("/users/:id", [](AppContext &ctx)
          { ctx.text(std::string(ctx.param("id"))); });

  app.post("/users", [](AppContext &ctx)
           { ctx.text("user created"); });

  app.listen("127.0.0.1", 8080);
}
