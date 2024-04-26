/**
 *  Copyright (C) 2022 Masatoshi Fukunaga
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 */

#include "SFMT.h"
#include <time.h>
// lua
#include <lauxhlib.h>

#define MODULE_MT "sfmt"

static int DEFAULT_SFMT_REF = LUA_NOREF;

typedef enum {
    RANDBIT_ZERO = 0,
    RANDBIT_UINT32,
    RANDBIT_UINT64,
    RANDBIT_REAL1,
    RANDBIT_REAL2,
    RANDBIT_REAL3,
    RANDBIT_RES53,
    RANDBIT_RES53_mix
} randbit_t;

typedef struct {
    sfmt_t sfmt;
    randbit_t bit;
} lua_sfmt_t;

static inline lua_sfmt_t *checksfmt(lua_State *L, randbit_t bit)
{
    lua_sfmt_t *s = (lua_sfmt_t *)luaL_checkudata(L, 1, MODULE_MT);

    if (s->bit == 0) {
        s->bit = bit;
    } else if (s->bit != bit) {
        // automatically reinitialize sfmt_t
        uint32_t seeds[4] = {random(), random(), random(), random()};
        sfmt_init_by_array(&s->sfmt, seeds, 4);
    }
    return s;
}

static const lua_Number LUA_SFMT_NUMMAX =
    std::numeric_limits<lua_Number>::max();

static inline lua_Number dbl2num(double v)
{
    if (v > (double)LUA_SFMT_NUMMAX) {
        return LUA_SFMT_NUMMAX;
    }
    return (lua_Number)v;
}

