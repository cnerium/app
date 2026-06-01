# Cnerium

Cnerium is the official reliability-first application framework for the Vix and Softadastra ecosystem.

It brings together:

```txt
Vix
  -> runtime, HTTP, WebSocket, routing, middleware, build and developer workflow

Softadastra SDK
  -> durable storage, WAL-backed persistence, sync, transport and local-first foundations

Cnerium
  -> reliability-first application layer for durable APIs and realtime apps
```

Cnerium does not replace Vix.

Cnerium does not recreate the Softadastra SDK.

Cnerium uses both to provide a clean framework API for applications that must stay correct under retries, crashes, timeouts and unstable networks.

## What Cnerium is

Cnerium is a backend-focused framework for building reliable applications.

Its first goal is simple:

```txt
Turn a normal Vix route into a durable, idempotent and retry-safe route.
```

The first MVP use case is:

```txt
durable HTTP POST
+ Idempotency-Key
+ Softadastra SDK-backed storage
+ Vix WebSocket realtime event
```

## What Cnerium is not

Cnerium is not a clone of Laravel, Symfony, Express, Django or any existing web framework.

Cnerium is not a new HTTP server.

Cnerium is not a new WebSocket server.

Cnerium is not a new sync engine.

Cnerium is not a new WAL engine.

It is a different direction:

```txt
100% reliability-first application framework
built on top of Vix and Softadastra.
```

## Core idea

A normal backend route often fails badly when a client retries a request.

Example:

```txt
POST /orders
```

If the network cuts after the server creates the order but before the client receives the response, the client may retry the same request.

Without reliability rules, the server may create the same order twice.

Cnerium solves this with durable route semantics:

```txt
same Idempotency-Key + same body
  -> return the same stored response

same Idempotency-Key + different body
  -> reject with 409 Conflict

missing Idempotency-Key
  -> reject with 400 Bad Request
```

## Minimal example

```cpp
#include <cnerium/cnerium.hpp>

#include <string>

int main()
{
  cnerium::App app;

  app.realtime("/ws", "0.0.0.0", 9090);

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
```

## Durable route flow

```txt
Client
  -> POST /orders
  -> Idempotency-Key: order-123

Vix
  -> receives HTTP request
  -> routes request to Cnerium handler

Cnerium
  -> reads Idempotency-Key
  -> computes request body hash
  -> checks stored response through Softadastra SDK
  -> executes handler only if safe
  -> stores final response
  -> emits realtime event if needed

Vix
  -> sends HTTP response
  -> broadcasts WebSocket event through Vix WebSocket
```

## First MVP features

```txt
cnerium::App
cnerium::DurableRequest
cnerium::DurableResponse
cnerium::DurableRoute
cnerium::Idempotency
cnerium::ReplayProtection
cnerium::Store
cnerium::Realtime
cnerium::Event
```

## First MVP guarantees

```txt
A durable POST route requires Idempotency-Key.

The same request can be safely retried.

The same key with the same body returns the same response.

The same key with a different body returns 409 Conflict.

The user handler is not executed twice for the same completed request.

Realtime events are emitted through Vix WebSocket.
```

## Architecture

```txt
cnerium::App
  -> wraps vix::App
  -> owns AppRuntime
  -> registers durable routes into Vix

AppRuntime
  -> owns Softadastra SDK-backed Store
  -> owns Vix WebSocket adapter
  -> owns Vix runtime resources

Store
  -> uses softadastra::sdk::Client

DurableRoute
  -> uses Idempotency
  -> uses ReplayProtection
  -> uses Store

Realtime
  -> uses Vix WebSocket
```

## Repository sections

```txt
include/cnerium/app
  Application facade and runtime configuration.

include/cnerium/http
  Durable HTTP request, response, handler and route types.

include/cnerium/reliability
  Idempotency, replay protection and request hashing.

include/cnerium/store
  Softadastra SDK-backed storage facade.

include/cnerium/realtime
  Realtime event model and WebSocket facade.

include/cnerium/adapters
  Thin adapters for Vix HTTP, Vix WebSocket and Softadastra SDK.

include/cnerium/support
  Shared utility types: Error, Result, Json and String helpers.
```

## Example

The first example is:

```txt
examples/durable-orders-realtime
```

Run it with:

```bash
vix build
vix run
```

Create an order:

```bash
curl -i -X POST http://127.0.0.1:8080/orders \
  -H "Content-Type: application/json" \
  -H "Idempotency-Key: order-123" \
  -d '{"product_id":"p1","quantity":2}'
```

Retry the same request:

```bash
curl -i -X POST http://127.0.0.1:8080/orders \
  -H "Content-Type: application/json" \
  -H "Idempotency-Key: order-123" \
  -d '{"product_id":"p1","quantity":2}'
```

Expected behavior:

```txt
Same response.
The handler is not executed again.
```

Reuse the same key with a different body:

```bash
curl -i -X POST http://127.0.0.1:8080/orders \
  -H "Content-Type: application/json" \
  -H "Idempotency-Key: order-123" \
  -d '{"product_id":"p2","quantity":1}'
```

Expected behavior:

```txt
409 Conflict
```

## WebSocket

Realtime is enabled through Vix WebSocket.

Default example endpoint:

```txt
ws://127.0.0.1:9090/ws
```

Cnerium uses Vix WebSocket internally.

It does not create its own WebSocket server.

## Documentation

Continue with:

```txt
architecture.md
getting-started.md
durable-routes.md
idempotency.md
realtime.md
softadastra-sdk.md
vix-integration.md
```
