# Softadastra SDK

Cnerium uses the Softadastra SDK as its durable storage and local-first foundation.

Cnerium does not access Softadastra internal modules directly.

It uses the public SDK API.

## Role of the SDK

The Softadastra SDK provides the durable foundation used by Cnerium.

In the MVP, Cnerium uses it to store:

```txt
request hashes
stored HTTP responses
framework metadata
```

The important public type is:

```cpp
softadastra::sdk::Client
```

Cnerium wraps this client through:

```txt
cnerium::store::Store
```

## Public SDK surface used by Cnerium

Cnerium depends on the public SDK layer:

```txt
softadastra::sdk::Client
softadastra::sdk::ClientOptions
softadastra::sdk::Key
softadastra::sdk::Value
softadastra::sdk::Result
softadastra::sdk::Error
softadastra::sdk::SyncState
softadastra::sdk::TickResult
```

Cnerium must not depend on internal SDK runtime types.

## What Cnerium must not use directly

Cnerium must not use internal Softadastra modules such as:

```txt
StoreEngine
SyncEngine
SyncScheduler
TransportEngine
DiscoveryEngine
MetadataService
Runtime
RuntimeBuilder
internal/*
```

Those are SDK internals.

Cnerium should treat them as implementation details.

## Client

The SDK client is the main entry point.

Cnerium uses it through `Store`.

Basic SDK operations used by Cnerium:

```cpp
client.open();
client.close();
client.is_open();

client.put(key, value);
client.get(key);
client.remove(key);
client.contains(key);

client.tick();
client.sync_state();
```

Cnerium does not expose the raw SDK client as its main API.

Application code should use Cnerium abstractions first.

## Key

Softadastra SDK keys are represented by:

```cpp
softadastra::sdk::Key
```

Cnerium converts its internal keys to SDK keys.

Example Cnerium keys:

```txt
cnerium:request_hash:orders.create:order-123
cnerium:response:orders.create:order-123
cnerium:event:event-123
```

The Cnerium key type is:

```cpp
cnerium::store::StoreKey
```

## Value

Softadastra SDK values are binary-safe values represented by:

```cpp
softadastra::sdk::Value
```

Cnerium stores framework data as text values.

Examples:

```txt
request hash string
serialized StoredResponse JSON
event metadata JSON
```

Cnerium converts text to SDK values before writing:

```cpp
softadastra::sdk::Value::from_string(value)
```

## Store

Cnerium uses this facade:

```cpp
cnerium::store::Store
```

It wraps:

```txt
softadastra::sdk::Client
```

The Store API exposes only what Cnerium needs:

```cpp
store.open();
store.close();
store.is_open();

store.put(key, value);
store.get(key);
store.contains(key);
store.remove(key);

store.put_response(key, response);
store.get_response(key);

store.tick();
```

## Durable storage in Cnerium

When a durable route succeeds, Cnerium stores two records.

For this route:

```txt
operation: orders.create
Idempotency-Key: order-123
```

Cnerium stores:

```txt
cnerium:request_hash:orders.create:order-123
cnerium:response:orders.create:order-123
```

The request hash value is a stable hash of the request body.

The response value is a serialized `StoredResponse`.

## Stored response format

A stored response contains:

```txt
status_code
body
content_type
```

Example:

```json
{
  "status_code": 201,
  "body": "{\"ok\":true,\"order_id\":\"ord_order-123\"}",
  "content_type": "application/json; charset=utf-8"
}
```

This lets Cnerium replay the same HTTP response when a client retries the same durable request.

## Idempotency flow with SDK storage

First request:

```txt
Cnerium checks request hash key
  -> not found

Cnerium executes handler

Cnerium writes:
  -> request hash
  -> stored response
```

Retry with same body:

```txt
Cnerium checks request hash key
  -> found

Cnerium compares hashes
  -> same hash

Cnerium loads stored response
  -> returns stored response
```

Retry with different body:

```txt
Cnerium checks request hash key
  -> found

Cnerium compares hashes
  -> different hash

Cnerium returns:
  -> 409 Conflict
```

