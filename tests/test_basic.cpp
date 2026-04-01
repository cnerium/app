#include <app/app.hpp>
#include <iostream>

int main()
{
  auto nodes = app::make_chain(5);
  std::cout << "nodes=" << nodes.size() << "\n";
  return nodes.size() == 5 ? 0 : 1;
}
