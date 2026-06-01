# Architecture

Cnerium is the reliability-first application framework for the Vix and Softadastra ecosystem.

Its architecture is intentionally small:

```txt
Vix
  -> runtime, HTTP, routing, middleware, WebSocket, build and developer workflow

Softadastra SDK
  -> durable storage, WAL-backed persistence, sync and local-first foundation

Cnerium
  -> durable routes, idempotency, replay protection and realtime application events
```

Cnerium does not replace Vix.

Cnerium does not recreate Softadastra internals.

Cnerium connects both into a framework-level application API.

## Architecture rule

The main rule is:

```txt
Cnerium must not go below vix::App.
```

That means Cnerium must not own or recreate:

```txt
HTTP server
HTTP session
HTTP parser
Router tree
Middleware engine
TLS
WebSocket protocol
WebSocket session
Sync engine
WAL engine
Transport engine
Discovery engine
```

Those already belong to Vix or Softadastra SDK.

Cnerium owns only the application reliability layer.

## Layer overview

```txt
Application code
  -> cnerium::App
      -> durable_post()
      -> realtime()
      -> emit()

Cnerium app layer
  -> App
  -> AppConfig
  -> AppRuntime

Cnerium HTTP layer
  -> DurableRequest
  -> DurableResponse
  -> DurableHandler
  -> DurableRoute

Cnerium reliability layer
  -> Idempotency
  -> ReplayProtection
  -> RequestHash
  -> IdempotencyKey
  -> DurableResult

Cnerium store layer
  -> Store
  -> StoreKey
  -> StoredResponse

Cnerium realtime layer
  -> Realtime
  -> RealtimeConfig
  -> Event
  -> EventPayload

Cnerium adapters
  -> VixHttp
  -> VixWebSocket
  -> SoftadastraStore

External foundations
  -> vix::App
  -> vix::http::Request
  -> vix::http::ResponseWrapper
  -> vix::websocket::Server
  -> softadastra::sdk::Client
```

## Main components

## `cnerium::App`

`cnerium::App` is the public application facade.

It owns:

```txt
vix::App
AppRuntime
```

It exposes:

```cpp
app.durable_post("/orders", "orders.create", handler);
app.realtime("/ws", "0.0.0.0", 9090);
app.emit("order.created", payload);
app.run();
```

`cnerium::App` does not create a new HTTP server.

It registers routes into the underlying `vix::App`.

## `AppRuntime`

`AppRuntime` owns the runtime resources used by Cnerium.

It owns:

```txt
AppConfig
vix::config::Config
vix::executor::RuntimeExecutor
cnerium::store::Store
cnerium::adapters::VixWebSocket
```

Its job is to start and stop framework-level resources:

```txt
open Softadastra SDK store
start Vix WebSocket if realtime is enabled
stop WebSocket
close store
```

It does not run the HTTP server directly.

The HTTP server remains owned by Vix.

## `Store`

`Store` is the Cnerium storage facade.

It wraps the public Softadastra SDK client:

```txt
softadastra::sdk::Client
```

It stores Cnerium framework data using namespaced keys:

```txt
cnerium:request_hash:<operation>:<idempotency-key>
cnerium:response:<operation>:<idempotency-key>
cnerium:event:<event-id>
```

Cnerium must not use internal Softadastra modules directly.

It must use only the public SDK surface.

## `DurableRoute`

`DurableRoute` is the core durable HTTP executor.

It receives a Vix request and returns a Cnerium durable response:

```txt
vix::http::Request
  -> DurableRoute
      -> DurableRequest
      -> Idempotency
      -> DurableHandler
      -> DurableResponse
```

Its flow is:

```txt
read Idempotency-Key
compute request hash
check replay protection
replay stored response if possible
reject unsafe replay with 409 Conflict
execute user handler for new requests
store final response
return response
```

## `DurableRequest`

`DurableRequest` wraps `vix::http::Request`.

It exposes:

```txt
method()
target()
path()
body()
header()
idempotency_key()
idempotency_key_value()
request_hash()
json()
try_json()
param()
query()
native()
```

It does not own request data.

It only references the Vix request.

## `DurableResponse`

`DurableResponse` is the response returned by durable handlers.

It contains:

```txt
status code
body
content type
```

It can be converted into `StoredResponse` so Cnerium can replay the same response later.

## `Idempotency`

`Idempotency` is the high-level reliability service.

It coordinates:

```txt
RequestHash
ReplayProtection
StoredResponse
Store
```

Its rules are:

```txt
missing Idempotency-Key
  -> invalid

new key
  -> execute handler

same key + same body
  -> replay stored response

same key + different body
  -> conflict
```

## `ReplayProtection`

`ReplayProtection` checks existing stored request metadata.

It reads:

```txt
cnerium:request_hash:<operation>:<key>
cnerium:response:<operation>:<key>
```

Then it returns a `DurableResult`:

```txt
Execute
Replay
Conflict
Invalid
```