## Why Cnerium uses the SDK instead of internals

The SDK exists to hide Softadastra internals behind a stable API.

This keeps Cnerium clean:

```txt
Cnerium
  -> depends on public SDK

Softadastra SDK
  -> owns internal runtime construction

Softadastra internals
  -> store, WAL, sync, transport, discovery, metadata
```

If Cnerium used internals directly, it would become fragile.

Any internal Softadastra refactor could break Cnerium.

## Client options

Cnerium configures the SDK client through:

```cpp
softadastra::sdk::ClientOptions
```

The MVP uses a durable client configuration.

Conceptually:

```cpp
auto options =
    softadastra::sdk::ClientOptions::durable(
        node_id,
        wal_path);

softadastra::sdk::Client client{options};
```

Then Cnerium wraps it:

```cpp
cnerium::store::Store store{std::move(client)};
```

## AppRuntime and SDK client

Cnerium creates the SDK-backed store inside `AppRuntime`.

```txt
AppRuntime
  -> builds Softadastra SDK Client
  -> creates Store
  -> opens Store on start()
  -> closes Store on stop()
```

This keeps storage lifecycle tied to the Cnerium app lifecycle.

## Sync tick

The SDK exposes manual sync advancement through:

```cpp
client.tick();
```

Cnerium exposes a small helper:

```cpp
store.tick();
```

The MVP does not build advanced sync orchestration.

It only keeps a path open for applications that want to advance the SDK sync pipeline manually.

## Sync state

The SDK can expose sync state through:

```cpp
client.sync_state();
```

Future Cnerium versions can use this for:

```txt
health endpoint
admin diagnostics
developer dashboard
observability
reliability reports
```

The MVP does not require a full dashboard.

## Transport and discovery

The Softadastra SDK exposes transport and discovery features.

Cnerium should not expose them directly in the first version.

Future versions can use them for:

```txt
node discovery
peer sync
cluster diagnostics
local-first app coordination
edge node integration
```

For the MVP, durable route storage is enough.

## Error handling

The SDK has its own error type:

```cpp
softadastra::sdk::Error
```

Cnerium has its own public error type:

```cpp
cnerium::support::Error
```

Cnerium should convert SDK errors into Cnerium errors when exposing errors publicly.

Application code should not need to handle raw SDK errors for normal Cnerium usage.

## Relationship with Cnerium Store

The dependency direction is:

```txt
cnerium::store::Store
  -> softadastra::sdk::Client
  -> Softadastra SDK internals
```

The application should normally use:

```txt
cnerium::App
cnerium::DurableRoute
cnerium::Store
```

not:

```txt
softadastra::sdk::Client
```

unless it needs advanced SDK control.

## Relationship with durable routes

Durable routes depend on the SDK through `Store`.

```txt
DurableRoute
  -> Idempotency
      -> ReplayProtection
          -> Store
              -> softadastra::sdk::Client
```

This keeps the durable route logic independent from low-level SDK details.

## Relationship with Vix

Vix owns HTTP and WebSocket execution.

Softadastra SDK owns durable storage.

Cnerium connects them:

```txt
Vix HTTP request
  -> Cnerium durable route
      -> Softadastra SDK-backed Store
  -> Vix HTTP response

Cnerium realtime event
  -> Vix WebSocket
```

## MVP boundary

The MVP uses the SDK for:

```txt
opening a local durable client
put
get
contains
remove
stored response persistence
manual tick helper
```

The MVP does not yet use the SDK for:

```txt
advanced peer discovery
advanced transport control
cluster management
sync dashboard
metadata dashboard
admin tools
```

## Future Cnerium use of the SDK

Future versions can add:

```txt
durable jobs
outbox-backed events
audit logs
event replay
sync diagnostics
peer health
node identity
admin status endpoint
local-first app state
```

All of these should still use the public SDK API first.

## Summary

Cnerium uses the Softadastra SDK as its durability foundation.

The SDK owns:

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
application-level reliability rules
```

The main rule is:

```txt
Cnerium depends on the public Softadastra SDK.
Cnerium does not depend on Softadastra internals.
```
