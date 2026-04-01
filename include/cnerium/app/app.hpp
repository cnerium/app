/**
 * @file app.hpp
 * @brief cnerium::app — Main public header for the App module
 *
 * @version 0.1.0
 * @author Gaspard Kirira
 * @license MIT
 *
 * @details
 * Aggregates the public API of the Cnerium App layer.
 *
 * Include this file to access the full high-level framework:
 *   - version information
 *   - application configuration
 *   - request/response context
 *   - handler abstraction
 *   - main App class
 *
 * Notes:
 *   - This is the only header most users need
 *   - Lower-level modules (runtime, server, etc.) remain accessible
 *
 * Usage:
 * @code
 *   #include <cnerium/app/app.hpp>
 *
 *   using namespace cnerium::app;
 *
 *   int main()
 *   {
 *     App app;
 *
 *     app.get("/", [](AppContext &ctx)
 *     {
 *       ctx.text("Hello from Cnerium");
 *     });
 *
 *     app.run();
 *   }
 * @endcode
 */

#pragma once

#include <cnerium/app/version.hpp>
#include <cnerium/app/AppConfig.hpp>
#include <cnerium/app/AppContext.hpp>
#include <cnerium/app/AppHandler.hpp>
#include <cnerium/app/App.hpp>
