l = package.loadlib("./libmylib.so", "luaopen_mylib")
m = l()

tokens = m.split("Hell world", " ")
for k, v in pairs(tokens) do
    print(v)
end