## `RequestHash`

`RequestHash` computes a stable body hash.

Cnerium does not use `std::hash`, because `std::hash` is not guaranteed to be stable across implementations.

The MVP uses deterministic FNV-1a 64-bit hashing.

## `Realtime`

`Realtime` exposes application-level realtime events.

It does not implement WebSocket itself.

It delegates to Vix WebSocket through `VixWebSocket`.

Application code uses:

```cpp
app.emit("order.created", payload);
app.emit_to("orders", "order.created", payload);
```

## `VixHttp`

`VixHttp` converts a Cnerium `DurableResponse` into a Vix `ResponseWrapper`.

It does not create:

```txt
server
router
session
middleware
parser
```

It only writes the response.

## `VixWebSocket`

`VixWebSocket` adapts Cnerium realtime events to Vix WebSocket.

It handles:

```txt
start Vix WebSocket server
stop Vix WebSocket server
broadcast event
broadcast event to room
convert EventPayload to vix::json::kvs
```

It does not implement:

```txt
WebSocket protocol
frame parsing
sessions
heartbeat
rooms
long-polling
```

Those belong to Vix.

## Request lifecycle

A durable HTTP request follows this path:

```txt
Client
  -> POST /orders
  -> Idempotency-Key: order-123

Vix HTTP
  -> receives request
  -> parses request
  -> matches route
  -> calls registered Cnerium route lambda

Cnerium
  -> wraps request as DurableRequest
  -> reads Idempotency-Key
  -> computes RequestHash
  -> checks Idempotency
  -> executes handler only if safe
  -> stores response through Store
  -> emits realtime event if needed
  -> returns DurableResponse

Vix HTTP
  -> sends response to client
```

## Idempotency lifecycle

First request:

```txt
POST /orders
Idempotency-Key: order-123
Body: {"product_id":"p1","quantity":2}

Cnerium:
  -> request hash not found
  -> execute handler
  -> store request hash
  -> store response
  -> return 201 Created
```

Retry with same key and same body:

```txt
POST /orders
Idempotency-Key: order-123
Body: {"product_id":"p1","quantity":2}

Cnerium:
  -> request hash found
  -> hash matches
  -> response found
  -> return stored response
  -> handler is not executed again
```

Retry with same key and different body:

```txt
POST /orders
Idempotency-Key: order-123
Body: {"product_id":"p2","quantity":1}

Cnerium:
  -> request hash found
  -> hash does not match
  -> return 409 Conflict
  -> handler is not executed
```

## Realtime lifecycle

A durable handler can emit a realtime event:

```cpp
app.emit(
    "order.created",
    cnerium::support::object({
        {"order_id", cnerium::Json(order_id)},
        {"product_id", cnerium::Json(product_id)},
        {"quantity", cnerium::Json(quantity)},
    }));
```

Flow:

```txt
Cnerium handler
  -> app.emit()
  -> AppRuntime
  -> VixWebSocket adapter
  -> vix::websocket::Server
  -> connected clients
```

## Repository architecture

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

## Ownership model

```txt
cnerium::App
  owns vix::App
  owns AppRuntime

AppRuntime
  owns Store
  owns VixWebSocket
  owns Vix config
  owns Vix executor

Store
  owns softadastra::sdk::Client

VixWebSocket
  owns vix::websocket::Server

DurableRoute
  references Store
  owns operation name
  owns user handler

DurableRequest
  references vix::http::Request
```

## Dependency direction

The dependency direction must stay one-way:

```txt
support
  -> no Cnerium high-level dependency

store
  -> support
  -> Softadastra SDK

reliability
  -> store
  -> support

http
  -> reliability
  -> store
  -> Vix HTTP

realtime
  -> support
  -> Vix WebSocket types

adapters
  -> http
  -> realtime
  -> Vix
  -> Softadastra SDK

app
  -> all Cnerium layers
  -> Vix app
  -> Softadastra SDK through Store
```

Cnerium modules must not depend on application code.

## What Cnerium must not contain

Cnerium must not contain folders like:

```txt
server/
session/
router/
http_parser/
websocket_protocol/
websocket_session/
tls/
wal_engine/
sync_engine/
transport/
discovery/
```

Those would duplicate Vix or Softadastra.

## MVP boundary

The first version of Cnerium should stay focused on:

```txt
durable_post()
Idempotency-Key
request body hash
stored response replay
409 conflict on unsafe replay
Softadastra SDK-backed store
realtime event emission through Vix WebSocket
```

Everything else can come later.

## Future extensions

Future versions can add:

```txt
durable_put()
durable_patch()
durable_delete()
auth
sessions
API tokens
permissions
jobs
outbox
audit logs
webhooks
OpenAPI metadata
long-polling fallback
room-level realtime helpers
```

But the core rule remains:

```txt
Cnerium owns reliability semantics.
Vix owns runtime and server infrastructure.
Softadastra SDK owns durable storage and sync foundations.
```
