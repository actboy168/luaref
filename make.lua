local lm = require "luamake"

lm.mode = "debug"

lm:lua_dll "testlib" {
    sources = {
        "luaref.cpp",
        "test.cpp",
    },
    export_luaopen = "off",
}

lm:runlua "runtest" {
    deps = "testlib",
    script = "test.lua"
}
