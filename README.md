# lua-sfmt

[![test](https://github.com/mah0x211/lua-sfmt/actions/workflows/test.yml/badge.svg)](https://github.com/mah0x211/lua-sfmt/actions/workflows/test.yml)
[![codecov](https://codecov.io/gh/mah0x211/lua-sfmt/branch/master/graph/badge.svg)](https://codecov.io/gh/mah0x211/lua-sfmt)

lua bindings for https://github.com/MersenneTwister-Lab/SFMT


## Installation

```sh
luarocks install sfmt
```

***


## v = sfmt.rand32( [max [, min]] )

generates a pseudo-random 32bit unsigned integer.

**Parameters**

- `max:integer`: a maximum value. (default: maximum value of `lua_Integer`)
- `min:integer`: a minimum value. (default: `0`)

**Returns**

- `v:integer`: a pseudo-random 32bit unsigned integer.

**Example**

```lua
local sfmt = require('sfmt')
local v = sfmt.rand32()
print(v) -- 600594209
```


## v = sfmt.rand64( [max [, min]] )

generates a pseudo-random 64bit unsigned integer.

**Parameters**

- `max:integer`: a maximum value. (default: maximum value of `lua_Integer`)
- `min:integer`: a minimum value. (default: `0`)

**Returns**

- `v:integer`: a pseudo-random 64bit unsigned integer.

**Example**

```lua
local sfmt = require('sfmt')
local v = sfmt.rand64()
print(v) -- 8.6473937871833e+17
```


## v = sfmt.real1( [max [, min]] )

generates a pseudo-random number on closed interval [0,1]. (0 <= v <= 1)

**Parameters**

- `max:number`: a maximum value. (default: `1`)
- `min:number`: a minimum value. (default: `0`)

**Returns**

- `v:number`: a pseudo-random number.

**Example**

```lua
local sfmt = require('sfmt')
local v = sfmt.real1()
print(v) -- 0.017114813676364
```


## v = sfmt.real2( [max [, min]] )

generates a pseudo-random number on half-open interval [0,1). (0 <= v < 1)

**Parameters**

- `max:number`: a maximum value. (default: `1`)
- `min:number`: a minimum value. (default: `0`)

**Returns**

- `v:number`: a pseudo-random number.

**Example**

```lua
local sfmt = require('sfmt')
local v = sfmt.real2()
print(v) -- 0.35999271506444
```


## v = sfmt.real3( [max [, min]] )

generates a pseudo-random number on open interval (0,1). (0 < v < 1)

**Parameters**

- `max:number`: a maximum value. (default: `1`)
- `min:number`: a minimum value. (default: `0`)

**Returns**

- `v:number`: a pseudo-random number.

**Example**

```lua
local sfmt = require('sfmt')
local v = sfmt.real3()
print(v) -- 0.017114813676364
```


## v = sfmt.res53( [max [, min]] )

generates a pseudo-random number on half-open interval [0,1) with 53-bit resolution. (0 <= v < 1)

**Parameters**

- `max:number`: a maximum value. (default: `1`)
- `min:number`: a minimum value. (default: `0`)

**Returns**

- `v:number`: a pseudo-random number with 53-bit resolution.

**Example**

```lua
local sfmt = require('sfmt')
local v = sfmt.res53()
print(v) -- 0.35999271506444
```


## v = sfmt.res53mix( [max [, min]] )

generates a pseudo-random number on half-open interval [0,1) with 53-bit resolution using two 32bit integers.

**Parameters**

- `max:number`: a maximum value. (default: `1`)
- `min:number`: a minimum value. (default: `0`)

**Returns**

- `v:number`: a pseudo-random number with 53-bit resolution.

**Example**

```lua
local sfmt = require('sfmt')
local v = sfmt.res53mix()
print(v) -- 0.35999271506444
```


## sfmt.init( [seed, ...] )

initializes the default generator with an unsigned 32bit integer seed value.

**Parameters**

- `seed:integer`: seed value.
- `...:integer`: additional seed values.

**NOTE**

If no seed value is passed, a default seeds are automatically generated as follows:

```c
// create 4 seed values
for (int i = 0; i < 4; i++) {
    struct timespec t = {0};
    clock_gettime(CLOCK_MONOTONIC, &t);
    lua_pushinteger(L, t.tv_nsec);
}
// initialize the default generator using the above seed values
// ...
```


## s = sfmt.new( [seed, ...] )

creates an independent random number generator object.

**Parameters**

- `seed:integer`: seed value.
- `...:integer`: additional seed values.

**Returns**

- `s:sfmt`: an independent random number generator object. this object has the same methods as the default generator.
    - `s:init()`: same as `sfmt.init()`
    - `s:rand32()`: same as `sfmt.rand32()`
    - `s:rand64()`: same as `sfmt.rand64()`
    - `s:real1()`: same as `sfmt.real1()`
    - `s:real2()`: same as `sfmt.real2()`
    - `s:real3()`: same as `sfmt.real3()`
    - `s:res53()`: same as `sfmt.res53()`
    - `s:res53mix()`: same as `sfmt.res53mix()`

**Example**

```lua
local sfmt = require('sfmt')
local s = sfmt.new()
print(s) -- sfmt: 0x12b80fc38
print(s:rand32()) -- 1587595272
```


## License

The MIT License (MIT)