#define genrand_res(L, bit)                                                    \
    do {                                                                       \
        lua_sfmt_t *s  = checksfmt(L, RANDBIT_RES##bit);                       \
        lua_Number max = lauxh_optunsigned(L, 2, 1);                           \
        lua_Number min = lauxh_optunsigned(L, 3, 0);                           \
        double v       = 0;                                                    \
        lauxh_argcheck(L, min <= max, 2,                                       \
                       "min must be less than or equal to max");               \
        v = sfmt_genrand_res##bit(&s->sfmt);                                   \
        v = min + v * (max - min);                                             \
        lua_pushnumber((L), v);                                                \
    } while (0)

static int res53mix_lua(lua_State *L)
{
    genrand_res(L, 53_mix);
    return 1;
}

static int res53_lua(lua_State *L)
{
    genrand_res(L, 53);
    return 1;
}

#define default_genrand_res(L, bit)                                            \
    do {                                                                       \
        lauxh_pushref((L), DEFAULT_SFMT_REF);                                  \
        lua_insert(L, 1);                                                      \
        genrand_res((L), bit);                                                 \
    } while (0)

static int default_res53mix_lua(lua_State *L)
{
    default_genrand_res(L, 53_mix);
    return 1;
}

static int default_res53_lua(lua_State *L)
{
    default_genrand_res(L, 53);
    return 1;
}

#undef default_genrand_res
#undef genrand_res

#define genrand_real(L, bit)                                                   \
    do {                                                                       \
        lua_sfmt_t *s  = checksfmt(L, RANDBIT_REAL##bit);                      \
        lua_Number max = lauxh_optunsigned(L, 2, 1);                           \
        lua_Number min = lauxh_optunsigned(L, 3, 0);                           \
        double v       = 0;                                                    \
        lauxh_argcheck(L, min <= max, 2,                                       \
                       "min must be less than or equal to max");               \
        v = sfmt_genrand_real##bit(&s->sfmt);                                  \
        v = min + v * (max - min);                                             \
        lua_pushnumber((L), dbl2num(v));                                       \
    } while (0)

static int real3_lua(lua_State *L)
{
    genrand_real(L, 3);
    return 1;
}

static int real2_lua(lua_State *L)
{
    genrand_real(L, 2);
    return 1;
}

static int real1_lua(lua_State *L)
{
    genrand_real(L, 1);
    return 1;
}

#define default_genrand_real(L, bit)                                           \
    do {                                                                       \
        lauxh_pushref((L), DEFAULT_SFMT_REF);                                  \
        lua_insert(L, 1);                                                      \
        genrand_real((L), bit);                                                \
    } while (0)

static int default_real3_lua(lua_State *L)
{
    default_genrand_real(L, 3);
    return 1;
}

static int default_real2_lua(lua_State *L)
{
    default_genrand_real(L, 2);
    return 1;
}

static int default_real1_lua(lua_State *L)
{
    default_genrand_real(L, 1);
    return 1;
}

#undef default_genrand_real
#undef genrand_real

static const lua_Integer LUA_SFMT_INTMAX =
    std::numeric_limits<lua_Integer>::max();

static inline lua_Integer uint2int(uintmax_t v)
{
    if (v > (uintmax_t)LUA_SFMT_INTMAX) {
        return LUA_SFMT_INTMAX;
    }
    return (lua_Integer)v;
}

#define genrand_uint(L, bit)                                                    \
    do {                                                                        \
        lua_sfmt_t *s   = checksfmt(L, RANDBIT_UINT##bit);                      \
        uintmax_t max   = (uintmax_t)lauxh_optuint##bit(L, 2, LUA_SFMT_INTMAX); \
        uintmax_t min   = (uintmax_t)lauxh_optuint##bit(L, 3, 0);               \
        uint##bit##_t v = 0;                                                    \
        lauxh_argcheck(L, min <= max, 2,                                        \
                       "min must be less than or equal to max");                \
        v = sfmt_genrand_uint##bit(&s->sfmt);                                   \
        v = min + (v % (max - min + 1));                                        \
        lua_pushinteger((L), uint2int(v));                                      \
    } while (0)

static int rand64_lua(lua_State *L)
{
    genrand_uint(L, 64);
    return 1;
}

static int rand32_lua(lua_State *L)
{
    genrand_uint(L, 32);
    return 1;
}

#define default_genrand_uint(L, bit)                                           \
    do {                                                                       \
        lauxh_pushref((L), DEFAULT_SFMT_REF);                                  \
        lua_insert(L, 1);                                                      \
        genrand_uint((L), bit);                                                \
    } while (0)

static int default_rand64_lua(lua_State *L)
{
    default_genrand_uint(L, 64);
    return 1;
}

static int default_rand32_lua(lua_State *L)
{
    default_genrand_uint(L, 32);
    return 1;
}

#undef default_genrand_uint
#undef genrand_uint

static inline void init_sfmt(lua_State *L)
{
    int top       = lua_gettop(L);
    lua_sfmt_t *s = (lua_sfmt_t *)luaL_checkudata(L, 1, MODULE_MT);

    s->bit = RANDBIT_ZERO;
    switch (top) {
    case 2: {
        uint32_t seed = lauxh_checkuint32(L, 2);
        sfmt_init_gen_rand(&s->sfmt, seed);
    } break;

    case 1:
        // set default seeds
        {
            struct timespec t = {};
            clock_gettime(CLOCK_MONOTONIC, &t);
            srand(((uint64_t)t.tv_sec * 1000000000ULL + t.tv_nsec));
        }
        for (int i = 0; i < 4; i++) {
            lua_pushinteger(L, random());
        }
        top += 4;

    default: {
        int len = top - 1;
        uint32_t *seeds =
            (uint32_t *)lua_newuserdata(L, sizeof(uint32_t) * len);
        for (int i = 2; i <= top; i++) {
            seeds[i - 2] = lauxh_checkuint32(L, i);
        }
        sfmt_init_by_array(&s->sfmt, seeds, len);
    }
    }
}

static int init_lua(lua_State *L)
{
    init_sfmt(L);
    lua_settop(L, 1);
    return 1;
}

static int default_init_lua(lua_State *L)
{
    lauxh_pushref(L, DEFAULT_SFMT_REF);
    lua_insert(L, 1);
    init_sfmt(L);
    return 0;
}

static int tostring_lua(lua_State *L)
{
    luaL_checkudata(L, 1, MODULE_MT);
    lua_pushfstring(L, MODULE_MT ": %p", lua_topointer(L, 1));
    return 1;
}

static int new_lua(lua_State *L)
{
    lua_newuserdata(L, sizeof(lua_sfmt_t));
    lauxh_setmetatable(L, MODULE_MT);
    lua_insert(L, 1);
    return init_lua(L);
}

static int idstring_lua(lua_State *L)
{
    lua_pushstring(L, sfmt_get_idstring(NULL));
    return 1;
}

#ifdef __cplusplus
extern "C" {
#endif

LUALIB_API int luaopen_sfmt(lua_State *L)
{
    // create metatable
    if (luaL_newmetatable(L, MODULE_MT)) {
        struct luaL_Reg mmethod[] = {
            {"__tostring", tostring_lua},
            {NULL,         NULL        }
        };
        struct luaL_Reg method[] = {
            {"init",     init_lua    },
            {"rand32",   rand32_lua  },
            {"rand64",   rand64_lua  },
            {"real1",    real1_lua   },
            {"real2",    real2_lua   },
            {"real3",    real3_lua   },
            {"res53",    res53_lua   },
            {"res53mix", res53mix_lua},
            {NULL,       NULL        }
        };

        // metamethods
        for (struct luaL_Reg *ptr = mmethod; ptr->name; ptr++) {
            lauxh_pushfn2tbl(L, ptr->name, ptr->func);
        }
        // methods
        lua_newtable(L);
        for (struct luaL_Reg *ptr = method; ptr->name; ptr++) {
            lauxh_pushfn2tbl(L, ptr->name, ptr->func);
        }
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
    }

    // create default lua_sfmt_t
    lua_newuserdata(L, sizeof(lua_sfmt_t));
    lauxh_setmetatable(L, MODULE_MT);
    DEFAULT_SFMT_REF = lauxh_ref(L);
    lua_settop(L, 0);
    default_init_lua(L);
    lua_settop(L, 0);

    lua_createtable(L, 0, 0);
    lauxh_pushfn2tbl(L, "idstring", idstring_lua);
    lauxh_pushfn2tbl(L, "init", default_init_lua);
    lauxh_pushfn2tbl(L, "rand32", default_rand32_lua);
    lauxh_pushfn2tbl(L, "rand64", default_rand64_lua);
    lauxh_pushfn2tbl(L, "real1", default_real1_lua);
    lauxh_pushfn2tbl(L, "real2", default_real2_lua);
    lauxh_pushfn2tbl(L, "real3", default_real3_lua);
    lauxh_pushfn2tbl(L, "res53", default_res53_lua);
    lauxh_pushfn2tbl(L, "res53mix", default_res53mix_lua);
    lauxh_pushfn2tbl(L, "new", new_lua);

    return 1;
}

#ifdef __cplusplus
}
#endif
