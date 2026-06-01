# Idempotency

Idempotency is the reliability rule that allows Cnerium to process the same durable request safely more than once.
In Cnerium, idempotency is used by durable routes to prevent duplicated writes when a client retries a request.

## Why idempotency matters

A client can retry a request when:

```txt
network timeout
connection reset
server response lost
mobile network interruption
proxy timeout
browser retry
API client retry
```

The dangerous case is:

```txt
server processed the request
but the client did not receive the response
```

Without idempotency, the retry may execute the operation again.

Example:

```txt
POST /orders
  -> creates order

client retries

POST /orders
  -> creates duplicate order
```

Cnerium prevents this.

## Cnerium rule

Every durable route requires:

```txt
Idempotency-Key
```

Example:

```txt
Idempotency-Key: order-123
```

Cnerium combines this key with a stable request body hash.

```txt
operation name
+ Idempotency-Key
+ request body hash
```

Then it decides whether to execute, replay, reject or fail.

## Decision table

```txt
Missing Idempotency-Key
  -> 400 Bad Request

New Idempotency-Key
  -> execute handler
  -> store request hash
  -> store response

Same Idempotency-Key + same body
  -> replay stored response
  -> handler is not executed again

Same Idempotency-Key + different body
  -> 409 Conflict
  -> handler is not executed
```

## Example request

```bash
curl -i -X POST http://127.0.0.1:8080/orders \
  -H "Content-Type: application/json" \
  -H "Idempotency-Key: order-123" \
  -d '{"product_id":"p1","quantity":2}'
```

Cnerium reads:

```txt
operation: orders.create
idempotency key: order-123
body: {"product_id":"p1","quantity":2}
```

Then it computes:

```txt
request hash: stable hash of the body
```

## Storage keys

Cnerium stores idempotency metadata using namespaced keys.

For the operation:

```txt
orders.create
```

And idempotency key:

```txt
order-123
```

Cnerium writes:

```txt
cnerium:request_hash:orders.create:order-123
cnerium:response:orders.create:order-123
```

The request hash key stores the body hash.

The response key stores the replayable HTTP response.

## First request flow

```txt
Client
  -> POST /orders
  -> Idempotency-Key: order-123

Cnerium
  -> reads key
  -> computes request hash
  -> checks cnerium:request_hash:orders.create:order-123
  -> no stored hash found
  -> executes handler
  -> stores hash
  -> stores response
  -> returns response
```

Expected response:

```txt
201 Created
```

## Retry with same body

```bash
curl -i -X POST http://127.0.0.1:8080/orders \
  -H "Content-Type: application/json" \
  -H "Idempotency-Key: order-123" \
  -d '{"product_id":"p1","quantity":2}'
```

Cnerium sees:

```txt
stored hash exists
new hash matches stored hash
stored response exists
```

So it returns the stored response.

The handler is not executed again.

## Retry with different body

```bash
curl -i -X POST http://127.0.0.1:8080/orders \
  -H "Content-Type: application/json" \
  -H "Idempotency-Key: order-123" \
  -d '{"product_id":"p2","quantity":1}'
```

Cnerium sees:

```txt
stored hash exists
new hash does not match stored hash
```

So it returns:

```txt
409 Conflict
```

This prevents unsafe reuse of the same key for a different operation.

## Main classes

## `IdempotencyKey`

`IdempotencyKey` represents the value of the `Idempotency-Key` HTTP header.

```cpp
cnerium::reliability::IdempotencyKey key{"order-123"};

if (key.is_valid())
{
  // key can be used
}
```

A valid key must not be empty.

## `RequestHash`

`RequestHash` represents the stable hash of the request body.

```cpp
auto hash =
    cnerium::reliability::RequestHash::from_body(
        R"({"product_id":"p1","quantity":2})");
```

Cnerium does not use `std::hash`.

The MVP uses deterministic FNV-1a 64-bit hashing because the hash must be stable.

## `ReplayProtection`

`ReplayProtection` checks whether a request should execute, replay, conflict or fail.

```cpp
cnerium::reliability::ReplayProtection replay{store};

auto result =
    replay.check(
        "orders.create",
        key,
        hash);
```

It returns a `DurableResult`.

## `DurableResult`

`DurableResult` tells the durable route what to do.

Possible actions:

