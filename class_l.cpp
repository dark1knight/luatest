#include <iostream>
#include <vector>
#include <cassert>
#include <lua.hpp>

using namespace std;
typedef vector<int> ** intpvec;

// Lua RAII state
struct State {
    lua_State *L;
    State() { L = luaL_newstate(); luaL_openlibs(L); cout << "created lua state\n"; }
    ~State() { lua_close(L); cout << "destroyed lua state\n"; }
};

struct Library {
    State *state_p;
     void Run();
};

struct Complex {
    vector<int> ints;
    State      *state_p;

    void Run();
};

extern "C" {
    static int add5(lua_State *L);
    static int add10(lua_State *L);
}

void Library::Run()
{
    const struct luaL_reg mylib[] = {
	{"add5", add5},
	{"add10", add10},
	{NULL, NULL}
    };
    cout << "opening library...\n";
    luaL_openlib(state_p->L, "mylib", mylib, 0);

    int status = luaL_loadfile(state_p->L, "library.lua");
    if (0 != status) {
	cerr << "problem loading library.lua\n";
	return;
    }

    status = lua_pcall(state_p->L, 0, 0, 0);
    if (status) {
	cerr << "error = " << lua_tostring(state_p->L, -1) << "\n";
    }
}

extern "C" {
static int vec_size(lua_State *L);
static int vec_at(lua_State *L);
}

void Complex::Run()
{
    ints.push_back(10);
    const struct luaL_reg veclib[] = {
	{"size", vec_size},
	{"at", vec_at},
	{NULL, NULL}
    };
    cout << "opening library for vec...\n";
    
    intpvec ipv = (intpvec)lua_newuserdata(state_p->L, sizeof(vector<int> *)); // first on stack
    // push new user data onto stack 
    *ipv = (&ints); // set the 
    { // method 1
    cout << "lua_gettop = " << lua_gettop(state_p->L) << "\n";
    luaL_newmetatable(state_p->L, "A.veclib"); // second on stack
    cout << "lua_gettop = " << lua_gettop(state_p->L) << "\n";
    luaL_openlib(state_p->L, "veclib", veclib, 0);
    lua_setfield(state_p->L, -2, "__index"); // attaching __index to userdata
    cout << "lua_gettop = " << lua_gettop(state_p->L) << "\n";
    lua_setmetatable(state_p->L, -2); // set metatable on the userdata and pop it off
    cout << "lua_gettop = " << lua_gettop(state_p->L) << "\n";
    }
    { // method 2 - doesn't work. figure out why...
    //luaL_newmetatable(state_p->L, "A.b");
    //lua_pushstring(state_p->L, "__index");
    //lua_pushvalue(state_p->L, -2); // push the metatable
    //lua_settable(state_p->L, -3);
    //luaL_openlib(state_p->L, NULL, veclib, 0);
    }
    lua_setglobal(state_p->L, "my_vec");

    int status = luaL_loadfile(state_p->L, "complex.lua");
    if (0 != status) {
	cerr << "problem loading complex.lua\n";
	return;
    }

    status = lua_pcall(state_p->L, 0, 0, 0);
    if (status) {
	cerr << "error = " << lua_tostring(state_p->L, -1) << "\n";
    }
}

int main()
{
    State state;
    Complex comp;
    comp.state_p = &state;
    comp.Run();

    //Library l;
    //l.state_p = &state;
    //l.Run();

    //comp.Run();
    return 0;
}

extern "C"
{
int add5(lua_State *L)
{
    assert(lua_gettop(L) == 1); // there should always be 1 arg to this func
    int num = luaL_checknumber(L, 1);
    lua_pushnumber(L, num + 5);
    return 1;
}

int add10(lua_State *L)
{
    assert(lua_gettop(L) == 1); // there should always be 1 arg to this func
    int num = luaL_checknumber(L, 1);
    lua_pushnumber(L, num + 10);
    return 1;
}

int vec_size(lua_State *L)
{
    cout << "vec_size\n";
    assert(lua_gettop(L) == 1); // size only requires self
    cout << "getting intpvec...\n";
    intpvec ipv = (intpvec)lua_touserdata(L, 1);
    cout << "got ipv...\n";
    int sz = (*ipv)->size();
    cout << "size = " << sz << "\n";
    lua_pushnumber(L, sz);
    return 1;
}

int vec_at(lua_State *L)
{
    assert(lua_gettop(L) == 2); // size only requires self
    int idx = luaL_checknumber(L, -1); // top arg is index)
    cout << "index = " << idx << "\n";
    intpvec ipv = (intpvec)lua_touserdata(L, -2); // first arg is self
    int sz = (*ipv)->size();
    if (idx >= sz) { lua_pushnil(L); return 0; }
    lua_pushnumber(L, (**ipv)[idx]);
    return 1;
}
}
