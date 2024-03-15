// LuaTest.cpp : Defines the entry point for the application.
//

#include <stdio.h>
#include <string.h>
#include <lua.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <memory>

#define LUACHECK(x)                                     \
do {                                                    \
    int error = x;                                      \
    if (error != 0) {                                   \
        fprintf(stderr, "%s", lua_tostring(L, -1));     \
        lua_pop(L, 1);                                  \
    }                                                   \
} while (false);

extern "C" {
    static int draw_rect(lua_State* L);
    static int draw_text(lua_State* L);
    static int draw_color(lua_State* L);
}

class Engine {
    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<sf::Font> font;
    sf::Color drawColor;

    lua_State* L;

    void loadEngineFunctions() {
        lua_pushcfunction(L, draw_rect);
        lua_setglobal(L, "draw_rect");
        lua_pushcfunction(L, draw_text);
        lua_setglobal(L, "draw_text");
        lua_pushcfunction(L, draw_color);
        lua_setglobal(L, "draw_color");
    }
public:
    Engine() {
        window = std::make_unique<sf::RenderWindow>(sf::VideoMode{ sf::Vector2u{800, 600} }, "LuaTest");
        font = std::make_unique<sf::Font>();
        if (!font->loadFromFile("CourierPrime-Regular.ttf"))
            std::cout << "balls" << std::endl;

        L = luaL_newstate();
        luaL_openlibs(L);

        loadEngineFunctions();
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

    void drawRect(double x, double y, double width, double height) {
        sf::RectangleShape sfRect({(float)width, (float)height});
        sfRect.setPosition({ (float)x, (float)y });
        sfRect.setFillColor(drawColor);

        window->draw(sfRect);
    }

    void drawText(double x, double y, unsigned int size, const char* text) {
        sf::Text sfText(*font);
        sfText.setPosition({ (float)x, (float)y });
        sfText.setCharacterSize((unsigned int)size);
        sfText.setFillColor(drawColor);
        sfText.setString(text);

        window->draw(sfText);
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b) {
        drawColor = sf::Color(r, g, b);
    }

    void update() {
        sf::Event event;
        while (window->pollEvent(event));

        window->clear(sf::Color::White);

        runUpdate("script1");

        window->display();
    }
};

std::unique_ptr<Engine> engine;

extern "C" {
    static int draw_rect(lua_State* L) {
        double x = lua_tonumber(L, 1);
        double y = lua_tonumber(L, 2);
        double width = lua_tonumber(L, 3);
        double height = lua_tonumber(L, 3);

        engine->drawRect(x, y, width, height);

        return 0;
    }

    static int draw_text(lua_State* L) {
        double x = lua_tonumber(L, 1);
        double y = lua_tonumber(L, 2);
        unsigned int size = (unsigned int)lua_tonumber(L, 3);
        const char* text = lua_tostring(L, 4);

        engine->drawText(x, y, size, text);

        return 0;
    }

    static int draw_color(lua_State* L) {
        uint8_t r = (uint8_t)std::clamp(lua_tonumber(L, 1), 0.0, 255.0);
        uint8_t g = (uint8_t)std::clamp(lua_tonumber(L, 1), 0.0, 255.0);
        uint8_t b = (uint8_t)std::clamp(lua_tonumber(L, 1), 0.0, 255.0);

        engine->setColor(r, g, b);

        return 0;
    }
}

int main()
{
    engine = std::make_unique<Engine>();

    std::ifstream script("script.lua");

    std::ostringstream sstr;
    sstr << script.rdbuf();

    engine->loadScript(sstr.str(), "script1");

    sf::Clock clock;
    sf::Time time;
    sf::Time frame;

    while (time < sf::seconds(100)) {
        sf::Time current = clock.restart();
        time += current;
        frame += current;

        if (frame > sf::milliseconds(20)) {
            frame -= sf::milliseconds(20);

            engine->update();
        }

        sf::sleep(sf::milliseconds(1));
    }

	return 0;
}
