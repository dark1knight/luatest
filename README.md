# Example of calling a C-library in lua

### Steps

- write C functions and register them with lua using ```luaL_reg```
- compile the C library to a shared object
- load that shared object in lua using ```package.loadlib```

Details in ```testlib.lua, Makefile and functions.c```
