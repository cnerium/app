# Getting Started

This guide shows how to create and run the first Cnerium MVP application.

The goal is to build a small backend that supports:

```txt
durable HTTP POST
+ Idempotency-Key
+ Softadastra SDK-backed storage
+ Vix WebSocket realtime event
```

## Requirements

You need:

```txt
Vix installed
Softadastra SDK available
Cnerium built or available as a dependency
C++20 compiler
```

Cnerium is built on top of Vix and the Softadastra SDK.

It does not replace them.

## Create a project

Create a Vix project:

```bash
vix new durable-orders --template backend
cd durable-orders
```

Add Cnerium as a dependency when it is available through the Vix registry:

```bash
vix add softadastra/cnerium@0.1.0
vix install
```

For local development, you can link Cnerium from your local repository instead.

## Minimal application

Create or update your `src/main.cpp`:

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

## Build

```bash
vix build
```

## Run

```bash
vix run
```

The HTTP API should be available on:

```txt
http://127.0.0.1:8080
```

The WebSocket endpoint should be available on:

```txt
ws://127.0.0.1:9090/ws
```

## Create an order

Send a durable POST request:

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

The handler executes once.

Cnerium stores the response through the Softadastra SDK-backed store.

Cnerium also emits a realtime event:

```txt
order.created
```

## Retry the same request

Send the same request again:

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

Cnerium detects:

```txt
same Idempotency-Key
same request body hash
stored response exists
```

So it replays the stored response.

## Reuse the same key with a different body

Send a different body with the same key:

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

Cnerium rejects the request because the same idempotency key was reused with a different body.

## Missing Idempotency-Key

Send a request without `Idempotency-Key`:

```bash
curl -i -X POST http://127.0.0.1:8080/orders \
  -H "Content-Type: application/json" \
  -d '{"product_id":"p1","quantity":2}'
```

Expected behavior:

```txt
400 Bad Request
```

Durable routes require `Idempotency-Key`.

## What happens internally

```txt
Client
  -> POST /orders
  -> Idempotency-Key: order-123

Vix
  -> receives the HTTP request
  -> routes it to Cnerium

Cnerium
  -> wraps the request as DurableRequest
  -> reads Idempotency-Key
  -> hashes the request body
  -> checks replay protection
  -> executes the handler only if safe
  -> stores the response
  -> emits realtime event

Vix
  -> sends the HTTP response
  -> broadcasts WebSocket event through Vix WebSocket
```

## Project files

A minimal Cnerium app usually contains:

```txt
src/main.cpp
vix.app
vix.json
data/cnerium/
```

Cnerium stores framework data under:

```txt
data/cnerium
```

The first MVP uses the Softadastra SDK client as the durable foundation.

## Next steps

Continue with:

```txt
architecture.md
durable-routes.md
idempotency.md
realtime.md
softadastra-sdk.md
vix-integration.md
```
