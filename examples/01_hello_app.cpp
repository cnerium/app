#include <cnerium/app/app.hpp>

using namespace cnerium::app;

int main()
{
  App app;

  app.get("/", [](AppContext &ctx)
          { ctx.text("Hello from Cnerium"); });

  app.listen("127.0.0.1", 8080);
}
