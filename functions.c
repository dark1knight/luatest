#include <lua.h>
#include <lauxlib.h>

#include <string.h>

static int l_split(lua_State *L) {
    const char *str = luaL_checkstring(L, 1);  // first argument
    const char *sep = luaL_checkstring(L, 2);  // second argument

    const char *e;
    int i = 1;

    lua_newtable(L);  // result table

    while ((e = strchr(str, *sep)) != NULL) {
        lua_pushlstring(L, str, e - str);  // push substring
        lua_rawseti(L, -2, i++);
        str = e + 1;  // advance string's current pos past separator
    }

    lua_pushstring(L, str);
    lua_rawseti(L, -2, i);

    return 1;
}

const struct luaL_reg mylib[] = {{"split", l_split}, {NULL, NULL}};

int luaopen_mylib(lua_State *L) {
    luaL_openlib(L, "mylib", mylib, 0);
    return 1;
}
