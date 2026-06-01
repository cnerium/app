# Vix Integration

Cnerium uses Vix as its runtime, HTTP, WebSocket and developer workflow foundation.
Cnerium does not replace Vix.
Cnerium builds on top of Vix to provide reliability-first application semantics.

## Role of Vix

Vix provides the application runtime layer used by Cnerium.

Cnerium relies on Vix for:

```txt
HTTP server
routing
request parsing
response writing
middleware execution
runtime executor
configuration
WebSocket server
WebSocket sessions
WebSocket broadcast
build workflow
developer workflow
```

Cnerium adds:

```txt
durable routes
idempotency
replay protection
stored responses
realtime application events
Softadastra SDK-backed persistence
```

## Main rule

The main rule is:

```txt
Cnerium must not recreate Vix.
```

That means Cnerium must not implement its own:

```txt
HTTP server
HTTP session
HTTP parser
router tree
middleware engine
TLS layer
WebSocket server
WebSocket protocol
WebSocket session
runtime executor
build workflow
```

Vix already provides those.

## Cnerium integration point

Cnerium integrates at the `vix::App` level.

```txt
cnerium::App
  -> owns vix::App
  -> registers routes into vix::App
  -> runs through vix::App::run()
```

Cnerium should not go below `vix::App`.

## HTTP integration

Cnerium durable routes are registered as normal Vix routes.

Application code:

```cpp
app.durable_post(
    "/orders",
    "orders.create",
    handler);
```

Internally:

```txt
cnerium::App
  -> app_.post(...)
      -> DurableRoute
      -> VixHttp adapter
      -> vix::http::ResponseWrapper
```

Vix still owns:

```txt
socket accept
HTTP session
request parsing
route matching
response serialization
connection lifecycle
```

Cnerium only owns the reliability layer around the handler.

## Request integration

Cnerium wraps Vix requests with:

```cpp
cnerium::DurableRequest
```

Internally, it references:

```cpp
vix::http::Request
```

`DurableRequest` exposes useful data:

```cpp
request.method();
request.target();
request.path();
request.body();
request.header("Content-Type");
request.idempotency_key();
request.idempotency_key_value();
request.request_hash();
request.json();
request.try_json();
request.param("id");
request.query("page");
request.native();
```

The underlying request remains owned by Vix.

## Response integration

Cnerium handlers return:

```cpp
cnerium::DurableResponse
```

Then the adapter writes it into:

```cpp
vix::http::ResponseWrapper
```

Flow:

```txt
DurableResponse
  -> VixHttp::write_response()
  -> ResponseWrapper.status()
  -> ResponseWrapper.type()
  -> ResponseWrapper.header()
  -> ResponseWrapper.send()
```

Cnerium does not serialize HTTP responses directly.

Vix writes the final response to the client.

## Durable route flow

```txt
Client
  -> POST /orders

Vix
  -> receives request
  -> parses request
  -> matches route
  -> calls Cnerium route lambda

Cnerium
  -> wraps request as DurableRequest
  -> reads Idempotency-Key
  -> computes request hash
  -> checks replay protection
  -> executes handler only if safe
  -> stores response through Softadastra SDK
  -> returns DurableResponse

Vix
  -> sends HTTP response
```

## WebSocket integration

Cnerium uses Vix WebSocket for realtime events.

Application code:

```cpp
app.realtime("/ws", "0.0.0.0", 9090);

app.emit(
    "order.created",
    cnerium::support::object({
        {"order_id", cnerium::Json("ord_123")},
    }));
```

Internally:

```txt
cnerium::App
  -> AppRuntime
  -> VixWebSocket adapter
  -> vix::websocket::Server
```

Vix WebSocket owns:

```txt
WebSocket server
sessions
protocol
frame parsing
connection lifecycle
ping/pong
broadcast
rooms
metrics
long-polling bridge
```

Cnerium owns only:

```txt
Event
EventPayload
RealtimeConfig
emit()
emit_to()
```

## Realtime flow

```txt
Durable handler
  -> app.emit("order.created", payload)

Cnerium
  -> creates Event
  -> converts EventPayload to Vix JSON payload
  -> calls Vix WebSocket server

Vix
  -> broadcasts event to connected clients
```

## VixHttp adapter

`VixHttp` is a thin adapter.

It converts:

```txt
cnerium::DurableResponse
```

into:

```txt
vix::http::ResponseWrapper
```

It provides:

```cpp
VixHttp::write_response(response, output);
VixHttp::execute_route(route, request, output);
```

It does not create a server or router.

## VixWebSocket adapter

`VixWebSocket` is a thin adapter.

It provides:

