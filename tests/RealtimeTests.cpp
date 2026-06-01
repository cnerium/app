/**
 *
 *  @file RealtimeTests.cpp
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

#include <cnerium/adapters/VixWebSocket.hpp>
#include <cnerium/realtime/Event.hpp>
#include <cnerium/realtime/Realtime.hpp>
#include <cnerium/realtime/RealtimeConfig.hpp>
#include <cnerium/support/Json.hpp>

#include <vix/config/Config.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <string>

TEST(RealtimeTests, DefaultConfigIsDisabled)
{
  cnerium::realtime::RealtimeConfig config;

  EXPECT_FALSE(config.is_enabled());
  EXPECT_FALSE(config.enabled());
  EXPECT_EQ(config.endpoint(), "/ws");
  EXPECT_EQ(config.host(), "0.0.0.0");
  EXPECT_EQ(config.port(), 9090);
  EXPECT_TRUE(config.is_valid());
  EXPECT_TRUE(config.valid());
}

TEST(RealtimeTests, EnabledConfigIsValid)
{
  const auto config =
      cnerium::realtime::RealtimeConfig::enabled(
          "/ws",
          "127.0.0.1",
          9091);

  EXPECT_TRUE(config.is_enabled());
  EXPECT_TRUE(config.enabled());
  EXPECT_EQ(config.endpoint(), "/ws");
  EXPECT_EQ(config.host(), "127.0.0.1");
  EXPECT_EQ(config.port(), 9091);
  EXPECT_TRUE(config.is_valid());
}

TEST(RealtimeTests, DisabledConfigIsAlwaysValid)
{
  const auto config =
      cnerium::realtime::RealtimeConfig::disabled();

  EXPECT_FALSE(config.is_enabled());
  EXPECT_TRUE(config.is_valid());
}

TEST(RealtimeTests, EnabledConfigRequiresEndpoint)
{
  auto config =
      cnerium::realtime::RealtimeConfig::enabled(
          "",
          "127.0.0.1",
          9091);

  EXPECT_TRUE(config.is_enabled());
  EXPECT_FALSE(config.has_endpoint());
  EXPECT_FALSE(config.is_valid());
}

TEST(RealtimeTests, EnabledConfigRequiresHost)
{
  auto config =
      cnerium::realtime::RealtimeConfig::enabled(
          "/ws",
          "",
          9091);

  EXPECT_TRUE(config.is_enabled());
  EXPECT_FALSE(config.has_host());
  EXPECT_FALSE(config.is_valid());
}

TEST(RealtimeTests, EnabledConfigRequiresPort)
{
  auto config =
      cnerium::realtime::RealtimeConfig::enabled(
          "/ws",
          "127.0.0.1",
          0);

  EXPECT_TRUE(config.is_enabled());
  EXPECT_FALSE(config.has_port());
  EXPECT_FALSE(config.is_valid());
}

TEST(RealtimeTests, ConfigSettersUpdateValues)
{
  cnerium::realtime::RealtimeConfig config;

  config.set_enabled(true);
  config.set_endpoint("/events");
  config.set_host("127.0.0.1");
  config.set_port(9191);

  EXPECT_TRUE(config.is_enabled());
  EXPECT_EQ(config.endpoint(), "/events");
  EXPECT_EQ(config.host(), "127.0.0.1");
  EXPECT_EQ(config.port(), 9191);
  EXPECT_TRUE(config.is_valid());
}

TEST(RealtimeTests, ConfigClearDisablesAndEmptiesValues)
{
  auto config =
      cnerium::realtime::RealtimeConfig::enabled(
          "/ws",
          "127.0.0.1",
          9091);

  config.clear();

  EXPECT_FALSE(config.is_enabled());
  EXPECT_FALSE(config.has_endpoint());
  EXPECT_FALSE(config.has_host());
  EXPECT_FALSE(config.has_port());
  EXPECT_TRUE(config.is_valid());
}

TEST(RealtimeTests, DefaultEventIsInvalid)
{
  cnerium::realtime::Event event;

  EXPECT_TRUE(event.empty());
  EXPECT_FALSE(event.is_valid());
  EXPECT_FALSE(event.valid());
}

TEST(RealtimeTests, EventWithTypeIsValid)
{
  cnerium::realtime::Event event{
      "order.created",
      cnerium::support::object({
          {"order_id", cnerium::Json("ord_123")},
      })};

  EXPECT_FALSE(event.empty());
  EXPECT_TRUE(event.is_valid());
  EXPECT_TRUE(event.valid());
  EXPECT_EQ(event.type(), "order.created");
  EXPECT_EQ(cnerium::support::string_or(event.payload(), "order_id", ""), "ord_123");
}

TEST(RealtimeTests, EventMakeCreatesEvent)
{
  const auto event =
      cnerium::realtime::Event::make(
          "order.created",
          cnerium::support::object({
              {"order_id", cnerium::Json("ord_456")},
          }));

  EXPECT_TRUE(event.is_valid());
  EXPECT_EQ(event.type(), "order.created");
  EXPECT_EQ(cnerium::support::string_or(event.payload(), "order_id", ""), "ord_456");
}

TEST(RealtimeTests, EventFromCreatesEvent)
{
  const auto event =
      cnerium::realtime::Event::from(
          "order.updated",
          cnerium::support::object({
              {"order_id", cnerium::Json("ord_789")},
          }));

  EXPECT_TRUE(event.is_valid());
  EXPECT_EQ(event.type(), "order.updated");
  EXPECT_EQ(cnerium::support::string_or(event.payload(), "order_id", ""), "ord_789");
}

TEST(RealtimeTests, EventSettersUpdateValues)
{
  cnerium::realtime::Event event;

  event.set_type("order.created");
  event.set_payload(
      cnerium::support::object({
          {"order_id", cnerium::Json("ord_123")},
      }));

  EXPECT_TRUE(event.is_valid());
  EXPECT_EQ(event.type(), "order.created");
  EXPECT_EQ(cnerium::support::string_or(event.payload(), "order_id", ""), "ord_123");
}

TEST(RealtimeTests, EventToJsonContainsTypeAndPayload)
{
  const cnerium::realtime::Event event{
      "order.created",
      cnerium::support::object({
          {"order_id", cnerium::Json("ord_123")},
          {"quantity", cnerium::Json(2)},
      })};

  const auto json = event.to_json();

  EXPECT_EQ(cnerium::support::string_or(json, "type", ""), "order.created");
  ASSERT_TRUE(json.contains("payload"));
  EXPECT_EQ(cnerium::support::string_or(json["payload"], "order_id", ""), "ord_123");
  EXPECT_EQ(cnerium::support::int_or(json["payload"], "quantity", 0), 2);
}

TEST(RealtimeTests, EventToJsonTextSerializesEvent)
{
  const cnerium::realtime::Event event{
      "order.created",
      cnerium::support::object({
          {"order_id", cnerium::Json("ord_123")},
      })};

  const std::string text = event.to_json_text();

  EXPECT_TRUE(text.find("order.created") != std::string::npos);
  EXPECT_TRUE(text.find("ord_123") != std::string::npos);
}

TEST(RealtimeTests, EventClearMakesEventInvalid)
{
  cnerium::realtime::Event event{
      "order.created",
      cnerium::support::object({
          {"order_id", cnerium::Json("ord_123")},
      })};

  ASSERT_TRUE(event.is_valid());

  event.clear();

  EXPECT_TRUE(event.empty());
  EXPECT_FALSE(event.is_valid());
  EXPECT_TRUE(event.payload().is_object());
}

TEST(RealtimeTests, RealtimeStartsDisabled)
{
  cnerium::realtime::Realtime realtime;

  EXPECT_FALSE(realtime.is_enabled());
  EXPECT_FALSE(realtime.is_running());
  EXPECT_EQ(realtime.server(), nullptr);
}

TEST(RealtimeTests, RealtimeEnableUpdatesConfig)
{
  cnerium::realtime::Realtime realtime;

  realtime.enable("/events", "127.0.0.1", 9191);

  EXPECT_TRUE(realtime.is_enabled());
  EXPECT_EQ(realtime.config().endpoint(), "/events");
  EXPECT_EQ(realtime.config().host(), "127.0.0.1");
  EXPECT_EQ(realtime.config().port(), 9191);
}

TEST(RealtimeTests, RealtimeDisableStopsAndDisables)
{
  cnerium::realtime::Realtime realtime;

  realtime.enable("/events", "127.0.0.1", 9191);
  ASSERT_TRUE(realtime.is_enabled());

  realtime.disable();

  EXPECT_FALSE(realtime.is_enabled());
  EXPECT_FALSE(realtime.is_running());
}

TEST(RealtimeTests, RealtimeStartReturnsTrueWhenDisabled)
{
  cnerium::realtime::Realtime realtime;
  vix::config::Config config{"vix.json"};

  EXPECT_TRUE(realtime.start(config, nullptr));
  EXPECT_FALSE(realtime.is_running());
}

TEST(RealtimeTests, RealtimeStartReturnsFalseForEnabledWithoutExecutor)
{
  cnerium::realtime::Realtime realtime;
  vix::config::Config config{"vix.json"};

  realtime.enable("/ws", "127.0.0.1", 9091);

  EXPECT_FALSE(realtime.start(config, nullptr));
  EXPECT_FALSE(realtime.is_running());
}

TEST(RealtimeTests, RealtimeEmitReturnsFalseWhenNotRunning)
{
  cnerium::realtime::Realtime realtime;

  const cnerium::realtime::Event event{
      "order.created",
      cnerium::support::object({
          {"order_id", cnerium::Json("ord_123")},
      })};

  EXPECT_FALSE(realtime.emit(event));
}

TEST(RealtimeTests, RealtimeEmitInvalidEventReturnsFalse)
{
  cnerium::realtime::Realtime realtime;

  EXPECT_FALSE(realtime.emit(cnerium::realtime::Event{}));
}

TEST(RealtimeTests, RealtimeEmitToReturnsFalseWhenNotRunning)
{
  cnerium::realtime::Realtime realtime;

  const cnerium::realtime::Event event{
      "order.created",
      cnerium::support::object({
          {"order_id", cnerium::Json("ord_123")},
      })};

  EXPECT_FALSE(realtime.emit_to("orders", event));
}

TEST(RealtimeTests, VixWebSocketPayloadConvertsString)
{
  const auto payload =
      cnerium::support::object({
          {"order_id", cnerium::Json("ord_123")},
      });

  const auto converted =
      cnerium::adapters::VixWebSocket::to_vix_payload(payload);

  const auto *value = converted.get_ptr("order_id");

  ASSERT_NE(value, nullptr);
  ASSERT_TRUE(value->is_string());
  EXPECT_EQ(value->as_string_or(""), "ord_123");
}

TEST(RealtimeTests, VixWebSocketPayloadConvertsInteger)
{
  const auto payload =
      cnerium::support::object({
          {"quantity", cnerium::Json(2)},
      });

  const auto converted =
      cnerium::adapters::VixWebSocket::to_vix_payload(payload);

  const auto *value = converted.get_ptr("quantity");

  ASSERT_NE(value, nullptr);
  ASSERT_TRUE(value->is_i64());
  EXPECT_EQ(value->as_i64_or(0), 2);
}

TEST(RealtimeTests, VixWebSocketPayloadConvertsBoolean)
{
  const auto payload =
      cnerium::support::object({
          {"ok", cnerium::Json(true)},
      });

  const auto converted =
      cnerium::adapters::VixWebSocket::to_vix_payload(payload);

  const auto *value = converted.get_ptr("ok");

  ASSERT_NE(value, nullptr);
  ASSERT_TRUE(value->is_bool());
  EXPECT_TRUE(value->as_bool_or(false));
}

TEST(RealtimeTests, VixWebSocketPayloadConvertsFloat)
{
  const auto payload =
      cnerium::support::object({
          {"price", cnerium::Json(19.5)},
      });

  const auto converted =
      cnerium::adapters::VixWebSocket::to_vix_payload(payload);

  const auto *value = converted.get_ptr("price");

  ASSERT_NE(value, nullptr);
  ASSERT_TRUE(value->is_f64());
  EXPECT_DOUBLE_EQ(value->as_f64_or(0.0), 19.5);
}

TEST(RealtimeTests, VixWebSocketPayloadConvertsNull)
{
  const auto payload =
      cnerium::support::object({
          {"note", cnerium::Json(nullptr)},
      });

  const auto converted =
      cnerium::adapters::VixWebSocket::to_vix_payload(payload);

  const auto *value = converted.get_ptr("note");

  ASSERT_NE(value, nullptr);
  EXPECT_TRUE(value->is_null());
}

TEST(RealtimeTests, VixWebSocketPayloadConvertsNestedObjectAsString)
{
  const auto payload =
      cnerium::support::object({
          {"meta",
           cnerium::support::object({
               {"source", cnerium::Json("test")},
           })},
      });

  const auto converted =
      cnerium::adapters::VixWebSocket::to_vix_payload(payload);

  const auto *value = converted.get_ptr("meta");

  ASSERT_NE(value, nullptr);
  ASSERT_TRUE(value->is_string());
  EXPECT_TRUE(value->as_string_or("").find("source") != std::string::npos);
}

TEST(RealtimeTests, VixWebSocketPayloadWrapsNonObjectPayload)
{
  const cnerium::Json payload = cnerium::Json("hello");

  const auto converted =
      cnerium::adapters::VixWebSocket::to_vix_payload(payload);

  const auto *value = converted.get_ptr("value");

  ASSERT_NE(value, nullptr);
  ASSERT_TRUE(value->is_string());
  EXPECT_TRUE(value->as_string_or("").find("hello") != std::string::npos);
}
