# Cnerium

Cnerium is the official reliability-first application framework for the Vix and Softadastra ecosystem.

It brings together:

```txt
Vix
  -> runtime, HTTP, WebSocket, routing, middleware, build and developer workflow

Softadastra SDK
  -> durable storage, WAL-backed persistence, sync and local-first foundations

Cnerium
  -> reliability-first application layer for durable APIs and realtime apps
```

Cnerium does not replace Vix.

Cnerium does not recreate the Softadastra SDK.

Cnerium uses both to provide a clean application framework API for systems that must stay correct under retries, crashes, timeouts and unstable networks.

## What Cnerium is

Cnerium is a backend-focused framework for building reliable applications on top of Vix and Softadastra.

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

Cnerium is a different direction:

```txt
100% reliability-first application framework
built on top of Vix and Softadastra.
```

## Why Cnerium exists

A normal backend route can fail badly when a client retries a request.

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

## Durable route behavior

Create an order:

```bash
curl -i -X POST http://127.0.0.1:8080/orders \
  -H "Content-Type: application/json" \
  -H "Idempotency-Key: order-123" \
  -d '{"product_id":"p1","quantity":2}'
```

Expected behavior:

```txt
201 Created
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

## WebSocket realtime

Cnerium can emit realtime events through Vix WebSocket.

Example endpoint:

```txt
ws://127.0.0.1:9090/ws
```

Example event:

```txt
order.created
```

Cnerium does not create its own WebSocket server.

It uses Vix WebSocket internally.

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

## Repository structure

```txt
include/cnerium/
  cnerium.hpp
  Version.hpp

include/cnerium/app/
  App.hpp
  AppConfig.hpp
  AppRuntime.hpp

include/cnerium/http/
  DurableRequest.hpp
  DurableResponse.hpp
  DurableHandler.hpp
  DurableRoute.hpp

include/cnerium/reliability/
  Idempotency.hpp
  IdempotencyKey.hpp
  RequestHash.hpp
  ReplayProtection.hpp
  DurableResult.hpp

include/cnerium/store/
  Store.hpp
  StoreKey.hpp
  StoredResponse.hpp

include/cnerium/realtime/
  Realtime.hpp
  RealtimeConfig.hpp
  Event.hpp
  EventPayload.hpp

include/cnerium/adapters/
  VixHttp.hpp
  VixWebSocket.hpp
  SoftadastraStore.hpp

include/cnerium/support/
  Error.hpp
  Result.hpp
  Json.hpp
  String.hpp
```

## Build

```bash
vix build
```

## Run tests

```bash
vix tests
```

## Example

The first example is:

```txt
examples/durable-orders-realtime
```

Run it:

```bash
cd examples/durable-orders-realtime
vix build
vix run
```

## MVP features

```txt
cnerium::App
durable_post()
Idempotency-Key
request body hash
stored response replay
409 Conflict on unsafe replay
Softadastra SDK-backed store
realtime()
emit()
Vix WebSocket integration
```

## Design rules

Cnerium must not duplicate Vix.

Vix owns:

```txt
HTTP server
router
middleware
Request
ResponseWrapper
runtime executor
WebSocket server
WebSocket sessions
broadcast
rooms
developer workflow
```

Cnerium must not duplicate Softadastra SDK internals.

Softadastra SDK owns:

```txt
durable storage
WAL-backed persistence
sync pipeline
transport
discovery
metadata
```

Cnerium owns:

```txt
durable route semantics
idempotency
replay protection
stored response replay
application-level realtime events
```

## Documentation

```txt
docs/index.md
docs/architecture.md
docs/getting-started.md
docs/durable-routes.md
docs/idempotency.md
docs/realtime.md
docs/softadastra-sdk.md
docs/vix-integration.md
```

## Status

Cnerium is currently in MVP development.

The first stable goal is:

```txt
durable HTTP action
+ retry-safe response replay
+ realtime WebSocket event
```

## License

Cnerium is licensed under the MIT License.
