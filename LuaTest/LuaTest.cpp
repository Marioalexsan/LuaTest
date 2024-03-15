// LuaTest.cpp : Defines the entry point for the application.
//

#include <stdio.h>
#include <string.h>
#include <lua.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#define LUACHECK(x)                                     \
do {                                                    \
    int error = x;                                      \
    if (error != 0) {                                   \
        fprintf(stderr, "%s", lua_tostring(L, -1));     \
        lua_pop(L, 1);                                  \
    }                                                   \
} while (false);

class Engine {
    lua_State* L;
public:
    Engine() {
        L = luaL_newstate();
        luaL_openlibs(L);
    }

    ~Engine() {
        lua_close(L);
    }

    void loadScript(const std::string& script, const std::string& name, const std::string& debug = "loadedScript") {
        LUACHECK(luaL_loadbuffer(L, script.c_str(), script.size(), debug.c_str()));
        LUACHECK(lua_pcall(L, 0, 1, 0));
        lua_setglobal(L, name.c_str());
    }

    void runUpdate(const std::string& scriptName) {
        lua_getglobal(L, scriptName.c_str());
        lua_getfield(L, -1, "update");
        LUACHECK(lua_pcall(L, 0, 0, 0));
        lua_remove(L, -1);
    }
};

int main()
{
    Engine engine;

    std::ifstream script("script.lua");

    std::ostringstream sstr;
    sstr << script.rdbuf();

    engine.loadScript(sstr.str(), "script1");
    engine.runUpdate("script1");
    engine.runUpdate("script1");
    engine.runUpdate("script1");
    engine.runUpdate("script1");
    engine.runUpdate("script1");
    engine.runUpdate("script1");

    std::cout << "Done" << std::endl;

	return 0;
}
