#include <cassert>

#include <cnerium/app/app.hpp>

int main()
{
  using namespace cnerium::app;

  App app;
  assert(app.config().valid());

  AppHandler handler = [](AppContext &) {};
  assert(static_cast<bool>(handler));

  return 0;
}
