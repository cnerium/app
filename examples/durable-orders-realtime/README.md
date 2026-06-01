# Durable Orders Realtime Example

This example shows the first Cnerium MVP use case:

```txt
durable HTTP POST
+ Idempotency-Key
+ Softadastra SDK-backed storage
+ Vix WebSocket realtime event
```

## Run

```bash
vix build
vix run
```

## Create an order

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

The order is created once and Cnerium emits:

```txt
order.created
```

## Retry the same request

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

## Reuse the same key with a different body

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

Realtime is enabled on:

```txt
ws://127.0.0.1:9090/ws
```

Cnerium uses Vix WebSocket internally.

It does not create its own WebSocket server.
