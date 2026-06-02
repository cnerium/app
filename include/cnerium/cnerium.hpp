/**
 *
 *  @file cnerium.hpp
 *  @author Softadastra
 *
 *  Copyright 2026, Softadastra.
 *  All rights reserved.
 *  https://github.com/softadastra/cnerium.git
 *
 *  Licensed under the MIT License.
 *
 *  Cnerium
 *
 */

#ifndef CNERIUM_HPP
#define CNERIUM_HPP

#include <cnerium/Version.hpp>

/*
 * Cnerium application layer.
 *
 * Cnerium attaches to an existing vix::App.
 * Vix remains the owner of the backend runtime, HTTP server, routing,
 * middleware, WebSocket runtime and developer workflow.
 */
#include <cnerium/app/App.hpp>
#include <cnerium/app/AppConfig.hpp>
#include <cnerium/app/AppRuntime.hpp>

/*
 * Durable HTTP layer.
 *
 * These types are used when a Vix route must become durable,
 * idempotent and retry-safe.
 */
#include <cnerium/http/DurableHandler.hpp>
#include <cnerium/http/DurableRequest.hpp>
#include <cnerium/http/DurableResponse.hpp>
#include <cnerium/http/DurableRoute.hpp>

/*
 * Realtime event layer.
 *
 * Cnerium emits application-level events through Vix WebSocket.
 */
#include <cnerium/realtime/Event.hpp>
#include <cnerium/realtime/EventPayload.hpp>
#include <cnerium/realtime/Realtime.hpp>
#include <cnerium/realtime/RealtimeConfig.hpp>

/*
 * Reliability layer.
 *
 * These are the core primitives behind durable routes:
 * idempotency, request hashing and replay protection.
 */
#include <cnerium/reliability/DurableResult.hpp>
#include <cnerium/reliability/Idempotency.hpp>
#include <cnerium/reliability/IdempotencyKey.hpp>
#include <cnerium/reliability/ReplayProtection.hpp>
#include <cnerium/reliability/RequestHash.hpp>

/*
 * Store layer.
 *
 * Cnerium stores framework metadata and replayable responses through
 * the public Softadastra SDK.
 */
#include <cnerium/store/Store.hpp>
#include <cnerium/store/StoreKey.hpp>
#include <cnerium/store/StoredResponse.hpp>

/*
 * Support layer.
 */
#include <cnerium/support/Error.hpp>
#include <cnerium/support/Json.hpp>
#include <cnerium/support/Result.hpp>
#include <cnerium/support/String.hpp>

#endif // CNERIUM_HPP