```cpp
start(config, core_config, executor);
stop();
emit(event);
emit_to(room, event);
to_vix_payload(payload);
```

It delegates to:

```cpp
vix::websocket::Server
```

It does not implement WebSocket itself.

## Runtime integration

Cnerium runtime owns a shared Vix executor:

```txt
AppRuntime
  -> vix::executor::RuntimeExecutor
```

This executor is used by the Vix WebSocket adapter.

The HTTP application remains executed through `vix::App`.

## Configuration integration

Cnerium has its own config:

```cpp
cnerium::app::AppConfig
```

It contains Cnerium-level options:

```txt
name
data_dir
node_id
vix_config_path
realtime config
```

Vix still owns Vix-specific configuration.

Cnerium loads Vix config through:

```cpp
vix::config::Config
```

## Build workflow

Cnerium should use the normal Vix workflow:

```bash
vix build
vix run
vix dev
vix tests
```

Cnerium should not create a separate build workflow.

Vix remains the developer workflow foundation.

## Project workflow

A Cnerium application should feel like a Vix application with reliability-first features.

Example:

```bash
vix new api --template backend
vix add softadastra/cnerium@0.1.0
vix install
vix dev
```

For local development, Cnerium can be linked directly from the local repository.

## Why Cnerium should not duplicate Vix

Vix already provides a stable runtime foundation.

Duplicating Vix would create:

```txt
another HTTP server
another WebSocket server
another routing system
another middleware model
another runtime lifecycle
another build workflow
more bugs
more stabilization work
```

Cnerium should stay focused on what makes it unique:

```txt
reliability-first application semantics
```

## What Vix owns

Vix owns:

```txt
App
HTTP server
router
Request
ResponseWrapper
middleware
config
runtime executor
WebSocket server
WebSocket session
broadcast
rooms
developer commands
templates
build workflow
production workflow
```

## What Cnerium owns

Cnerium owns:

```txt
durable_post()
DurableRequest
DurableResponse
DurableRoute
Idempotency
ReplayProtection
RequestHash
StoredResponse
Store facade
Realtime facade
Event model
Vix adapters
Softadastra SDK integration
```

## What Softadastra SDK owns

Softadastra SDK owns:

```txt
durable storage
WAL-backed persistence
sync pipeline
transport
discovery
metadata
local-first foundation
```

## Clean dependency direction

The dependency direction should stay clear:

```txt
Application code
  -> Cnerium
      -> Vix
      -> Softadastra SDK
```

Vix should not depend on Cnerium.

Softadastra SDK should not depend on Cnerium.

Cnerium depends on both.

## Example full flow

```txt
Client
  -> POST /orders
  -> Idempotency-Key: order-123

Vix HTTP
  -> receives request
  -> routes to Cnerium durable handler

Cnerium
  -> checks idempotency
  -> executes handler
  -> stores response through Softadastra SDK
  -> emits order.created

Vix WebSocket
  -> broadcasts order.created

Vix HTTP
  -> returns 201 Created
```

Retry flow:

```txt
Client retries same POST with same Idempotency-Key and same body

Vix HTTP
  -> routes to Cnerium

Cnerium
  -> finds stored hash
  -> finds stored response
  -> does not execute handler
  -> returns stored response

Vix HTTP
  -> sends same response
```

Conflict flow:

```txt
Client retries same Idempotency-Key with different body

Cnerium
  -> detects different body hash
  -> returns 409 Conflict

Vix HTTP
  -> sends conflict response
```

## MVP boundary

The MVP integration with Vix includes:

```txt
vix::App wrapping
durable POST route registration
Vix request wrapping
Vix response writing
Vix WebSocket event broadcast
Vix config usage
Vix executor usage
```

The MVP does not include:

```txt
custom Vix template changes
custom Vix middleware system
custom HTTP server
custom WebSocket protocol
custom TLS management
custom production workflow
```

## Future integration points

Future versions can integrate more deeply with Vix features:

```txt
OpenAPI route metadata
middleware helpers
auth middleware
validation module
Vix templates
Vix CLI templates
Vix registry packaging
Vix dev diagnostics
Vix tests workflow
Vix production workflow
```

But the rule stays the same:

```txt
Cnerium extends Vix.
Cnerium does not duplicate Vix.
```

## Summary

Cnerium uses Vix for runtime infrastructure.

Vix handles:

```txt
HTTP
routing
middleware
WebSocket
runtime
build
developer workflow
```

Cnerium adds:

```txt
durable route semantics
idempotency
replay protection
stored responses
realtime application events
Softadastra SDK-backed reliability
```

The integration is intentionally small, stable and focused.
