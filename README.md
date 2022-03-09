# lua-sfmt

[![test](https://github.com/mah0x211/lua-sfmt/actions/workflows/test.yml/badge.svg)](https://github.com/mah0x211/lua-sfmt/actions/workflows/test.yml)
[![codecov](https://codecov.io/gh/mah0x211/lua-sfmt/branch/master/graph/badge.svg)](https://codecov.io/gh/mah0x211/lua-sfmt)

lua bindings for https://github.com/MersenneTwister-Lab/SFMT


## Installation

```sh
luarocks install sfmt
```

## Usage

```lua
local sfmt = require('sfmt')

-- gen 32bit random number
print(sfmt.rand32()) -- 600594209

-- gen 64bit random number
--
-- NOTE: If rand32 function has been called previously, it must be reinitialized 
-- before calling rand64 function.
--
-- NOTE: If no seed value is passed, a default seeds are automatically
-- generated as follows:
--
--  // set default seeds
--  srandom((unsigned int)time(NULL));
--  srandom((unsigned int)random());
--  for (int i = 0; i < 4; i++) {
--      lua_pushinteger(L, random());
--  }
--
sfmt.init()
print(sfmt.rand64()) -- 8.6473937871833e+17

-- converts 32bit random number to a double on (0,1)-real-interval.
print(sfmt.real1()) -- 0.017114813676364
print(sfmt.real2()) -- 0.35999271506444
print(sfmt.real3()) -- 0.017114813676364

-- gen random number on (0,1)-real-interval with 53-bit resolution using two 
-- 32bit integers.
print(sfmt.res53mix()) -- 0.45613130475084

-- converts 64bit random number to a double on (0,1)-real-interval with 53bit 
-- resolution.
sfmt.init()
print(sfmt.res53()) -- 0.81847656181705

-- init seed
sfmt.init(123456)
print(sfmt.rand32()) -- 1126405254

-- create an independent random number generator object
local s = sfmt.new( --[[seed, ...]] )
print(s:rand32()) -- 1587595272
```

