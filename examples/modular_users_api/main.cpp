/**
 * @file main.cpp
 * @brief Modular users API example entry point
 */

#include "UserController.hpp"
#include "UserRepository.hpp"
#include "UserService.hpp"

#include <cnerium/app/app.hpp>
#include <cnerium/http/Status.hpp>
#include <iostream>

int main()
{
  using namespace example::users;

  cnerium::app::App app;

  UserRepository repository;
  UserService service(repository);
  UserController controller(service);

  controller.register_routes(app);

  app.set_not_found_handler([](cnerium::server::Context &ctx)
                            {
                              auto &res = ctx.response();
                              res.set_status(cnerium::http::Status::not_found);
                              res.json({
                                  {"ok", false},
                                  {"error", "route not found"},
                              }); });

  app.set_error_handler([](cnerium::server::Context &ctx, const std::exception &ex)
                        {
                          auto &res = ctx.response();
                          res.set_status(cnerium::http::Status::internal_server_error);
                          res.json({
                              {"ok", false},
                              {"error", "internal server error"},
                              {"message", ex.what()},
                          }); });

  app.listen("127.0.0.1", 8080, []()
             { std::cout << "Modular Users API is ready.\n"; });
}
