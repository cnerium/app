/**
 *
 *  @file main.cpp
 *  @author Softadastra
 *
 *  Copyright 2026, Softadastra.
 *  All rights reserved.
 *  https://github.com/softadastra/cnerium.git
 *
 *  Licensed under the MIT License.
 *
 *  Cnerium
 *
 */

#include <cnerium/cnerium.hpp>

#include <string>

int main()
{
  cnerium::app::AppConfig config =
      cnerium::app::AppConfig::development();

  config.set_name("durable-orders-realtime");
  config.set_data_dir("data/cnerium");
  config.set_node_id("durable-orders-realtime-node");
  config.set_vix_config_path("vix.json");
  config.enable_realtime("/ws", "0.0.0.0", 9090);

  cnerium::App app{std::move(config)};

  app.durable_post(
      "/orders",
      "orders.create",
      [&app](cnerium::DurableRequest &request)
      {
        const auto body = request.json();

        const std::string product_id =
            cnerium::support::string_or(body, "product_id", "");

        const int quantity =
            cnerium::support::int_or(body, "quantity", 0);

        if (product_id.empty())
        {
          return cnerium::DurableResponse::bad_request(
              "Missing required field: product_id");
        }

        if (quantity <= 0)
        {
          return cnerium::DurableResponse::bad_request(
              "Field quantity must be greater than zero");
        }

        const std::string order_id =
            "ord_" + request.idempotency_key_value();

        app.emit(
            "order.created",
            cnerium::support::object({
                {"order_id", cnerium::Json(order_id)},
                {"product_id", cnerium::Json(product_id)},
                {"quantity", cnerium::Json(quantity)},
            }));

        return cnerium::created(
            cnerium::support::object({
                {"ok", cnerium::Json(true)},
                {"order_id", cnerium::Json(order_id)},
                {"product_id", cnerium::Json(product_id)},
                {"quantity", cnerium::Json(quantity)},
            }));
      });

  return app.run();
}
