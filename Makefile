default:
	g++ luatest.cpp -I/usr/include/lua5.1/ -L/usr/lib/i386-linux-gnu/ -llua5.1-c++

class:
	g++ class_l.cpp -I/usr/include/lua5.1/ -L/usr/lib/i386-linux-gnu/ -llua5.1-c++
