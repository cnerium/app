# Realtime

Cnerium realtime provides a simple application-level event API backed by Vix WebSocket.

It lets a Cnerium application emit typed events after durable operations.

Example:

```txt
POST /orders
  -> durable route succeeds
  -> Cnerium emits order.created
  -> Vix WebSocket broadcasts the event
```

Cnerium does not implement its own WebSocket server.

Cnerium uses Vix WebSocket internally.

## Goal

The goal of realtime in the MVP is simple:

```txt
Allow a durable HTTP action to notify connected clients in realtime.
```

The first use case is:

```txt
durable order creation
+ order.created realtime event
```

## Enable realtime

Enable realtime on the application:

```cpp
#include <cnerium/cnerium.hpp>

int main()
{
  cnerium::App app;

  app.realtime("/ws", "0.0.0.0", 9090);

  return app.run();
}
```

This enables the WebSocket endpoint:

```txt
ws://127.0.0.1:9090/ws
```

## Emit an event

Use `app.emit()`:

```cpp
app.emit(
    "order.created",
    cnerium::support::object({
        {"order_id", cnerium::Json("ord_123")},
        {"product_id", cnerium::Json("p1")},
        {"quantity", cnerium::Json(2)},
    }));
```

The first argument is the event type.

The second argument is the event payload.

## Emit from a durable route

Realtime is most useful after a durable operation succeeds.

```cpp
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
          }));
    });
```

## Event model

A realtime event contains:

```txt
type
payload
```

Example:

```json
{
  "type": "order.created",
  "payload": {
    "order_id": "ord_123",
    "product_id": "p1",
    "quantity": 2
  }
}
```

In Cnerium code:

```cpp
cnerium::Event event{
    "order.created",
    cnerium::support::object({
        {"order_id", cnerium::Json("ord_123")},
    })};
```

## Event types

Use clear event names.

Good examples:

```txt
order.created
order.updated
order.cancelled
user.registered
payment.created
notification.sent
```

Avoid vague names:

```txt
created
updated
message
event
data
```

The event type should describe what happened.

## Event payload

The payload is JSON.

```cpp
cnerium::EventPayload payload =
    cnerium::support::object({
        {"order_id", cnerium::Json("ord_123")},
        {"quantity", cnerium::Json(2)},
    });
```

Cnerium uses the same JSON type as Vix JSON.

This keeps Cnerium compatible with Vix HTTP and Vix WebSocket.

## Emit to all clients

Use:

```cpp
app.emit("order.created", payload);
```

This broadcasts the event to connected clients.

## Emit to a room

Use:

```cpp
app.emit_to("orders", "order.created", payload);
```

This sends the event to a specific room.

Room support is delegated to Vix WebSocket.

Cnerium does not implement its own room system.

## RealtimeConfig

Realtime can be configured with `RealtimeConfig`.

```cpp
auto config =
    cnerium::realtime::RealtimeConfig::enabled(
        "/ws",
        "0.0.0.0",
        9090);
```

Fields:

```txt
enabled
endpoint
host
port
```

Default values:

```txt
endpoint: /ws
host: 0.0.0.0
port: 9090
```

## Disable realtime

Realtime is disabled by default in some configurations.

You can disable it explicitly:

```cpp
app.config().disable_realtime();
```

Or through `RealtimeConfig`:

```cpp
auto config = cnerium::realtime::RealtimeConfig::disabled();
```

When realtime is disabled, `emit()` returns `false`.

## Internal flow

```txt
Application handler
  -> app.emit("order.created", payload)

cnerium::App
  -> AppRuntime

AppRuntime
  -> VixWebSocket adapter

VixWebSocket
  -> converts EventPayload to vix::json::kvs
  -> calls vix::websocket::Server

Vix WebSocket
  -> broadcasts the event to connected clients
```

## What Cnerium owns

Cnerium owns the application-level event model:

```txt
Event
EventPayload
RealtimeConfig
Realtime facade
emit()
emit_to()
```

## What Vix owns

Vix owns the WebSocket runtime:

```txt
WebSocket server
WebSocket session
WebSocket protocol
frame parsing
connection lifecycle
ping/pong
broadcast
rooms
metrics
long-polling bridge
message store
```

Cnerium does not recreate those parts.

## Relationship with durable routes

Realtime events are not the source of truth.

The durable operation is the source of truth.

A common flow is:

```txt
1. Durable route receives request.
2. Cnerium validates idempotency.
3. Handler creates application result.
4. Cnerium stores replayable response.
5. Handler emits realtime event.
6. Vix WebSocket broadcasts event.
```

The event is a notification.

The durable response and storage are the reliability foundation.

## Retry behavior

When the same durable request is retried with the same body, Cnerium replays the stored response.

The handler is not executed again.

That means the realtime event is not emitted again by the handler.

```txt
First request
  -> handler executes
  -> order.created emitted

Retry with same key and same body
  -> stored response replayed
  -> handler not executed
  -> event not emitted again
```

This prevents duplicate realtime notifications for completed durable requests.

## Example client connection

A browser or WebSocket client can connect to:

```txt
ws://127.0.0.1:9090/ws
```

The exact client message format depends on the Vix WebSocket protocol.

Cnerium emits typed events through Vix WebSocket.

## MVP features

The MVP realtime layer supports:

```txt
enable realtime
emit event
emit event to room
convert JSON payload to Vix WebSocket payload
start and stop Vix WebSocket through Cnerium runtime
```

## MVP limitations

The MVP does not add Cnerium-specific:

```txt
WebSocket authentication
subscription authorization
advanced room policies
presence
message persistence
long-polling fallback configuration
event replay
event outbox
```

Those can come later.

The MVP only proves:

```txt
durable HTTP action
+ realtime event
```

## Future extensions

Future versions can add:

```txt
authenticated realtime connections
room authorization
tenant-aware rooms
event persistence
event replay
long-polling fallback
presence
delivery acknowledgements
outbox-backed event delivery
```

But the rule remains:

```txt
Cnerium owns application realtime semantics.
Vix owns WebSocket infrastructure.
```

## Summary

Cnerium realtime provides:

```txt
simple app.emit()
typed application events
JSON payloads
optional rooms
Vix WebSocket integration
no duplicate WebSocket implementation
```

Use realtime when an application action should notify clients immediately.

Use durable routes when the action must be safe under retries.

Use both together for reliability-first realtime applications.
