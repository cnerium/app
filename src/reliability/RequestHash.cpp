/**
 *
 *  @file RequestHash.cpp
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

#include <cnerium/reliability/RequestHash.hpp>

#include <array>
#include <utility>

namespace cnerium::reliability
{
  namespace
  {
    constexpr std::uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
    constexpr std::uint64_t FNV_PRIME = 1099511628211ULL;

    constexpr std::array<char, 16> HEX_DIGITS{
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        'a',
        'b',
        'c',
        'd',
        'e',
        'f'};
  } // namespace

  RequestHash::RequestHash(std::string value)
      : value_(std::move(value))
  {
  }

  RequestHash::RequestHash(std::string_view value)
      : value_(value)
  {
  }

  RequestHash::RequestHash(const char *value)
      : value_(value == nullptr ? "" : value)
  {
  }

  RequestHash RequestHash::from(std::string value)
  {
    return RequestHash{std::move(value)};
  }

  RequestHash RequestHash::from_body(std::string_view body)
  {
    return RequestHash{to_hex(fnv1a64(body))};
  }

  std::uint64_t RequestHash::fnv1a64(std::string_view value) noexcept
  {
    std::uint64_t hash = FNV_OFFSET_BASIS;

    for (const unsigned char character : value)
    {
      hash ^= static_cast<std::uint64_t>(character);
      hash *= FNV_PRIME;
    }

    return hash;
  }

  std::string RequestHash::to_hex(std::uint64_t value)
  {
    std::string output;
    output.resize(16);

    for (int index = 15; index >= 0; --index)
    {
      output[static_cast<std::size_t>(index)] =
          HEX_DIGITS[static_cast<std::size_t>(value & 0x0FULL)];

      value >>= 4U;
    }

    return output;
  }

  const std::string &RequestHash::str() const noexcept
  {
    return value_;
  }

  const std::string &RequestHash::value() const noexcept
  {
    return value_;
  }

  bool RequestHash::empty() const noexcept
  {
    return value_.empty();
  }

  bool RequestHash::is_valid() const noexcept
  {
    return !value_.empty();
  }

  bool RequestHash::valid() const noexcept
  {
    return is_valid();
  }

  void RequestHash::clear() noexcept
  {
    value_.clear();
  }

} // namespace cnerium::reliability