```txt
Execute
Replay
Conflict
Invalid
```

Example:

```cpp
if (result.should_execute())
{
  // run handler
}

if (result.should_replay())
{
  // return stored response
}

if (result.is_conflict())
{
  // return 409 Conflict
}

if (result.is_invalid())
{
  // return 400 Bad Request
}
```

## `Idempotency`

`Idempotency` is the high-level service used by durable routes.

```cpp
cnerium::reliability::Idempotency idempotency{store};

auto result =
    idempotency.check(
        "orders.create",
        key,
        request_body);
```

After the handler succeeds:

```cpp
idempotency.commit(
    "orders.create",
    key,
    request_body,
    stored_response);
```

## Internal flow

```txt
DurableRoute
  -> DurableRequest
      -> Idempotency-Key
      -> request body
  -> Idempotency
      -> RequestHash
      -> ReplayProtection
          -> Store
              -> Softadastra SDK Client
```

## What gets stored

Cnerium stores two main records.

### Request hash

```txt
key:
cnerium:request_hash:orders.create:order-123

value:
stable request body hash
```

### Stored response

```txt
key:
cnerium:response:orders.create:order-123

value:
{
  "status_code": 201,
  "body": "...",
  "content_type": "application/json; charset=utf-8"
}
```

## Stored response replay

A stored response contains:

```txt
status_code
body
content_type
```

This lets Cnerium return the same response later.

Example:

```txt
First request:
  -> executes handler
  -> returns 201 Created
  -> stores response

Retry:
  -> does not execute handler
  -> returns stored 201 Created response
```

## Handler execution rule

The user handler runs only when Cnerium returns:

```txt
DurableResult::Execute
```

The handler does not run when:

```txt
DurableResult::Replay
DurableResult::Conflict
DurableResult::Invalid
```

This is the main safety rule.

## Why same key with different body is rejected

An idempotency key must represent one logical operation.

This is safe:

```txt
key: order-123
body: {"product_id":"p1","quantity":2}
```

Retrying the same body is safe.

This is unsafe:

```txt
key: order-123
body: {"product_id":"p2","quantity":1}
```

The same key now points to a different operation.

Cnerium rejects it with:

```txt
409 Conflict
```

## Idempotency and operation names

The operation name is important.

Example:

```cpp
app.durable_post(
    "/orders",
    "orders.create",
    handler);
```

The operation name separates idempotency data between different route operations.

Example keys:

```txt
cnerium:request_hash:orders.create:order-123
cnerium:request_hash:payments.create:order-123
```

Even if the same idempotency key is reused, the operation namespace is different.

## Best practices

Use stable operation names:

```txt
orders.create
payments.create
users.register
invoices.create
notifications.send
```

Use clear idempotency keys from the client:

```txt
order-123
payment-456
registration-789
```

Do not reuse the same key for different request bodies.

Do not generate a new idempotency key for every retry.

The client must reuse the same key when retrying the same logical operation.

## Good client behavior

```txt
Generate Idempotency-Key once
Send request
If timeout happens, retry with the same key and same body
Use the response returned by Cnerium
```

## Bad client behavior

```txt
Generate a new Idempotency-Key for every retry
Reuse the same key with a different body
Omit Idempotency-Key on durable routes
```

## Relationship with Softadastra SDK

Cnerium does not implement a new durable store.

Cnerium stores idempotency data through:

```txt
cnerium::store::Store
  -> softadastra::sdk::Client
```

Softadastra SDK provides the durability foundation.

Cnerium provides the application-level idempotency semantics.

## Relationship with Vix

Vix handles:

```txt
HTTP request parsing
routing
response sending
runtime execution
```

Cnerium handles:

```txt
Idempotency-Key
request hash
replay protection
stored response
handler execution safety
```

## MVP limitation

The MVP applies idempotency to:

```txt
durable_post()
```

Future versions can apply the same model to:

```txt
durable_put()
durable_patch()
durable_delete()
jobs
webhooks
outbox events
auth flows
```

## Summary

Cnerium idempotency guarantees:

```txt
A completed durable request can be retried safely.

The same key and same body returns the same response.

The same key with a different body returns 409 Conflict.

The durable handler is not executed twice for the same completed request.

Idempotency metadata is stored through the Softadastra SDK.

HTTP execution remains handled by Vix.
```
