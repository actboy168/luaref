#include <lua.hpp>

#include "luaref.h"

#define FREELIST 1

static luaref getLr(lua_State *L) {
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    return (luaref)lua_touserdata(L, 1);
}

static int ref_init(lua_State *L) {
    luaref ref = luaref_init(L);
    lua_pushlightuserdata(L, (void *)ref);
    return 1;
}

static int ref_close(lua_State *L) {
    luaref ref = getLr(L);
    luaref_close(ref);
    return 0;
}

static int ref_isvalid(lua_State *L) {
    luaref ref = getLr(L);
    int r      = (int)luaL_checkinteger(L, 2);
    lua_pushboolean(L, luaref_isvalid(ref, r));
    return 1;
}

static int ref_ref(lua_State *L) {
    luaref ref = getLr(L);
    lua_settop(L, 2);
    int r = luaref_ref(ref, L);
    if (r == LUA_NOREF) {
        return luaL_error(L, "Too many refs.");
    }
    if (r <= 1) {
        return luaL_error(L, "Unexpected error.");
    }
    lua_pushinteger(L, r);
    return 1;
}

static int ref_unref(lua_State *L) {
    luaref ref = getLr(L);
    int r      = (int)luaL_checkinteger(L, 2);
    luaref_unref(ref, r);
    return 0;
}

static int ref_get(lua_State *L) {
    luaref ref = getLr(L);
    int r      = (int)luaL_checkinteger(L, 2);
    if (!luaref_isvalid(ref, r)) {
        return luaL_error(L, "invalid ref: %d", r);
    }
    luaref_get(ref, L, r);
    return 1;
}

static int ref_set(lua_State *L) {
    luaref ref = getLr(L);
    int r      = (int)luaL_checkinteger(L, 2);
    lua_settop(L, 3);
    if (!luaref_isvalid(ref, r)) {
        return luaL_error(L, "invalid ref: %d", r);
    }
    luaref_set(ref, L, r);
    return 1;
}

#if defined(_WIN32)
#    define DLLEXPORT __declspec(dllexport)
#else
#    define DLLEXPORT __attribute__((visibility("default")))
#endif

extern "C" DLLEXPORT int luaopen_testlib_luaref(lua_State *L) {
    struct luaL_Reg l[] = {
        { "init", ref_init },
        { "close", ref_close },
        { "isvalid", ref_isvalid },
        { "ref", ref_ref },
        { "unref", ref_unref },
        { "get", ref_get },
        { "set", ref_set },
        { NULL, NULL },
    };
    luaL_newlib(L, l);
    return 1;
}
