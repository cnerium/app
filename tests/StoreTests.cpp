/**
 *
 *  @file StoreTests.cpp
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

#include <cnerium/store/Store.hpp>
#include <cnerium/store/StoreKey.hpp>
#include <cnerium/store/StoredResponse.hpp>

#include <gtest/gtest.h>

#include <string>

namespace
{
  [[nodiscard]] cnerium::store::StoreKey test_key(const std::string &name)
  {
    return cnerium::store::StoreKey::from(
        "cnerium:test:store:" + name);
  }
} // namespace

TEST(StoreTests, DefaultStoreStartsClosed)
{
  cnerium::store::Store store;

  EXPECT_FALSE(store.is_open());
  EXPECT_FALSE(store.opened());
}

TEST(StoreTests, OpenMakesStoreUsable)
{
  cnerium::store::Store store;

  EXPECT_TRUE(store.open());
  EXPECT_TRUE(store.is_open());
  EXPECT_TRUE(store.opened());
}

TEST(StoreTests, CloseIsSafeWhenStoreIsClosed)
{
  cnerium::store::Store store;

  EXPECT_NO_THROW(store.close());
  EXPECT_FALSE(store.is_open());
}

TEST(StoreTests, PutReturnsFalseForInvalidKey)
{
  cnerium::store::Store store;
  cnerium::store::StoreKey key;

  EXPECT_FALSE(store.put(key, "value"));
}

TEST(StoreTests, PutAndGetStringValue)
{
  cnerium::store::Store store;

  const auto key = test_key("put-get-string");

  ASSERT_TRUE(store.open());
  ASSERT_TRUE(store.put(key, "hello"));

  const auto value = store.get(key);

  ASSERT_TRUE(value.has_value());
  EXPECT_EQ(*value, "hello");
}

TEST(StoreTests, GetReturnsNulloptForInvalidKey)
{
  cnerium::store::Store store;
  cnerium::store::StoreKey key;

  ASSERT_TRUE(store.open());

  const auto value = store.get(key);

  EXPECT_FALSE(value.has_value());
}

TEST(StoreTests, GetReturnsNulloptWhenStoreIsClosed)
{
  cnerium::store::Store store;

  const auto value = store.get(test_key("closed-store"));

  EXPECT_FALSE(value.has_value());
}

TEST(StoreTests, ContainsReturnsTrueAfterPut)
{
  cnerium::store::Store store;

  const auto key = test_key("contains-after-put");

  ASSERT_TRUE(store.open());
  ASSERT_TRUE(store.put(key, "value"));

  EXPECT_TRUE(store.contains(key));
}

TEST(StoreTests, ContainsReturnsFalseForInvalidKey)
{
  cnerium::store::Store store;
  cnerium::store::StoreKey key;

  ASSERT_TRUE(store.open());

  EXPECT_FALSE(store.contains(key));
}

TEST(StoreTests, RemoveDeletesExistingKey)
{
  cnerium::store::Store store;

  const auto key = test_key("remove-existing");

  ASSERT_TRUE(store.open());
  ASSERT_TRUE(store.put(key, "value"));
  ASSERT_TRUE(store.contains(key));

  EXPECT_TRUE(store.remove(key));
  EXPECT_FALSE(store.contains(key));
}

TEST(StoreTests, RemoveReturnsFalseForInvalidKey)
{
  cnerium::store::Store store;
  cnerium::store::StoreKey key;

  ASSERT_TRUE(store.open());

  EXPECT_FALSE(store.remove(key));
}

TEST(StoreTests, PutResponseStoresReplayableResponse)
{
  cnerium::store::Store store;

  const auto key = test_key("stored-response");

  const cnerium::store::StoredResponse response =
      cnerium::store::StoredResponse::json(
          201,
          cnerium::support::object({
              {"ok", cnerium::Json(true)},
              {"order_id", cnerium::Json("ord_123")},
          }));

  ASSERT_TRUE(store.open());
  ASSERT_TRUE(store.put_response(key, response));

  const auto restored = store.get_response(key);

  ASSERT_TRUE(restored.has_value());
  EXPECT_EQ(restored->status_code(), 201);
  EXPECT_EQ(restored->content_type(), "application/json; charset=utf-8");
  EXPECT_TRUE(restored->body().find("ord_123") != std::string::npos);
}

TEST(StoreTests, GetResponseReturnsNulloptForInvalidKey)
{
  cnerium::store::Store store;
  cnerium::store::StoreKey key;

  ASSERT_TRUE(store.open());

  const auto response = store.get_response(key);

  EXPECT_FALSE(response.has_value());
}

TEST(StoreTests, GetResponseReturnsNulloptForInvalidStoredJson)
{
  cnerium::store::Store store;

  const auto key = test_key("invalid-stored-json");

  ASSERT_TRUE(store.open());
  ASSERT_TRUE(store.put(key, "not-json"));

  const auto response = store.get_response(key);

  EXPECT_FALSE(response.has_value());
}

TEST(StoreTests, TickOpensStoreIfNeeded)
{
  cnerium::store::Store store;

  EXPECT_TRUE(store.tick());
  EXPECT_TRUE(store.is_open());
}

TEST(StoreTests, SizeAndEmptyAreSafe)
{
  cnerium::store::Store store;

  EXPECT_NO_THROW({
    (void)store.size();
    (void)store.empty();
  });
}

TEST(StoreTests, MoveConstructorTransfersStore)
{
  cnerium::store::Store original;

  const auto key = test_key("move-constructor");

  ASSERT_TRUE(original.open());
  ASSERT_TRUE(original.put(key, "value"));

  cnerium::store::Store moved{std::move(original)};

  EXPECT_TRUE(moved.is_open());

  const auto value = moved.get(key);

  ASSERT_TRUE(value.has_value());
  EXPECT_EQ(*value, "value");
}

TEST(StoreTests, MoveAssignmentTransfersStore)
{
  cnerium::store::Store original;
  cnerium::store::Store moved;

  const auto key = test_key("move-assignment");

  ASSERT_TRUE(original.open());
  ASSERT_TRUE(original.put(key, "value"));

  moved = std::move(original);

  EXPECT_TRUE(moved.is_open());

  const auto value = moved.get(key);

  ASSERT_TRUE(value.has_value());
  EXPECT_EQ(*value, "value");
}
