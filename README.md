# cnerium/app

High-level application framework for the Cnerium web stack.

**Header-only. Deterministic. App + Routing + Runtime.**

---

## Download

https://vixcpp.com/registry/pkg/cnerium/app

---

## Overview

`cnerium/app` is the user-facing layer of the Cnerium framework.

It integrates:

- server (`cnerium/server`)
- runtime (`cnerium/runtime`)

It is responsible for:

- exposing a simple application API
- registering routes and middleware
- orchestrating server and runtime
- running the application

It is designed to be:

- minimal
- predictable
- fully deterministic
- easy to use

---

## Why cnerium/app?

Most frameworks mix:

- routing
- middleware
- concurrency
- networking

This leads to:

- hidden complexity
- hard debugging
- tight coupling
- unpredictable behavior

`cnerium/app` separates concerns:

- **app** → user API
- **server** → HTTP execution
- **runtime** → concurrency

Result:

- clean architecture
- simple API
- scalable system

---

## Dependencies

Depends on:

- `cnerium/server`
- `cnerium/runtime`

This ensures:

- clean separation between layers
- reusable execution engine
- flexible architecture

---

## Installation

### Using Vix

```bash
vix add @cnerium/app
vix install
```

### Manual

```bash
git clone https://github.com/cnerium/app.git
```

Add `include/` to your project.

---

## Core Concepts

### App

```cpp
App app;
```

Main entry point of the framework.

### Route

```cpp
app.get("/users/:id", [](AppContext &ctx)
{
  ctx.text("user");
});
```

### Middleware

```cpp
app.use([](auto &ctx, auto next)
{
  ctx.response().set_header("X-App", "Cnerium");
  next();
});
```

### Context

```cpp
AppContext ctx;
```

Provides access to:

- request
- response
- params

With helpers:

```cpp
ctx.text("hello");
ctx.json({...});
ctx.status(Status::ok);
```

### Runtime

```cpp
app.runtime().post([]()
{
  // background task
});
```

---

## Typical Flow

```text
HTTP → server → app handler → runtime → response
```

---

## Example

```cpp
#include <cnerium/app/app.hpp>

using namespace cnerium::app;

int main()
{
  App app;

  app.get("/", [](AppContext &ctx)
  {
    ctx.text("Hello from Cnerium");
  });

  app.run();
}
```

---

## Routing Example

```cpp
app.get("/users/:id", [](AppContext &ctx)
{
  ctx.text(std::string(ctx.params().get("id")));
});
```

---

## Middleware Example

```cpp
app.use([](auto &ctx, auto next)
{
  if (ctx.request().path() == "/admin")
  {
    ctx.text("Access denied");
    return;
  }

  next();
});
```

---

## JSON Response Example

```cpp
app.get("/", [](AppContext &ctx)
{
  ctx.json({
    {"ok", true},
    {"framework", "Cnerium"}
  });
});
```

---

## Runtime Example

```cpp
app.runtime().post([]()
{
  // async work
});
```

---

## Execution Rules

- routes are evaluated in insertion order
- first match wins
- middleware runs before handler
- middleware can stop execution
- runtime executes tasks concurrently

---

## Complexity

| Operation | Complexity |
|----------|-----------|
| route registration | O(1) |
| route matching | O(n) |
| middleware execution | O(n) |
| task scheduling | O(1) |

---

## Architecture

```text
App → Server → Router → Middleware → Handler → Runtime → Worker
```

---

## Design Philosophy

- minimal API surface
- deterministic execution
- explicit behavior
- no hidden magic
- composable system

---

## Tests

```bash
vix build
vix test
```

---

## License

MIT License
Copyright (c) Gaspard Kirira

