#include <cassert>
#include <atomic>
#include <chrono>
#include <thread>

#include <cnerium/app/app.hpp>

int main()
{
  using namespace cnerium::app;

  App app;
  app.start();

  assert(app.runtime().running());

  std::atomic<int> counter{0};

  const bool ok = app.runtime().post([&counter]()
                                     { ++counter; });

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  app.stop();
  app.join();

  assert(ok);
  assert(counter.load() == 1);

  return 0;
}
