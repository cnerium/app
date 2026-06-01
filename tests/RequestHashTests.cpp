/**
 *
 *  @file RequestHashTests.cpp
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

#include <cnerium/reliability/RequestHash.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <string>

TEST(RequestHashTests, DefaultHashIsInvalid)
{
  cnerium::reliability::RequestHash hash;

  EXPECT_TRUE(hash.empty());
  EXPECT_FALSE(hash.is_valid());
  EXPECT_FALSE(hash.valid());
}

TEST(RequestHashTests, ConstructFromString)
{
  cnerium::reliability::RequestHash hash{"abc123"};

  EXPECT_FALSE(hash.empty());
  EXPECT_TRUE(hash.is_valid());
  EXPECT_TRUE(hash.valid());
  EXPECT_EQ(hash.value(), "abc123");
  EXPECT_EQ(hash.str(), "abc123");
}

TEST(RequestHashTests, ConstructFromStringView)
{
  const std::string value = "hash-value";

  cnerium::reliability::RequestHash hash{
      std::string_view{value}};

  EXPECT_EQ(hash.value(), "hash-value");
}

TEST(RequestHashTests, ConstructFromNullCStringCreatesEmptyHash)
{
  const char *value = nullptr;

  cnerium::reliability::RequestHash hash{value};

  EXPECT_TRUE(hash.empty());
  EXPECT_FALSE(hash.is_valid());
}

TEST(RequestHashTests, FromCreatesHash)
{
  auto hash =
      cnerium::reliability::RequestHash::from("stored-hash");

  EXPECT_EQ(hash.value(), "stored-hash");
  EXPECT_TRUE(hash.is_valid());
}

TEST(RequestHashTests, FromBodyIsDeterministic)
{
  const auto first =
      cnerium::reliability::RequestHash::from_body(
          R"({"product_id":"p1","quantity":2})");

  const auto second =
      cnerium::reliability::RequestHash::from_body(
          R"({"product_id":"p1","quantity":2})");

  EXPECT_EQ(first, second);
  EXPECT_EQ(first.value(), second.value());
}

TEST(RequestHashTests, DifferentBodiesProduceDifferentHashes)
{
  const auto first =
      cnerium::reliability::RequestHash::from_body(
          R"({"product_id":"p1","quantity":2})");

  const auto second =
      cnerium::reliability::RequestHash::from_body(
          R"({"product_id":"p2","quantity":1})");

  EXPECT_NE(first, second);
  EXPECT_NE(first.value(), second.value());
}

TEST(RequestHashTests, EmptyBodyStillProducesValidHash)
{
  const auto hash =
      cnerium::reliability::RequestHash::from_body("");

  EXPECT_TRUE(hash.is_valid());
  EXPECT_FALSE(hash.empty());
  EXPECT_EQ(hash.value().size(), 16U);
}

TEST(RequestHashTests, HashValueIsSixteenHexCharacters)
{
  const auto hash =
      cnerium::reliability::RequestHash::from_body("hello");

  EXPECT_EQ(hash.value().size(), 16U);

  for (const char character : hash.value())
  {
    const bool is_digit =
        character >= '0' && character <= '9';

    const bool is_lower_hex =
        character >= 'a' && character <= 'f';

    EXPECT_TRUE(is_digit || is_lower_hex);
  }
}

TEST(RequestHashTests, Fnva164KnownEmptyHash)
{
  const std::uint64_t value =
      cnerium::reliability::RequestHash::fnv1a64("");

  EXPECT_EQ(value, 14695981039346656037ULL);
}

TEST(RequestHashTests, Fnva164KnownHelloHash)
{
  const std::uint64_t value =
      cnerium::reliability::RequestHash::fnv1a64("hello");

  EXPECT_EQ(value, 11831194018420276491ULL);
}

TEST(RequestHashTests, ToHexPadsWithLeadingZeros)
{
  const std::string value =
      cnerium::reliability::RequestHash::to_hex(1ULL);

  EXPECT_EQ(value, "0000000000000001");
}

TEST(RequestHashTests, ToHexUsesLowercase)
{
  const std::string value =
      cnerium::reliability::RequestHash::to_hex(0xABCDEFULL);

  EXPECT_EQ(value, "0000000000abcdef");
}

TEST(RequestHashTests, EqualityComparesValue)
{
  const cnerium::reliability::RequestHash left{"same"};
  const cnerium::reliability::RequestHash right{"same"};

  EXPECT_TRUE(left == right);
  EXPECT_FALSE(left != right);
}

TEST(RequestHashTests, InequalityComparesValue)
{
  const cnerium::reliability::RequestHash left{"left"};
  const cnerium::reliability::RequestHash right{"right"};

  EXPECT_TRUE(left != right);
  EXPECT_FALSE(left == right);
}

TEST(RequestHashTests, LessThanOrdersLexicographically)
{
  const cnerium::reliability::RequestHash left{"aaa"};
  const cnerium::reliability::RequestHash right{"bbb"};

  EXPECT_TRUE(left < right);
}

TEST(RequestHashTests, ClearEmptiesHash)
{
  cnerium::reliability::RequestHash hash{"abc123"};

  ASSERT_TRUE(hash.is_valid());

  hash.clear();

  EXPECT_TRUE(hash.empty());
  EXPECT_FALSE(hash.is_valid());
}
