# Durable Routes

Durable routes are the core feature of Cnerium.
A durable route is a normal Vix HTTP route wrapped with Cnerium reliability rules.
It protects critical backend operations from unsafe retries, duplicated requests and unstable network behavior.

## Why durable routes exist

A normal HTTP request can fail after the server has already processed it.

Example:

```txt
Client sends POST /orders
Server creates the order
Network cuts before the client receives the response
Client retries the request
Server creates the same order again
```

This is a common backend reliability problem.

Cnerium solves it with durable route semantics.

## Durable route rule

A durable route requires an `Idempotency-Key` header.

```txt
Idempotency-Key: order-123
```

Cnerium uses this key with the request body hash to decide what to do.

```txt
same Idempotency-Key + same body
  -> return the same stored response

same Idempotency-Key + different body
  -> return 409 Conflict

missing Idempotency-Key
  -> return 400 Bad Request
```

## Register a durable route

Use `durable_post()`:

```cpp
#include <cnerium/cnerium.hpp>

int main()
{
  cnerium::App app;

  app.durable_post(
      "/orders",
      "orders.create",
      [](cnerium::DurableRequest &request)
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

## Parameters

```cpp
app.durable_post(path, operation, handler);
```

### `path`

The HTTP path registered into the underlying Vix app.

Example:

```txt
/orders
```

### `operation`

A stable operation name used by Cnerium idempotency.

Example:

```txt
orders.create
```

The operation name is part of the internal storage key.

```txt
cnerium:request_hash:orders.create:order-123
cnerium:response:orders.create:order-123
```

### `handler`

The user-defined durable handler.

```cpp
[](cnerium::DurableRequest &request)
{
  return cnerium::created(...);
}
```

The handler is executed only when Cnerium decides the request is safe to process.

## Request lifecycle

```txt
Vix receives HTTP request
  -> Vix router matches the route
  -> Cnerium DurableRoute executes
  -> Cnerium reads Idempotency-Key
  -> Cnerium computes request hash
  -> Cnerium checks stored metadata
  -> Cnerium decides Execute, Replay, Conflict or Invalid
```

## First request

```bash
curl -i -X POST http://127.0.0.1:8080/orders \
  -H "Content-Type: application/json" \
  -H "Idempotency-Key: order-123" \
  -d '{"product_id":"p1","quantity":2}'
```

Cnerium checks storage:

```txt
request hash not found
stored response not found
```

Cnerium executes the handler.

Then it stores:

```txt
cnerium:request_hash:orders.create:order-123
cnerium:response:orders.create:order-123
```

Expected response:

```txt
201 Created
```

## Retry with same key and same body

```bash
curl -i -X POST http://127.0.0.1:8080/orders \
  -H "Content-Type: application/json" \
  -H "Idempotency-Key: order-123" \
  -d '{"product_id":"p1","quantity":2}'
```

Cnerium checks storage:

```txt
request hash found
hash matches
stored response found
```

Cnerium does not execute the handler again.

It returns the stored response.

Expected behavior:

```txt
Same response.
The handler is not executed again.
```

## Retry with same key and different body

```bash
curl -i -X POST http://127.0.0.1:8080/orders \
  -H "Content-Type: application/json" \
  -H "Idempotency-Key: order-123" \
  -d '{"product_id":"p2","quantity":1}'
```

Cnerium checks storage:

```txt
request hash found
hash does not match
```

Cnerium rejects the request.

Expected response:

```txt
409 Conflict
```

## Missing Idempotency-Key

```bash
curl -i -X POST http://127.0.0.1:8080/orders \
  -H "Content-Type: application/json" \
  -d '{"product_id":"p1","quantity":2}'
```

Cnerium rejects the request.

Expected response:

```txt
400 Bad Request
```

Durable routes require `Idempotency-Key`.

## DurableRequest

`DurableRequest` wraps `vix::http::Request`.

It exposes the useful request data for durable handlers.

```cpp
request.method();
request.target();
request.path();
request.body();
request.header("Content-Type");
request.has_header("Idempotency-Key");
request.idempotency_key();
request.idempotency_key_value();
request.request_hash();
request.json();
request.try_json();
request.param("id");
request.query("page");
request.native();
```

## DurableResponse

`DurableResponse` is the response returned by a durable handler.

```cpp
return cnerium::json(
    200,
    cnerium::support::object({
        {"ok", cnerium::Json(true)},
    }));
```

Common helpers:

```cpp
cnerium::json(body);
cnerium::json(200, body);
cnerium::created(body);
cnerium::text(200, "OK");

cnerium::DurableResponse::bad_request("Invalid body");
cnerium::DurableResponse::conflict("Duplicate request conflict");
cnerium::DurableResponse::internal_error("Internal error");
```

## Stored response

After the handler succeeds, Cnerium stores the response.

The stored response contains:

```txt
status_code
body
content_type
```

This lets Cnerium replay the exact same response later.

## Internal flow

```txt
DurableRoute
  -> DurableRequest
  -> Idempotency
      -> RequestHash
      -> ReplayProtection
      -> Store
          -> Softadastra SDK Client
  -> DurableHandler
  -> DurableResponse
  -> StoredResponse
```

## Relationship with Vix

Cnerium does not create a new HTTP server.

Cnerium uses Vix.

```txt
vix::App
  -> owns HTTP server
  -> owns router
  -> owns middleware execution
  -> owns request and response lifecycle

cnerium::App
  -> wraps vix::App
  -> registers durable routes into Vix
```

A durable route is still executed by the Vix HTTP runtime.

Cnerium only adds reliability rules around the handler.

## Relationship with Softadastra SDK

Cnerium does not create a new WAL engine or sync engine.

Cnerium uses the public Softadastra SDK client through `cnerium::store::Store`.

```txt
cnerium::store::Store
  -> softadastra::sdk::Client
```

The SDK is responsible for durable storage and local-first foundations.

## When to use durable routes

Use durable routes for operations that must not be executed twice accidentally.

Examples:

```txt
create order
create payment intent
register user
send critical notification
submit form
start workflow
create invoice
reserve stock
```

## When not to use durable routes

Do not use durable routes for simple read-only operations.

Use normal Vix routes for:

```txt
GET /products
GET /health
GET /status
GET /users/{id}
```

Durable routes are mainly for state-changing operations.

## MVP limitation

The first MVP supports:

```txt
durable_post()
```

Future versions can add:

```txt
durable_put()
durable_patch()
durable_delete()
```

The first version stays focused on one strong use case:

```txt
durable HTTP POST
+ idempotency
+ replay protection
+ stored response
```

## Summary

A Cnerium durable route guarantees:

```txt
The handler does not run twice for the same completed request.

Retries with the same body return the same response.

Reusing the same key with a different body returns 409 Conflict.

Missing Idempotency-Key returns 400 Bad Request.

Storage is handled through the Softadastra SDK.

HTTP execution remains handled by Vix.
```
