#include <cassert>

#include <cnerium/app/app.hpp>

int main()
{
  using namespace cnerium::app;

  AppConfig config;
  assert(config.valid());

  config.host = "0.0.0.0";
  config.port = 9090;
  config.thread_count = 4;
  config.backlog = 256;

  assert(config.host == "0.0.0.0");
  assert(config.port == 9090);
  assert(config.thread_count == 4);
  assert(config.backlog == 256);

  config.reset();

  assert(config.host == "127.0.0.1");
  assert(config.port == 8080);
  assert(config.backlog == 128);
  assert(config.read_buffer_size == 8 * 1024);
  assert(config.max_request_body_size == 1024 * 1024);

  return 0;
}
