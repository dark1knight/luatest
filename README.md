# Example of calling a C-library in lua

### Steps
1) write C functions and register them with lua using ```luaL_reg```
2) compile the C library to a shared object
3) load that shared object in lua using ```package.loadlib```

Details in ```testlib.lua, Makefile and functions.c```
