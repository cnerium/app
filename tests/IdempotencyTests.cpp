/**
 *
 *  @file IdempotencyTests.cpp
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

#include <cnerium/reliability/Idempotency.hpp>
#include <cnerium/reliability/IdempotencyKey.hpp>
#include <cnerium/reliability/RequestHash.hpp>
#include <cnerium/store/Store.hpp>
#include <cnerium/store/StoredResponse.hpp>
#include <cnerium/support/Json.hpp>

#include <gtest/gtest.h>

#include <string>

namespace
{
  [[nodiscard]] cnerium::reliability::IdempotencyKey key(
      const std::string &value)
  {
    return cnerium::reliability::IdempotencyKey{value};
  }

  [[nodiscard]] cnerium::store::StoredResponse response(
      int status_code = 201,
      const std::string &order_id = "ord_123")
  {
    return cnerium::store::StoredResponse::json(
        status_code,
        cnerium::support::object({
            {"ok", cnerium::Json(true)},
            {"order_id", cnerium::Json(order_id)},
        }));
  }
} // namespace

TEST(IdempotencyTests, MissingKeyReturnsInvalid)
{
  cnerium::store::Store store;
  cnerium::reliability::Idempotency idempotency{store};

  ASSERT_TRUE(store.open());

  const auto result =
      idempotency.check(
          "orders.create",
          cnerium::reliability::IdempotencyKey{},
          R"({"product_id":"p1","quantity":2})");

  EXPECT_TRUE(result.is_invalid());
  EXPECT_FALSE(result.should_execute());
  EXPECT_FALSE(result.should_replay());
  EXPECT_FALSE(result.is_conflict());
}

TEST(IdempotencyTests, EmptyOperationReturnsInvalid)
{
  cnerium::store::Store store;
  cnerium::reliability::Idempotency idempotency{store};

  ASSERT_TRUE(store.open());

  const auto result =
      idempotency.check(
          "",
          key("order-123"),
          R"({"product_id":"p1","quantity":2})");

  EXPECT_TRUE(result.is_invalid());
}

TEST(IdempotencyTests, NewRequestReturnsExecute)
{
  cnerium::store::Store store;
  cnerium::reliability::Idempotency idempotency{store};

  ASSERT_TRUE(store.open());

  const auto result =
      idempotency.check(
          "orders.create",
          key("new-request"),
          R"({"product_id":"p1","quantity":2})");

  EXPECT_TRUE(result.should_execute());
  EXPECT_FALSE(result.should_replay());
  EXPECT_FALSE(result.is_conflict());
  EXPECT_FALSE(result.is_invalid());
}

TEST(IdempotencyTests, CommitStoresCompletedResponse)
{
  cnerium::store::Store store;
  cnerium::reliability::Idempotency idempotency{store};

  ASSERT_TRUE(store.open());

  const bool committed =
      idempotency.commit(
          "orders.create",
          key("commit-response"),
          R"({"product_id":"p1","quantity":2})",
          response());

  EXPECT_TRUE(committed);
}

TEST(IdempotencyTests, SameKeyAndSameBodyReturnsReplay)
{
  cnerium::store::Store store;
  cnerium::reliability::Idempotency idempotency{store};

  ASSERT_TRUE(store.open());

  const std::string operation = "orders.create";
  const auto idem_key = key("same-key-same-body");
  const std::string body = R"({"product_id":"p1","quantity":2})";

  ASSERT_TRUE(
      idempotency.commit(
          operation,
          idem_key,
          body,
          response(201, "ord_same")));

  const auto result =
      idempotency.check(
          operation,
          idem_key,
          body);

  ASSERT_TRUE(result.should_replay());
  EXPECT_FALSE(result.should_execute());
  EXPECT_FALSE(result.is_conflict());
  EXPECT_FALSE(result.is_invalid());

  EXPECT_EQ(result.response().status_code(), 201);
  EXPECT_TRUE(result.response().body().find("ord_same") != std::string::npos);
}

TEST(IdempotencyTests, SameKeyAndDifferentBodyReturnsConflict)
{
  cnerium::store::Store store;
  cnerium::reliability::Idempotency idempotency{store};

  ASSERT_TRUE(store.open());

  const std::string operation = "orders.create";
  const auto idem_key = key("same-key-different-body");

  ASSERT_TRUE(
      idempotency.commit(
          operation,
          idem_key,
          R"({"product_id":"p1","quantity":2})",
          response()));

  const auto result =
      idempotency.check(
          operation,
          idem_key,
          R"({"product_id":"p2","quantity":1})");

  EXPECT_TRUE(result.is_conflict());
  EXPECT_FALSE(result.should_execute());
  EXPECT_FALSE(result.should_replay());
  EXPECT_FALSE(result.is_invalid());
}

TEST(IdempotencyTests, CheckHashSupportsPrecomputedHash)
{
  cnerium::store::Store store;
  cnerium::reliability::Idempotency idempotency{store};

  ASSERT_TRUE(store.open());

  const std::string operation = "orders.create";
  const auto idem_key = key("precomputed-hash");
  const auto hash =
      cnerium::reliability::RequestHash::from_body(
          R"({"product_id":"p1","quantity":2})");

  const auto result =
      idempotency.check_hash(
          operation,
          idem_key,
          hash);

  EXPECT_TRUE(result.should_execute());
}

TEST(IdempotencyTests, CommitHashStoresCompletedResponse)
{
  cnerium::store::Store store;
  cnerium::reliability::Idempotency idempotency{store};

  ASSERT_TRUE(store.open());

  const std::string operation = "orders.create";
  const auto idem_key = key("commit-hash");
  const auto hash =
      cnerium::reliability::RequestHash::from_body(
          R"({"product_id":"p1","quantity":2})");

  EXPECT_TRUE(
      idempotency.commit_hash(
          operation,
          idem_key,
          hash,
          response(201, "ord_hash")));

  const auto result =
      idempotency.check_hash(
          operation,
          idem_key,
          hash);

  ASSERT_TRUE(result.should_replay());
  EXPECT_TRUE(result.response().body().find("ord_hash") != std::string::npos);
}

TEST(IdempotencyTests, CommitReturnsFalseForInvalidKey)
{
  cnerium::store::Store store;
  cnerium::reliability::Idempotency idempotency{store};

  ASSERT_TRUE(store.open());

  const bool committed =
      idempotency.commit(
          "orders.create",
          cnerium::reliability::IdempotencyKey{},
          R"({"product_id":"p1","quantity":2})",
          response());

  EXPECT_FALSE(committed);
}

TEST(IdempotencyTests, CommitReturnsFalseForEmptyOperation)
{
  cnerium::store::Store store;
  cnerium::reliability::Idempotency idempotency{store};

  ASSERT_TRUE(store.open());

  const bool committed =
      idempotency.commit(
          "",
          key("empty-operation"),
          R"({"product_id":"p1","quantity":2})",
          response());

  EXPECT_FALSE(committed);
}

TEST(IdempotencyTests, CommitHashReturnsFalseForInvalidHash)
{
  cnerium::store::Store store;
  cnerium::reliability::Idempotency idempotency{store};

  ASSERT_TRUE(store.open());

  const bool committed =
      idempotency.commit_hash(
          "orders.create",
          key("invalid-hash"),
          cnerium::reliability::RequestHash{},
          response());

  EXPECT_FALSE(committed);
}

TEST(IdempotencyTests, HashBodyMatchesRequestHashFromBody)
{
  const std::string body = R"({"product_id":"p1","quantity":2})";

  const auto left =
      cnerium::reliability::Idempotency::hash_body(body);

  const auto right =
      cnerium::reliability::RequestHash::from_body(body);

  EXPECT_EQ(left, right);
}
