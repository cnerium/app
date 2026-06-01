/**
 *
 *  @file DurablePostTests.cpp
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

#include <cnerium/http/DurableRoute.hpp>
#include <cnerium/http/DurableResponse.hpp>
#include <cnerium/store/Store.hpp>

#include <vix/http/Request.hpp>

#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

namespace
{
  [[nodiscard]] vix::http::Request make_request(
      std::string body,
      std::string idempotency_key = "order-123")
  {
    vix::http::Request request;

    request.set_method("POST");
    request.set_target("/orders");
    request.set_body(std::move(body));

    if (!idempotency_key.empty())
    {
      request.set_header("Idempotency-Key", std::move(idempotency_key));
    }

    request.set_header("Content-Type", "application/json");

    return request;
  }

  [[nodiscard]] cnerium::http::DurableResponse create_order_response(
      cnerium::http::DurableRequest &request)
  {
    const auto body = request.json();

    const std::string product_id =
        cnerium::support::string_or(body, "product_id", "");

    const int quantity =
        cnerium::support::int_or(body, "quantity", 0);

    return cnerium::created(
        cnerium::support::object({
            {"ok", cnerium::Json(true)},
            {"order_id", cnerium::Json("ord_" + request.idempotency_key_value())},
            {"product_id", cnerium::Json(product_id)},
            {"quantity", cnerium::Json(quantity)},
        }));
  }
} // namespace

TEST(DurablePostTests, MissingIdempotencyKeyReturnsBadRequest)
{
  cnerium::store::Store store;

  ASSERT_TRUE(store.open());

  cnerium::http::DurableRoute route{
      "orders.create",
      store,
      create_order_response};

  const auto request =
      make_request(
          R"({"product_id":"p1","quantity":2})",
          "");

  const auto response = route.execute(request);

  EXPECT_EQ(response.status_code(), 400);
  EXPECT_TRUE(response.body().find("Idempotency-Key") != std::string::npos);
}

TEST(DurablePostTests, FirstRequestExecutesHandler)
{
  cnerium::store::Store store;

  ASSERT_TRUE(store.open());

  int executed_count = 0;

  cnerium::http::DurableRoute route{
      "orders.create",
      store,
      [&executed_count](cnerium::http::DurableRequest &request)
      {
        ++executed_count;
        return create_order_response(request);
      }};

  const auto request =
      make_request(
          R"({"product_id":"p1","quantity":2})",
          "first-request");

  const auto response = route.execute(request);

  EXPECT_EQ(response.status_code(), 201);
  EXPECT_EQ(executed_count, 1);
  EXPECT_TRUE(response.body().find("ord_first-request") != std::string::npos);
}

TEST(DurablePostTests, SameKeyAndSameBodyReplaysStoredResponse)
{
  cnerium::store::Store store;

  ASSERT_TRUE(store.open());

  int executed_count = 0;

  cnerium::http::DurableRoute route{
      "orders.create",
      store,
      [&executed_count](cnerium::http::DurableRequest &request)
      {
        ++executed_count;
        return create_order_response(request);
      }};

  const auto first =
      make_request(
          R"({"product_id":"p1","quantity":2})",
          "replay-request");

  const auto second =
      make_request(
          R"({"product_id":"p1","quantity":2})",
          "replay-request");

  const auto first_response = route.execute(first);
  const auto second_response = route.execute(second);

  EXPECT_EQ(first_response.status_code(), 201);
  EXPECT_EQ(second_response.status_code(), 201);

  EXPECT_EQ(first_response.body(), second_response.body());
  EXPECT_EQ(executed_count, 1);
}

TEST(DurablePostTests, SameKeyAndDifferentBodyReturnsConflict)
{
  cnerium::store::Store store;

  ASSERT_TRUE(store.open());

  int executed_count = 0;

  cnerium::http::DurableRoute route{
      "orders.create",
      store,
      [&executed_count](cnerium::http::DurableRequest &request)
      {
        ++executed_count;
        return create_order_response(request);
      }};

  const auto first =
      make_request(
          R"({"product_id":"p1","quantity":2})",
          "conflict-request");

  const auto second =
      make_request(
          R"({"product_id":"p2","quantity":1})",
          "conflict-request");

  const auto first_response = route.execute(first);
  const auto second_response = route.execute(second);

  EXPECT_EQ(first_response.status_code(), 201);
  EXPECT_EQ(second_response.status_code(), 409);

  EXPECT_EQ(executed_count, 1);
  EXPECT_TRUE(second_response.body().find("different request body") != std::string::npos);
}

TEST(DurablePostTests, InvalidRouteReturnsInternalError)
{
  cnerium::http::DurableRoute route;

  const auto request =
      make_request(
          R"({"product_id":"p1","quantity":2})",
          "invalid-route");

  const auto response = route.execute(request);

  EXPECT_EQ(response.status_code(), 500);
  EXPECT_TRUE(response.body().find("not configured") != std::string::npos);
}

TEST(DurablePostTests, HandlerExceptionReturnsInternalError)
{
  cnerium::store::Store store;

  ASSERT_TRUE(store.open());

  cnerium::http::DurableRoute route{
      "orders.create",
      store,
      [](cnerium::http::DurableRequest &) -> cnerium::http::DurableResponse
      {
        throw std::runtime_error("handler failed");

        return cnerium::http::DurableResponse::internal_error(
            "handler failed");
      }};

  const auto request =
      make_request(
          R"({"product_id":"p1","quantity":2})",
          "handler-exception");

  const auto response = route.execute(request);

  EXPECT_EQ(response.status_code(), 500);
  EXPECT_TRUE(response.body().find("handler failed") != std::string::npos);
}

TEST(DurablePostTests, InvalidResponseReturnsInternalError)
{
  cnerium::store::Store store;

  ASSERT_TRUE(store.open());

  cnerium::http::DurableRoute route{
      "orders.create",
      store,
      [](cnerium::http::DurableRequest &)
      {
        return cnerium::http::DurableResponse{
            999,
            "{}",
            "application/json; charset=utf-8"};
      }};

  const auto request =
      make_request(
          R"({"product_id":"p1","quantity":2})",
          "invalid-response");

  const auto response = route.execute(request);

  EXPECT_EQ(response.status_code(), 500);
  EXPECT_TRUE(response.body().find("invalid response") != std::string::npos);
}

TEST(DurablePostTests, DurableRequestExposesBodyAndHash)
{
  const auto request =
      make_request(
          R"({"product_id":"p1","quantity":2})",
          "body-and-hash");

  cnerium::http::DurableRequest durable_request{request};

  EXPECT_EQ(durable_request.body(), R"({"product_id":"p1","quantity":2})");
  EXPECT_TRUE(durable_request.request_hash().is_valid());
  EXPECT_EQ(durable_request.idempotency_key_value(), "body-and-hash");
}

TEST(DurablePostTests, DurableRequestTryJsonReturnsValueForValidJson)
{
  const auto request =
      make_request(
          R"({"product_id":"p1","quantity":2})",
          "valid-json");

  cnerium::http::DurableRequest durable_request{request};

  const auto body = durable_request.try_json();

  ASSERT_TRUE(body.has_value());
  EXPECT_EQ(cnerium::support::string_or(*body, "product_id", ""), "p1");
  EXPECT_EQ(cnerium::support::int_or(*body, "quantity", 0), 2);
}

TEST(DurablePostTests, DurableRequestTryJsonReturnsNulloptForInvalidJson)
{
  const auto request =
      make_request(
          "not-json",
          "invalid-json");

  cnerium::http::DurableRequest durable_request{request};

  const auto body = durable_request.try_json();

  EXPECT_FALSE(body.has_value());
}
