#include <lua.hpp>
#include <iostream>

int main()
{
    lua_State *L = lua_open();
    luaL_openlibs(L);
    int s = luaL_loadfile(L, "test.lua");
    if (0 == s) {
	s = lua_pcall(L, 0, LUA_MULTRET, 0);
    }
    if (0 != s) {
	std::cerr << lua_tostring(L, -1) << std::endl;
	lua_pop(L, 1);
    }
    lua_close(L);
    return 0;
}
