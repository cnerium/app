#include <cnerium/app/app.hpp>

#include <iostream>

using namespace cnerium::app;

int main()
{
  App app;

  app.get("/", [](AppContext &ctx)
          { ctx.text("Hello with runtime"); });

  /**
   * Low-level runtime access.
   *
   * Demonstrates that App still exposes the underlying runtime
   * when background task scheduling is needed.
   */
  app.runtime().post([]()
                     { std::cout << "[Runtime] background task\n"; });

  app.listen("127.0.0.1", 8080, []()
             { std::cout << "App is ready.\n"; });
}
